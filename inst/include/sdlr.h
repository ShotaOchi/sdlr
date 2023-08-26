#ifndef SDLR_H
#define SDLR_H
#include <unordered_map>
#include <Rcpp.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#define SDLR_MINIMAL_WIDTH 200

inline std::vector<SDL_Point> SDLR_GetCirlePoints(const int& cx, const int& cy, const int& r)
{
  std::vector<SDL_Point> out;
  out.reserve(static_cast<size_t>(2 * r * M_PI + 1));
  std::function<void(int, int)> f = [&out, &cx, &cy](const int x, const int y)->void
  {
    if (x != y)
    {
      if (x != 0)
      {
        if (y != 0)
        {
          const int cxpx = cx + x;
          const int cxmx = cx - x;
          const int cxpy = cx + y;
          const int cxmy = cx - y;
          const int cypx = cy + x;
          const int cymx = cy - x;
          const int cypy = cy + y;
          const int cymy = cy - y;
          SDL_Point tmp = {cxpx, cypy};
          out.push_back(tmp);
          tmp.x = cxmx;
          out.push_back(tmp);
          tmp.y = cymy;
          out.push_back(tmp);
          tmp.x = cxpx;
          out.push_back(tmp);
          tmp.x = cxpy;
          tmp.y = cypx;
          out.push_back(tmp);
          tmp.x = cxmy;
          out.push_back(tmp);
          tmp.y = cymx;
          out.push_back(tmp);
          tmp.x = cxpy;
          out.push_back(tmp);
        } else
        {
          SDL_Point tmp = {cx + x, cy};
          out.push_back(tmp);
          tmp.x = cx - x;
          out.push_back(tmp);
          tmp.x = cx;
          tmp.y = cy + x;
          out.push_back(tmp);
          tmp.y = cy - x;
          out.push_back(tmp);
        }
      } else
      {
        SDL_Point tmp = {cx + y, cy};
        out.push_back(tmp);
        tmp.x = cx - y;
        out.push_back(tmp);
        tmp.x = cx;
        tmp.y = cy + y;
        out.push_back(tmp);
        tmp.y = cy - y;
        out.push_back(tmp);
      }
    } else
    {
      if (x != 0)
      {
        const int cxpx = cx + x;
        SDL_Point tmp = {cxpx, cy + x};
        out.push_back(tmp);
        tmp.x = cx - x;
        out.push_back(tmp);
        tmp.y = cy - x;
        out.push_back(tmp);
        tmp.x = cxpx;
        out.push_back(tmp);
      } else
      {
        SDL_Point tmp = {cx, cy};
        out.push_back(tmp);
      }
    }
  };
  int x = r;
  int y = 0;
  int d = 3 - (r + r);
  f(x,y);
  while (x >= y)
  {
    if (d <= 0)
    {
      d += 6 + 4 * y;
    } else
    {
      d += 10 + 4 * (y - x);
      --x;
    }
    ++y;
    f(x,y);
  }
  out.shrink_to_fit();
  return out;
}

inline void SDLR_RenderFillCirle(SDL_Renderer* renderer, const int cx, const int cy, const int r)
{
  int w, h;
  if (SDL_GetRendererOutputSize(renderer, &w, &h) != 0)
  {
    const std::string msg = SDL_GetError();
    Rcpp::stop("failed to get renderer output size.\n" + msg + "\n");
  }
  std::vector<SDL_Point> pts(SDLR_GetCirlePoints(cx, cy, r));
  std::sort(pts.begin(), pts.end(), [](const SDL_Point& a, const SDL_Point& b)
  {
    return a.x < b.x;
  });
  std::vector< std::pair<int, std::pair<int, int> > > vecypair; // first is x, second is min of y, third is max of y.
  vecypair.reserve(r + r + 1);
  std::pair<int, std::pair<int, int> > tmppair;
  tmppair.first = pts[0].x;
  tmppair.second.first = pts[0].y;
  tmppair.second.second = pts[0].y;
  for (size_t i = 0; i < pts.size(); ++i)
  {
    if (pts[i].x == tmppair.first)
    {
      if (pts[i].y < tmppair.second.first)
      {
        tmppair.second.first = pts[i].y;
      }
      if (pts[i].y > tmppair.second.second)
      {
        tmppair.second.second = pts[i].y;
      }
    } else
    {
      vecypair.push_back(tmppair);
      tmppair.first = pts[i].x;
      tmppair.second.first = pts[i].y;
      tmppair.second.second = pts[i].y;
    }
  }
  vecypair.push_back(tmppair);
  for (size_t i = 0; i < vecypair.size(); ++i)
  {
    if (vecypair[i].first >= 0 && vecypair[i].first < w)
    {
      for (int ypts = vecypair[i].second.first; ypts <= vecypair[i].second.second; ++ypts)
      {
        if (ypts >= 0 && ypts < h)
        {
          if (SDL_RenderDrawPoint(renderer, vecypair[i].first, ypts) != 0)
          {
            const std::string msg = SDL_GetError();
            Rcpp::stop("failed to draw point.\n" + msg + "\n");
          }
        }
      }
    }
  }
}

inline std::vector<SDL_Point> SDLR_GetLinePoints(const int& x1, const int& y1, const int& x2, const int& y2)
{
  const int dx = std::abs(x2 - x1);
  const int dy = std::abs(y2 - y1);
  const int n = std::max(dx + 1, dy + 1);
  std::vector<SDL_Point> out;
  out.reserve(n);
  if (x1 == x2)
  {
    if (y1 <= y2)
    {
      SDL_Point tmp = {x1, 0};
      for (int y = y1; y <= y2; ++y)
      {
        tmp.y = y;
        out.push_back(tmp);
      }
    } else
    {
      SDL_Point tmp = {x1, 0};
      for (int y = y2; y <= y1; ++y)
      {
        tmp.y = y;
        out.push_back(tmp);
      }
    }
  }
  if (y1 == y2)
  {
    if (x1 <= x2)
    {
      SDL_Point tmp = {0, y1};
      for (int x = x1; x <= x2; ++x)
      {
        tmp.x = x;
        out.push_back(tmp);
      }
    } else
    {
      SDL_Point tmp = {0, y1};
      for (int x = x2; x <= x1; ++x)
      {
        tmp.x = x;
        out.push_back(tmp);
      }
    }
  }
  if (dx > dy)
  {
    if (x1 < x2)
    {
      const bool is_positive = y1 < y2;
      int diff = (2 * dy) - dx;
      int y = y1;
      SDL_Point tmp;
      for (int x = x1; x <= x2; ++x)
      {
        tmp.x = x;
        tmp.y = y;
        out.push_back(tmp);
        if (diff > 0)
        {
          if (is_positive)
          {
            ++y;
          } else
          {
            --y;
          }
          diff += 2 * (dy - dx);
        } else
        {
          diff += 2 * dy;
        }
      }
    } else
    {
      const bool is_positive = y1 > y2;
      int diff = (2 * dy) - dx;
      int y = y2;
      SDL_Point tmp;
      for (int x = x2; x <= x1; ++x)
      {
        tmp.x = x;
        tmp.y = y;
        out.push_back(tmp);
        if (diff > 0)
        {
          if (is_positive)
          {
            ++y;
          } else
          {
            --y;
          }
          diff += 2 * (dy - dx);
        } else
        {
          diff += 2 * dy;
        }
      }
    }
  } else
  {
    if (y1 < y2)
    {
      const bool is_positive = x1 < x2;
      int diff = (2 * dx) - dy;
      int x = x1;
      SDL_Point tmp;
      for (int y = y1; y <= y2; ++y)
      {
        tmp.x = x;
        tmp.y = y;
        out.push_back(tmp);
        if (diff > 0)
        {
          if (is_positive)
          {
            ++x;
          } else
          {
            --x;
          }
          diff += 2 * (dx - dy);
        } else
        {
          diff += 2 * dx;
        }
      }
    } else
    {
      const bool is_positive = x1 > x2;
      int diff = (2 * dx) - dy;
      int x = x2;
      SDL_Point tmp;
      for (int y = y2; y <= y1; ++y)
      {
        tmp.x = x;
        tmp.y = y;
        out.push_back(tmp);
        if (diff > 0)
        {
          if (is_positive)
          {
            ++x;
          } else
          {
            --x;
          }
          diff += 2 * (dx - dy);
        } else
        {
          diff += 2 * dx;
        }
      }
    }
  }
  return out;
}

inline void SDLR_RenderDrawPoint(SDL_Renderer* renderer, const int x, const int y, const int size)
{
  if (size == 1)
  {
    if (SDL_RenderDrawPoint(renderer, x, y) != 0)
    {
      const std::string msg = SDL_GetError();
      Rcpp::stop("failed to draw point.\n" + msg + "\n");
    }
  } else
  {
    const int r = size / 2;
    SDLR_RenderFillCirle(renderer, x, y, r);
  }
}


inline void SDLR_RenderDrawLine(SDL_Renderer* renderer, const int x1, const int y1, const int x2, const int y2, const int size)
{
  const int r = size / 2;
  int w, h;
  if (SDL_GetRendererOutputSize(renderer, &w, &h) != 0)
  {
    const std::string msg = SDL_GetError();
    Rcpp::stop("failed to get renderer output size.\n" + msg + "\n");
  }
  if (x1 == x2)
  {
    if (y1 != y2)
    {
      SDL_Rect tmprect;
      tmprect.x = x1 >= r ? x1 - r : 0;
      tmprect.w = x1 + r < w ? size : w - tmprect.x;
      tmprect.y = y1 < y2 ? y1 : y2;
      tmprect.h = std::abs(y2 - y1);
      if (SDL_RenderFillRect(renderer, &tmprect) != 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to draw line.\n" + msg + "\n");
      }
    } else
    {
      SDLR_RenderDrawPoint(renderer, x1, y1, size);
    }
  } else if (y1 == y2)
  {
    SDL_Rect tmprect;
    tmprect.x = x1 < x2 ? x1 : x2;
    tmprect.w = std::abs(x2 - x1);
    tmprect.y = y1 - r >= 0 ? y1 - r : 0;
    tmprect.h = y1 + r < w ? size : w - tmprect.y;
    if (SDL_RenderFillRect(renderer, &tmprect) != 0)
    {
      const std::string msg = SDL_GetError();
      Rcpp::stop("failed to draw line.\n" + msg + "\n");
    }
  } else
  {
    const double angle_perp = std::atan(static_cast<double>((x1 - x2)) / (y2 - y1));
    const int dx = static_cast<int>(std::round(r * std::cos(angle_perp)));
    const int dy = static_cast<int>(std::round(r * std::sin(angle_perp)));
    std::vector<SDL_Point> pts1(SDLR_GetLinePoints(x1 + dx, y1 + dy, x2 + dx, y2 + dy));
    std::vector<SDL_Point> pts2(SDLR_GetLinePoints(x1 - dx, y1 - dy, x2 - dx, y2 - dy));
    std::vector<SDL_Point> pts3(SDLR_GetLinePoints(x1 + dx, y1 + dy, x1 - dx, y1 - dy));
    std::vector<SDL_Point> pts4(SDLR_GetLinePoints(x2 + dx, y2 + dy, x2 - dx, y2 - dy));
    std::sort(pts1.begin(), pts1.end(), [](const SDL_Point& a, const SDL_Point& b)
    {
      return a.x < b.x;
    });
    std::sort(pts2.begin(), pts2.end(), [](const SDL_Point& a, const SDL_Point& b)
    {
      return a.x < b.x;
    });
    std::sort(pts3.begin(), pts3.end(), [](const SDL_Point& a, const SDL_Point& b)
    {
      return a.x < b.x;
    });
    std::sort(pts4.begin(), pts4.end(), [](const SDL_Point& a, const SDL_Point& b)
    {
      return a.x < b.x;
    });
    int minx, maxx;
    if (dx > 0)
    {
      if (x1 < x2)
      {
        minx = x1 - dx;
        maxx = x2 + dx;
      } else
      {
        minx = x2 - dx;
        maxx = x1 + dx;
      }
    } else
    {
      if (x1 < x2)
      {
        minx = x1 + dx;
        maxx = x2 - dx;
      } else
      {
        minx = x2 + dx;
        maxx = x1 - dx;
      }
    }
    std::vector< std::pair<int, int> > vecypair(maxx - minx + 1, std::make_pair(h - 1, 0)); // first is min, second is max.
    for (size_t i = 0; i < pts1.size(); ++i)
    {
      const int idx = pts1[i].x - minx;
      if (pts1[i].y < vecypair[idx].first)
      {
        vecypair[idx].first = pts1[i].y;
      }
      if (pts1[i].y > vecypair[idx].second)
      {
        vecypair[idx].second = pts1[i].y;
      }
    }
    for (size_t i = 0; i < pts2.size(); ++i)
    {
      const int idx = pts2[i].x - minx;
      if (pts2[i].y < vecypair[idx].first)
      {
        vecypair[idx].first = pts2[i].y;
      }
      if (pts2[i].y > vecypair[idx].second)
      {
        vecypair[idx].second = pts2[i].y;
      }
    }
    for (size_t i = 0; i < pts3.size(); ++i)
    {
      const int idx = pts3[i].x - minx;
      if (pts3[i].y < vecypair[idx].first)
      {
        vecypair[idx].first = pts3[i].y;
      }
      if (pts3[i].y > vecypair[idx].second)
      {
        vecypair[idx].second = pts3[i].y;
      }
    }
    for (size_t i = 0; i < pts4.size(); ++i)
    {
      const int idx = pts4[i].x - minx;
      if (pts4[i].y < vecypair[idx].first)
      {
        vecypair[idx].first = pts4[i].y;
      }
      if (pts4[i].y > vecypair[idx].second)
      {
        vecypair[idx].second = pts4[i].y;
      }
    }
    for (size_t i = 0; i < vecypair.size(); ++i)
    {
      int xpts = minx + i;
      if (xpts >= 0 && xpts < w)
      {
        for (int ypts = vecypair[i].first; ypts <= vecypair[i].second; ++ypts)
        {
          if (ypts >= 0 && ypts < h)
          {
            if (SDL_RenderDrawPoint(renderer, xpts, ypts) != 0)
            {
              const std::string msg = SDL_GetError();
             Rcpp::stop("failed to draw point.\n" + msg + "\n");
            }
          }
        }
      }
    }
  }
}


inline void SDLR_RenderDrawRect(SDL_Renderer* renderer, const SDL_Rect* rect, int size)
{
  if (size <= 1)
  {
    if (SDL_RenderDrawRect(renderer, rect) != 0)
    {
      const std::string msg = SDL_GetError();
      Rcpp::stop("failed to draw line.\n" + msg + "\n");
    }
    return;
  }
  const int r = size / 2;
  if (size % 2 == 0)
  {
    ++size;
  }
  int w_renderer, h_renderer;
  if (SDL_GetRendererOutputSize(renderer, &w_renderer, &h_renderer) != 0)
  {
    const std::string msg = SDL_GetError();
    Rcpp::stop("failed to get renderer output size.\n" + msg + "\n");
  }
  SDL_Rect tmprect;
  tmprect.x = rect->x - r >= 0 ? rect->x - r : 0; // (x-r, y-r), (x+r, y+h+r)
  tmprect.y = rect->y - r >= 0 ? rect->y - r : 0;
  tmprect.w = rect->x + r < w_renderer ? size : w_renderer - tmprect.x;
  tmprect.h = rect->y + rect->h + r < h_renderer ? rect->h + size : h_renderer - tmprect.y;
  if (SDL_RenderFillRect(renderer, &tmprect) != 0)
  {
    const std::string msg = SDL_GetError();
    Rcpp::stop("failed to draw line.\n" + msg + "\n");
  }
  tmprect.x = rect->x + r < w_renderer ? rect->x + r : w_renderer - 1; // (x+r, y-r), (x+w-r, y+r)
  tmprect.w = rect->x + rect->w - r >= 0 ? rect->x + rect->w - r < w_renderer ? rect->w - size + 1 : w_renderer - tmprect.x : 0;
  tmprect.h = rect->y + r < h_renderer ? size : h_renderer - tmprect.y;
  if (SDL_RenderFillRect(renderer, &tmprect) != 0)
  {
    const std::string msg = SDL_GetError();
    Rcpp::stop("failed to draw line.\n" + msg + "\n");
  }
  tmprect.y = rect->y + rect->h - r >= 0 ? rect->y + rect->h - r < h_renderer ? rect->y + rect->h - r : h_renderer - 1 : 0; // (x+r, y+h-r), (x+w-r, y+h+r)
  tmprect.w = rect->x + rect->w - r >= 0 ? rect->x + rect->w - r < w_renderer ? rect->w - size + 1 : w_renderer - tmprect.x : 0;
  tmprect.h = rect->y + rect->h + r < h_renderer ? size : h_renderer - tmprect.y;
  if (SDL_RenderFillRect(renderer, &tmprect) != 0)
  {
    const std::string msg = SDL_GetError();
    Rcpp::stop("failed to draw line.\n" + msg + "\n");
  }
  tmprect.x = rect->x + rect->w - r >= 0 ? rect->x + rect->w - r < w_renderer ? rect->x + rect->w - r : w_renderer - 1 : 0; // (x+w-r, y-r), (x+w+r,y+h+r)
  tmprect.y = rect->y - r >= 0 ? rect->y - r : 0;
  tmprect.w = rect->x + rect->w + r < w_renderer ? size : w_renderer - tmprect.x;
  tmprect.h = rect->y + rect->h + r < h_renderer ? rect->h + size : h_renderer - tmprect.y;
  if (SDL_RenderFillRect(renderer, &tmprect) != 0)
  {
    const std::string msg = SDL_GetError();
    Rcpp::stop("failed to draw line.\n" + msg + "\n");
  }
}

inline void SDLR_Wait(SDL_Window* window, SDL_Event event, const int max_count = -1, const Uint32 wait_time = 1)
{
  int window_x;
  int window_y;
  int mouse_x = 0;
  int mouse_y = 0;
  bool dragged = false;
  Uint32 prev_time = SDL_GetTicks();
  int count = 0;
  while(true)
  {
    if (SDL_PollEvent(&event) == 0)
    {
      if (max_count >= 0)
      {
        ++count;
        if (count >= max_count)
        {
          break;
        }
        SDL_Delay(1);
      }
      continue;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
      dragged = true;
      SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
    } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
    {
      dragged = false;
    } else if(event.type == SDL_QUIT)
    {
      break;
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q)
    {
      break;
    }
    if (dragged && SDL_TICKS_PASSED(SDL_GetTicks(), prev_time + wait_time))
    {
      prev_time = SDL_GetTicks();
      int tmp_x, tmp_y;
      SDL_GetGlobalMouseState(&tmp_x, &tmp_y);
      if (mouse_x != tmp_x || mouse_y != tmp_y)
      {
        SDL_GetWindowPosition(window, &window_x, &window_y);
        SDL_SetWindowPosition(window, window_x + tmp_x - mouse_x, window_y + tmp_y - mouse_y);
        SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
      }
    }
  }
}

class SDLAUDIO
{
  public:
    SDL_AudioDeviceID devid;
    SDL_AudioSpec spec;
    Uint8* buf;
    Uint32 len;
    
    SDLAUDIO() = delete;
    
    SDLAUDIO(const std::string& file)
    {
      if (SDL_Init(SDL_INIT_AUDIO) != 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to initialize SDL AUDIO.\n" + msg + "\n");
      }
      if (SDL_LoadWAV(file.c_str(), &spec, &buf, &len) == NULL)
      {
        Rcpp::stop("failed to load " + file + ".\n");
      }
      devid = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
      if (!devid)
      {
        bool is_failed = true;
        int n = SDL_GetNumAudioDevices(0);
        for (int i = 0; i < n; ++i)
        {
          devid = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(i, 0), 0, &spec, NULL, 0);
          if (devid)
          {
            is_failed = false;
            break;
          }
        }
        if (is_failed)
        {
          const std::string msg(SDL_GetError());
          Rcpp::stop("failed to open audio device.\n" + msg + "\n");
        }
      }
    }
    
    SDLAUDIO(const std::string& file, const int idx_dev)
    {
      if (SDL_Init(SDL_INIT_AUDIO) != 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to initialize SDL AUDIO.\n" + msg + "\n");
      }
      if (SDL_LoadWAV(file.c_str(), &spec, &buf, &len) == NULL)
      {
        Rcpp::stop("failed to load " + file + ".\n");
      }
      devid = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(idx_dev, 0), 0, &spec, NULL, 0);
      if (!devid)
      {
        const std::string msg(SDL_GetError());
        Rcpp::stop("failed to open audio device.\n" + msg + "\n");
      }
    }
    
    SDLAUDIO(const std::string& file, const std::string& devname)
    {
      if (SDL_Init(SDL_INIT_AUDIO) != 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to initialize SDL AUDIO.\n" + msg + "\n");
      }
      if (SDL_LoadWAV(file.c_str(), &spec, &buf, &len) == NULL)
      {
        Rcpp::stop("failed to load " + file + ".\n");
      }
      devid = SDL_OpenAudioDevice(devname.c_str(), 0, &spec, NULL, 0);
      if (!devid)
      {
        const std::string msg(SDL_GetError());
        Rcpp::stop("failed to open audio device.\n" + msg + "\n");
      }
    }
    
    ~SDLAUDIO()
    {
      SDL_CloseAudioDevice(devid);
      SDL_FreeWAV(buf);
      SDL_Quit();
    }
    
    void clear()
    {
      SDL_ClearQueuedAudio(devid);
    }
    
    Rcpp::StringVector getstatus()
    {
      Rcpp::StringVector out(1);
      SDL_AudioStatus status = SDL_GetAudioDeviceStatus(devid);
      if (status == SDL_AUDIO_PLAYING)
      {
        out[0] = "playing";
      } else if (status == SDL_AUDIO_PAUSED)
      {
        out[0] = "paused";
      } else if (status == SDL_AUDIO_STOPPED)
      {
        out[0] = "stopped";
      }
      return out;
    }
    
    void pause()
    {
      SDL_PauseAudioDevice(devid, 1);
    }
    
    void unpause()
    {
      SDL_PauseAudioDevice(devid, 0);
    }
    
    void queue()
    {
      if (SDL_QueueAudio(devid, buf, len) != 0)
      {
        const std::string msg(SDL_GetError());
        Rcpp::stop("failed to queue.\n");
      }
    }
};


inline SDL_Surface* SDLR_CreateRGBSurfaceWithFormat(const int width, const int height)
{
  SDL_Surface* out = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
  if (out == NULL) {
      Rcpp::stop("failed to create surface.");
  }
  return out;
}

inline SDL_Surface* convertVector2SurfaceRGBA(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& chorder)
{
  const int n_pixels = width * height;
  SDL_PixelFormat* pixfmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
  if (pixfmt == NULL)
  {
    const std::string msg = SDL_GetError();
    Rcpp::stop("failed to allocate format.\n" + msg + "\n");
  }
  if (vec.length() != n_pixels * 4)
  {
    Rcpp::stop("length of vec is inappropriate.");
  }
  SDL_Surface* out = SDLR_CreateRGBSurfaceWithFormat(width, height);
  Uint32* pixels = (Uint32*)out->pixels;
  if (chorder == "cxy")
  {
    for (int i = 0; i < n_pixels; ++i)
    {
      pixels[i] = SDL_MapRGBA(pixfmt, static_cast<Uint8>(vec[4 * i]), static_cast<Uint8>(vec[4 * i + 1]), static_cast<Uint8>(vec[4 * i + 2]),static_cast<Uint8>(vec[4 * i + 3]));
    }
  } else if (chorder == "xyc")
  {
    for (int i = 0; i < n_pixels; ++i)
    {
      pixels[i] = SDL_MapRGBA(pixfmt, static_cast<Uint8>(vec[i]), static_cast<Uint8>(vec[i + n_pixels]), static_cast<Uint8>(vec[i + 2 * n_pixels]),static_cast<Uint8>(vec[i + 3 * n_pixels]));
    }
  } else
  {
    Rcpp::stop("chorder is inappropriate.");
  }
  SDL_FreeFormat(pixfmt);
  return out;
}

inline SDL_Surface* convertVector2SurfaceRGBA(const Rcpp::RawVector& vec, const int width, const int height, const std::string& chorder)
{
  const int n_pixels = width * height;
  SDL_PixelFormat* pixfmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
  if (pixfmt == NULL)
  {
    const std::string msg = SDL_GetError();
    Rcpp::stop("failed to allocate format.\n" + msg + "\n");
  }
  if (vec.length() != n_pixels * 4)
  {
    Rcpp::stop("length of vec is inappropriate.");
  }
  SDL_Surface* out = SDLR_CreateRGBSurfaceWithFormat(width, height);
  Uint32* pixels = (Uint32*)out->pixels;
  if (chorder == "cxy")
  {
    for (int i = 0; i < n_pixels; ++i)
    {
      pixels[i] = SDL_MapRGBA(pixfmt, static_cast<Uint8>(vec[4 * i]), static_cast<Uint8>(vec[4 * i + 1]), static_cast<Uint8>(vec[4 * i + 2]),static_cast<Uint8>(vec[4 * i + 3]));
    }
  } else if (chorder == "xyc")
  {
    for (int i = 0; i < n_pixels; ++i)
    {
      pixels[i] = SDL_MapRGBA(pixfmt, static_cast<Uint8>(vec[i]), static_cast<Uint8>(vec[i + n_pixels]), static_cast<Uint8>(vec[i + 2 * n_pixels]),static_cast<Uint8>(vec[i + 3 * n_pixels]));
    }
  } else
  {
    Rcpp::stop("chorder is inappropriate.");
  }
  SDL_FreeFormat(pixfmt);
  return out;
}


inline SDL_Surface* convertVector2SurfaceRGB(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& chorder)
{
  const int n_pixels = width * height;
  SDL_PixelFormat* pixfmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
  if (pixfmt == NULL)
  {
    const std::string msg = SDL_GetError();
    Rcpp::stop("failed to allocate format.\n" + msg + "\n");
  }
  if (vec.length() != n_pixels * 3)
  {
    Rcpp::stop("length of vec is inappropriate.");
  }
  SDL_Surface* out = SDLR_CreateRGBSurfaceWithFormat(width, height);
  Uint32* pixels = (Uint32*)out->pixels;
  if (chorder == "cxy")
  {
    for (int i = 0; i < n_pixels; ++i)
    {
      pixels[i] = SDL_MapRGB(pixfmt, static_cast<Uint8>(vec[3 * i]), static_cast<Uint8>(vec[3 * i + 1]), static_cast<Uint8>(vec[3 * i + 2]));
    }
  } else if (chorder == "xyc")
  {
    for (int i = 0; i < n_pixels; ++i)
    {
      pixels[i] = SDL_MapRGB(pixfmt, static_cast<Uint8>(vec[i]), static_cast<Uint8>(vec[i + n_pixels]), static_cast<Uint8>(vec[i + 2 * n_pixels]));
    }
  } else
  {
    Rcpp::stop("chorder is inappropriate.");
  }
  SDL_FreeFormat(pixfmt);
  return out;
}

inline SDL_Surface* convertVector2SurfaceRGB(const Rcpp::RawVector& vec, const int width, const int height, const std::string& chorder)
{
  const int n_pixels = width * height;
  SDL_PixelFormat* pixfmt =  SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
  if (pixfmt == NULL)
  {
    const std::string msg = SDL_GetError();
    Rcpp::stop("failed to allocate format.\n" + msg + "\n");
  }
  if (vec.length() != n_pixels * 3)
  {
    Rcpp::stop("length of vec is inappropriate.");
  }
  SDL_Surface* out = SDLR_CreateRGBSurfaceWithFormat(width, height);
  Uint32* pixels = (Uint32*)out->pixels;
  if (chorder == "cxy")
  {
    for (int i = 0; i < n_pixels; ++i)
    {
      pixels[i] = SDL_MapRGB(pixfmt, static_cast<Uint8>(vec[3 * i]), static_cast<Uint8>(vec[3 * i + 1]), static_cast<Uint8>(vec[3 * i + 2]));
    }
  } else if (chorder == "xyc")
  {
    for (int i = 0; i < n_pixels; ++i)
    {
      pixels[i] = SDL_MapRGB(pixfmt, static_cast<Uint8>(vec[i]), static_cast<Uint8>(vec[i + n_pixels]), static_cast<Uint8>(vec[i + 2 * n_pixels]));
    }
  } else
  {
    Rcpp::stop("chorder is inappropriate.");
  }
  SDL_FreeFormat(pixfmt);
  return out;
}

inline SDL_Surface* convertVector2SurfaceGrey(const Rcpp::NumericVector& vec, const int width, const int height)
{
  const int n_pixels = width * height;
  SDL_PixelFormat* pixfmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
  if (pixfmt == NULL)
  {
    const std::string msg = SDL_GetError();
    Rcpp::stop("failed to allocate format.\n" + msg + "\n");
  }
  if (vec.length() != n_pixels)
  {
    Rcpp::stop("length of vec is inappropriate.");
  }
  SDL_Surface* out = SDLR_CreateRGBSurfaceWithFormat(width, height);
  Uint32* pixels = (Uint32*)out->pixels;
  for (int i = 0; i < n_pixels; ++i)
  {
    pixels[i] = SDL_MapRGB(pixfmt, static_cast<Uint8>(vec[i]), static_cast<Uint8>(vec[i]), static_cast<Uint8>(vec[i]));
  }
  SDL_FreeFormat(pixfmt);
  return out;
}

inline SDL_Surface* convertVector2SurfaceGrey(const Rcpp::RawVector& vec, const int width, const int height)
{
  const int n_pixels = width * height;
  SDL_PixelFormat* pixfmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
  if (pixfmt == NULL)
  {
    const std::string msg = SDL_GetError();
    Rcpp::stop("failed to allocate format.\n" + msg + "\n");
  }
  if (vec.length() != n_pixels)
  {
    Rcpp::stop("length of vec is inappropriate.");
  }
  SDL_Surface* out = SDLR_CreateRGBSurfaceWithFormat(width, height);
  Uint32* pixels = (Uint32*)out->pixels;
  for (int i = 0; i < n_pixels; ++i)
  {
    pixels[i] = SDL_MapRGB(pixfmt, static_cast<Uint8>(vec[i]), static_cast<Uint8>(vec[i]), static_cast<Uint8>(vec[i]));
  }
  SDL_FreeFormat(pixfmt);
  return out;
}

class SDLIMAGE
{
  public:
    SDL_Surface* surface;
    
    SDLIMAGE() = delete;
    
    SDLIMAGE(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder)
    {
      if (SDL_Init(SDL_INIT_VIDEO) < 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("Failed to initialize SDL.\n" + msg + "\n");
      }
      if (colfmt == "RGB")
      {
        surface = convertVector2SurfaceRGB(vec, width, height, chorder);
      } else if (colfmt == "RGBA")
      {
        surface = convertVector2SurfaceRGBA(vec, width, height, chorder);
      } else if (colfmt == "greyscale")
      {
        surface = convertVector2SurfaceGrey(vec, width, height);
      } else
      {
        Rcpp::stop("colfmt must be \"RGB\", \"RGBA\", or \"greyscale\".");
      }
    }
    
    SDLIMAGE(const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder)
    {
      if (SDL_Init(SDL_INIT_VIDEO) < 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("Failed to initialize SDL.\n" + msg + "\n");
      }
      if (colfmt == "RGB")
      {
        surface = convertVector2SurfaceRGB(vec, width, height, chorder);
      } else if (colfmt == "RGBA")
      {
        surface = convertVector2SurfaceRGBA(vec, width, height, chorder);
      } else if (colfmt == "greyscale")
      {
        surface = convertVector2SurfaceGrey(vec, width, height);
      } else
      {
        Rcpp::stop("colfmt must be \"RGB\", \"RGBA\", or \"greyscale\".");
      }
    }
    
     ~SDLIMAGE()
    {
      SDL_FreeSurface(surface);
    }
    
    SDL_Texture* convert2texture(SDL_Renderer* renderer)
    {
      SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
      if (texture == NULL)
      {
        Rcpp::stop("Failed to create texture.");
      }
      return texture;
    }
    
    void replace(const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder)
    {
      if (surface->w != width || surface->h != height)
      {
        Rcpp::stop("width or height of input image is inappropriate.");
      }
      if (colfmt == "RGB")
      {
        surface = convertVector2SurfaceRGB(vec, width, height, chorder);
      } else if (colfmt == "RGBA")
      {
        surface = convertVector2SurfaceRGBA(vec, width, height, chorder);
      } else if (colfmt == "greyscale")
      {
        surface = convertVector2SurfaceGrey(vec, width, height);
      } else
      {
        Rcpp::stop("colfmt must be \"RGB\", \"RGBA\", or \"greyscale\".");
      }
    }
    
    void replace(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder)
    {
      if (surface->w != width || surface->h != height)
      {
        Rcpp::stop("width or height of input image is inappropriate.");
      }
      if (colfmt == "RGB")
      {
        surface = convertVector2SurfaceRGB(vec, width, height, chorder);
      } else if (colfmt == "RGBA")
      {
        surface = convertVector2SurfaceRGBA(vec, width, height, chorder);
      } else if (colfmt == "greyscale")
      {
        surface = convertVector2SurfaceGrey(vec, width, height);
      } else
      {
        Rcpp::stop("colfmt must be \"RGB\", \"RGBA\", or \"greyscale\".");
      }
    }
};

class SDLRIMAGE
{
  public:
    SDLIMAGE* image;
    SDL_Event event;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Window* window;
    
    SDLRIMAGE(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder, const std::string& title, const int x, const int y, const Uint32 window_flags, const Uint32 renderer_flags, const int renderer_idx)
    {
      image = new SDLIMAGE(vec, width, height, colfmt, chorder);
      if (width >= SDLR_MINIMAL_WIDTH)
      {
        window = SDL_CreateWindow(title.c_str(), x, y, width, height, window_flags);
      } else
      {
        window = SDL_CreateWindow(title.c_str(), x, y, SDLR_MINIMAL_WIDTH, height, window_flags);
      }
      if (window == NULL)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to create window.\n" + msg + "\n");
      }
      renderer = SDL_CreateRenderer(window, renderer_idx, renderer_flags);
      if (renderer == NULL)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to create renderer.\n" + msg + "\n");
      }
      if (width >= SDLR_MINIMAL_WIDTH)
      {
        texture = image->convert2texture(renderer);
      } else
      {
        SDL_Surface* tmp = SDLR_CreateRGBSurfaceWithFormat(SDLR_MINIMAL_WIDTH, height);
        if (SDL_BlitSurface(image->surface, NULL, tmp, NULL) != 0)
        {
          const std::string msg = SDL_GetError();
          Rcpp::stop("failed to blit surface.\n" + msg + "\n");
        }
        texture = SDL_CreateTextureFromSurface(renderer, tmp);
        if (texture == NULL)
        {
          Rcpp::stop("Failed to create texture.");
        }
      }
      if (SDL_RenderClear(renderer) != 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to clear renderer.\n" + msg + "\n");
      }
      if (SDL_RenderCopy(renderer, texture, NULL, NULL) != 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to copy texture to renderer.\n" + msg + "\n");
      }
      SDL_RenderPresent(renderer);
    }
    
    SDLRIMAGE(const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder, const std::string& title, const int x, const int y, const Uint32 window_flags, const Uint32 renderer_flags, const int renderer_idx)
    {
      image = new SDLIMAGE(vec, width, height, colfmt, chorder);
      if (width >= SDLR_MINIMAL_WIDTH)
      {
        window = SDL_CreateWindow(title.c_str(), x, y, width, height, window_flags);
      } else
      {
        window = SDL_CreateWindow(title.c_str(), x, y, SDLR_MINIMAL_WIDTH, height, window_flags);
      }
      if (window == NULL)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to create window.\n" + msg + "\n");
      }
      renderer = SDL_CreateRenderer(window, renderer_idx, renderer_flags);
      if (renderer == NULL)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to create renderer.\n" + msg + "\n");
      }
      if (width >= SDLR_MINIMAL_WIDTH)
      {
        texture = image->convert2texture(renderer);
      } else
      {
        SDL_Surface* tmp = SDLR_CreateRGBSurfaceWithFormat(SDLR_MINIMAL_WIDTH, height);
        if (SDL_BlitSurface(image->surface, NULL, tmp, NULL) != 0)
        {
          const std::string msg = SDL_GetError();
          Rcpp::stop("failed to blit surface.\n" + msg + "\n");
        }
        texture = SDL_CreateTextureFromSurface(renderer, tmp);
        if (texture == NULL)
        {
          Rcpp::stop("Failed to create texture.");
        }
      }
      if (SDL_RenderClear(renderer) != 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to clear renderer.\n" + msg + "\n");
      }
      if (SDL_RenderCopy(renderer, texture, NULL, NULL) != 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to copy texture to renderer.\n" + msg + "\n");
      }
      SDL_RenderPresent(renderer);
    }
    
    ~SDLRIMAGE()
    {
      delete image;
      SDL_DestroyTexture(texture);
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
    }
    
    void replace(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder)
    {
      image->replace(vec, width, height, colfmt, chorder);
      if (width >= SDLR_MINIMAL_WIDTH)
      {
        texture = image->convert2texture(renderer);
      } else
      {
        SDL_Surface* tmp = SDLR_CreateRGBSurfaceWithFormat(SDLR_MINIMAL_WIDTH, height);
        if (SDL_BlitSurface(image->surface, NULL, tmp, NULL) != 0)
        {
          const std::string msg = SDL_GetError();
          Rcpp::stop("failed to blit surface.\n" + msg + "\n");
        }
        texture = SDL_CreateTextureFromSurface(renderer, tmp);
        if (texture == NULL)
        {
          Rcpp::stop("Failed to create texture.");
        }
      }
      if (SDL_RenderClear(renderer) != 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to clear renderer.\n" + msg + "\n");
      }
      if (SDL_RenderCopy(renderer, texture, NULL, NULL) != 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to copy texture to renderer.\n" + msg + "\n");
      }
      SDL_RenderPresent(renderer);
    }
    
    void replace(const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder)
    {
      image->replace(vec, width, height, colfmt, chorder);
      if (width >= SDLR_MINIMAL_WIDTH)
      {
        texture = image->convert2texture(renderer);
      } else
      {
        SDL_Surface* tmp = SDLR_CreateRGBSurfaceWithFormat(SDLR_MINIMAL_WIDTH, height);
        if (SDL_BlitSurface(image->surface, NULL, tmp, NULL) != 0)
        {
          const std::string msg = SDL_GetError();
          Rcpp::stop("failed to blit surface.\n" + msg + "\n");
        }
        texture = SDL_CreateTextureFromSurface(renderer, tmp);
        if (texture == NULL)
        {
          Rcpp::stop("Failed to create texture.");
        }
      }
      if (SDL_RenderClear(renderer) != 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to clear renderer.\n" + msg + "\n");
      }
      if (SDL_RenderCopy(renderer, texture, NULL, NULL) != 0)
      {
        const std::string msg = SDL_GetError();
        Rcpp::stop("failed to copy texture to renderer.\n" + msg + "\n");
      }
      SDL_RenderPresent(renderer);
    }
};
#endif