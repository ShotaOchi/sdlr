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

inline void SDLR_wait(SDL_Window* window, SDL_Event event, Uint32 wait_time = 1)
{
  int window_x;
  int window_y;
  int mouse_x = 0;
  int mouse_y = 0;
  bool dragged = false;
  Rcpp::Function f("interactive");
  bool is_interactive = f();
  Uint32 prev_time = SDL_GetTicks();
  while(true)
  {
    if (SDL_PollEvent(&event) == 0)
    {
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
    if (dragged && SDL_GetTicks() > prev_time + wait_time)
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
    if(!is_interactive)
    {
      break;
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
        Rcpp::stop("failed to initialize SDL AUDIO.");
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
        Rcpp::stop("failed to initialize SDL AUDIO.");
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
        Rcpp::stop("failed to initialize SDL AUDIO.");
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
        Rcpp::stop("Failed to initialize SDL.");
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
        Rcpp::stop("Failed to initialize SDL.");
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