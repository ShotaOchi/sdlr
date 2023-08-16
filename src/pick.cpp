#include <sdlr.h>

Rcpp::DataFrame SDLR_pick_point(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Event event, const int n = 1, const bool mark = true, const Rcpp::NumericVector color = Rcpp::NumericVector::create(0,0,0,255), const int max_count = -1)
{
  if (mark)
  {
    if (color.size() != 4)
    {
      Rcpp::stop("color must has just 4 elements.");
    }
    if (SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(color[0]), static_cast<Uint8>(color[1]), static_cast<Uint8>(color[2]), static_cast<Uint8>(color[3])) != 0)
    {
      const std::string msg = SDL_GetError();
      Rcpp::stop("failed to set render draw coloror.\n" + msg + "\n");
    }
  }
  std::vector<int> vecx;
  vecx.reserve(n);
  std::vector<int> vecy;
  vecy.reserve(n);
  int window_x = 0;
  int window_y = 0;
  int mouse_x = 0;
  int mouse_y = 0;
  int count = 0;
  SDL_RaiseWindow(window);
  while(true)
  {
    if (SDL_PollEvent(&event) == 0)
    {
      if (max_count >= 0)
      {
        ++count;
        if (count >= max_count)
        {
          if (count == max_count)
          {
            SDL_Event tmpevent;
            tmpevent.type = SDL_MOUSEBUTTONDOWN;
            tmpevent.button.button = SDL_BUTTON_LEFT;
            if (SDL_PushEvent(&tmpevent) != 1)
            {
              const std::string msg = SDL_GetError();
              Rcpp::stop("failed to push event.\n" + msg + "\n");
            }
          } else if (count == max_count + 1)
          {
            SDL_Event tmpevent;
            tmpevent.type = SDL_KEYDOWN;
            tmpevent.key.keysym.sym = SDLK_q;
            if (SDL_PushEvent(&tmpevent) != 1)
            {
              const std::string msg = SDL_GetError();
              Rcpp::stop("failed to push event.\n" + msg + "\n");
            }
          }
        }
        SDL_Delay(1);
      }
      continue;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
      SDL_GetWindowPosition(window, &window_x, &window_y);
      SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
      vecx.push_back(mouse_x - window_x);
      vecy.push_back(mouse_y - window_y);
      if (mark)
      {
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
        for (size_t i = 0; i < vecx.size(); ++i)
        {
          if (SDL_RenderDrawPoint(renderer, vecx[i], vecy[i]) != 0)
          {
            const std::string msg = SDL_GetError();
            Rcpp::stop("failed to draw point.\n" + msg + "\n");
          }
        }
        SDL_RenderPresent(renderer);
      }
      ++count;
      if (count >= n)
      {
        break;
      }
    } else if(event.type == SDL_QUIT)
    {
      break;
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q)
    {
      break;
    }
  }
  return Rcpp::DataFrame::create(Rcpp::Named("x") = vecx, Rcpp::Named("y") = vecy);
}

Rcpp::DataFrame SDLR_pick_line(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Event event, const int n = 1, const bool mark = true, const Rcpp::NumericVector color = Rcpp::NumericVector::create(0,0,0,255), const int max_count = -1, const Uint64 wait_time = 1)
{
  if (mark)
  {
    if (color.size() != 4)
    {
      Rcpp::stop("color must has just 4 elements.");
    }
    if (SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(color[0]), static_cast<Uint8>(color[1]), static_cast<Uint8>(color[2]), static_cast<Uint8>(color[3])) != 0)
    {
      const std::string msg = SDL_GetError();
      Rcpp::stop("failed to set render draw coloror.\n" + msg + "\n");
    }
  }
  std::vector<int> vec_start_x;
  vec_start_x.reserve(n);
  std::vector<int> vec_end_x;
  vec_end_x.reserve(n);
  std::vector<int> vec_start_y;
  vec_start_y.reserve(n);
  std::vector<int> vec_end_y;
  vec_end_y.reserve(n);
  int window_x = 0;
  int window_y = 0;
  int mouse_x = 0;
  int mouse_y = 0;
  int count = 0;
  bool dragged = false;
  int start_x = 0;
  int start_y = 0;
  Uint64 prev_time = SDL_GetTicks64();
  SDL_RaiseWindow(window);
  while(true)
  {
    if (SDL_PollEvent(&event) == 0)
    {
      if (max_count >= 0)
      {
        ++count;
        if (count >= max_count)
        {
          if (count == max_count)
          {
            SDL_Event tmpevent;
            tmpevent.type = SDL_MOUSEBUTTONDOWN;
            tmpevent.button.button = SDL_BUTTON_LEFT;
            if (SDL_PushEvent(&tmpevent) != 1)
            {
              const std::string msg = SDL_GetError();
              Rcpp::stop("failed to push event.\n" + msg + "\n");
            }
          } else if (count == max_count + 1)
          {
            SDL_Event tmpevent;
            tmpevent.type = SDL_MOUSEBUTTONUP;
            tmpevent.button.button = SDL_BUTTON_LEFT;
            if (SDL_PushEvent(&tmpevent) != 1)
            {
              const std::string msg = SDL_GetError();
              Rcpp::stop("failed to push event.\n" + msg + "\n");
            }
          } else if (count == max_count + 2)
          {
            continue;
          } else if (count == max_count + 3)
          {
            SDL_Event tmpevent;
            tmpevent.type = SDL_KEYDOWN;
            tmpevent.key.keysym.sym = SDLK_q;
            if (SDL_PushEvent(&tmpevent) != 1)
            {
              const std::string msg = SDL_GetError();
              Rcpp::stop("failed to push event.\n" + msg + "\n");
            }
          }
        }
        SDL_Delay(1);
      }
      continue;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
      dragged = true;
      SDL_GetWindowPosition(window, &window_x, &window_y);
      SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
      start_x = mouse_x - window_x;
      start_y = mouse_y - window_y;
    } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
    {
      dragged = false;
      SDL_GetWindowPosition(window, &window_x, &window_y);
      SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
      vec_end_x.push_back(mouse_x - window_x);
      vec_end_y.push_back(mouse_y - window_y);
      vec_start_x.push_back(start_x);
      vec_start_y.push_back(start_y);
      if (mark)
      {
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
        for (size_t i = 0; i < vec_start_x.size(); ++i)
        {
          if (SDL_RenderDrawLine(renderer, vec_start_x[i], vec_start_y[i], vec_end_x[i], vec_end_y[i]) != 0)
          {
            const std::string msg = SDL_GetError();
            Rcpp::stop("failed to draw line.\n" + msg + "\n");
          }
        }
        SDL_RenderPresent(renderer);
      }
      ++count;
      if (count >= n)
      {
        break;
      }
    } else if(event.type == SDL_QUIT)
    {
      break;
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q)
    {
      break;
    }
    if (dragged && SDL_GetTicks64() >= prev_time + wait_time)
    {
      prev_time = SDL_GetTicks64();
      int tmp_x, tmp_y;
      SDL_GetGlobalMouseState(&tmp_x, &tmp_y);
      SDL_GetWindowPosition(window, &window_x, &window_y);
      if (mouse_x != tmp_x || mouse_y != tmp_y)
      {
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
        for (size_t i = 0; i < vec_start_x.size(); ++i)
        {
          if (SDL_RenderDrawLine(renderer, vec_start_x[i], vec_start_y[i], vec_end_x[i], vec_end_y[i]) != 0)
          {
            const std::string msg = SDL_GetError();
            Rcpp::stop("failed to draw line.\n" + msg + "\n");
          }
        }
        if (SDL_RenderDrawLine(renderer, start_x, start_y, tmp_x - window_x, tmp_y - window_y) != 0)
        {
          const std::string msg = SDL_GetError();
          Rcpp::stop("failed to draw line.\n" + msg + "\n");
        }
        SDL_RenderPresent(renderer);
        SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
      }
    }
  }
  return Rcpp::DataFrame::create(Rcpp::Named("start_x") = vec_start_x, Rcpp::Named("start_y") = vec_start_y, Rcpp::Named("end_x") = vec_end_x, Rcpp::Named("end_y") = vec_end_y);
}

Rcpp::DataFrame SDLR_pick_rect(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Event event, const int n = 1, const bool mark = true, const Rcpp::NumericVector color = Rcpp::NumericVector::create(0,0,0,255), const int max_count = -1, const Uint64 wait_time = 1)
{
  if (mark)
  {
    if (color.size() != 4)
    {
      Rcpp::stop("color must has just 4 elements.");
    }
    if (SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(color[0]), static_cast<Uint8>(color[1]), static_cast<Uint8>(color[2]), static_cast<Uint8>(color[3])) != 0)
    {
      const std::string msg = SDL_GetError();
      Rcpp::stop("failed to set render draw coloror.\n" + msg + "\n");
    }
  }
  std::vector<int> vecx;
  vecx.reserve(n);
  std::vector<int> vecy;
  vecy.reserve(n);
  std::vector<int> vecw;
  vecw.reserve(n);
  std::vector<int> vech;
  vech.reserve(n);
  int window_x = 0;
  int window_y = 0;
  int mouse_x = 0;
  int mouse_y = 0;
  int count = 0;
  bool dragged = false;
  int start_x = 0;
  int start_y = 0;
  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.w = 0;
  rect.h = 0;
  Uint64 prev_time = SDL_GetTicks64();
  SDL_RaiseWindow(window);
  while(true)
  {
    if (SDL_PollEvent(&event) == 0)
    {
      if (max_count >= 0)
      {
        ++count;
        if (count >= max_count)
        {
          if (count == max_count)
          {
            SDL_Event tmpevent;
            tmpevent.type = SDL_MOUSEBUTTONDOWN;
            tmpevent.button.button = SDL_BUTTON_LEFT;
            if (SDL_PushEvent(&tmpevent) != 1)
            {
              const std::string msg = SDL_GetError();
              Rcpp::stop("failed to push event.\n" + msg + "\n");
            }
          } else if (count == max_count + 1)
          {
            SDL_Event tmpevent;
            tmpevent.type = SDL_MOUSEBUTTONUP;
            tmpevent.button.button = SDL_BUTTON_LEFT;
            if (SDL_PushEvent(&tmpevent) != 1)
            {
              const std::string msg = SDL_GetError();
              Rcpp::stop("failed to push event.\n" + msg + "\n");
            }
          } else if (count == max_count + 2)
          {
            continue;
          } else if (count == max_count + 3)
          {
            SDL_Event tmpevent;
            tmpevent.type = SDL_KEYDOWN;
            tmpevent.key.keysym.sym = SDLK_q;
            if (SDL_PushEvent(&tmpevent) != 1)
            {
              const std::string msg = SDL_GetError();
              Rcpp::stop("failed to push event.\n" + msg + "\n");
            }
          }
        }
        SDL_Delay(1);
      }
      continue;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
      dragged = true;
      SDL_GetWindowPosition(window, &window_x, &window_y);
      SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
      start_x = mouse_x - window_x;
      start_y = mouse_y - window_y;
    } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
    {
      dragged = false;
      SDL_GetWindowPosition(window, &window_x, &window_y);
      SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
      int end_x = mouse_x - window_x;
      int end_y = mouse_y - window_y;
      if (start_x < end_x)
      {
        rect.x = start_x;
        rect.w = end_x - start_x;
      } else
      {
        rect.x = end_x;
        rect.w = start_x - end_x;
      }
      if (start_y < end_y)
      {
        rect.y = start_y;
        rect.h = end_y - start_y;
      } else
      {
        rect.y = end_y;
        rect.h = start_y - end_y;
      }
      vecx.push_back(rect.x);
      vecy.push_back(rect.y);
      vecw.push_back(rect.w);
      vech.push_back(rect.h);
      if (mark)
      {
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
        for (size_t i = 0; i < vecx.size(); ++i)
        {
          SDL_Rect tmprect;
          tmprect.x = vecx[i];
          tmprect.y = vecy[i];
          tmprect.w = vecw[i];
          tmprect.h = vech[i];
          if (SDL_RenderDrawRect(renderer, &tmprect) != 0)
          {
            const std::string msg = SDL_GetError();
            Rcpp::stop("failed to draw line.\n" + msg + "\n");
          }
        }
        SDL_RenderPresent(renderer);
      }
      ++count;
      if (count >= n)
      {
        break;
      }
    } else if(event.type == SDL_QUIT)
    {
      break;
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q)
    {
      break;
    }
    if (dragged && SDL_GetTicks64() >= prev_time + wait_time)
    {
      prev_time = SDL_GetTicks64();
      int tmp_x, tmp_y;
      SDL_GetGlobalMouseState(&tmp_x, &tmp_y);
      SDL_GetWindowPosition(window, &window_x, &window_y);
      if (mouse_x != tmp_x || mouse_y != tmp_y)
      {
        int end_x = tmp_x - window_x;
        int end_y = tmp_y - window_y;
        if (start_x < end_x)
        {
          rect.x = start_x;
          rect.w = end_x - start_x;
        } else
        {
          rect.x = end_x;
          rect.w = start_x - end_x;
        }
        if (start_y < end_y)
        {
          rect.y = start_y;
          rect.h = end_y - start_y;
        } else
        {
          rect.y = end_y;
          rect.h = start_y - end_y;
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
        for (size_t i = 0; i < vecx.size(); ++i)
        {
          SDL_Rect tmprect;
          tmprect.x = vecx[i];
          tmprect.y = vecy[i];
          tmprect.w = vecw[i];
          tmprect.h = vech[i];
          if (SDL_RenderDrawRect(renderer, &tmprect) != 0)
          {
            const std::string msg = SDL_GetError();
            Rcpp::stop("failed to draw line.\n" + msg + "\n");
          }
        }
        if (SDL_RenderDrawRect(renderer, &rect) != 0)
        {
          const std::string msg = SDL_GetError();
          Rcpp::stop("failed to draw line.\n" + msg + "\n");
        }
        SDL_RenderPresent(renderer);
        SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
      }
    }
  }
  return Rcpp::DataFrame::create(Rcpp::Named("x") = vecx, Rcpp::Named("y") = vecy, Rcpp::Named("w") = vecw, Rcpp::Named("h") = vech);
}

//[[Rcpp::export]]
Rcpp::DataFrame pick_point_raw(const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int x = 0, const int y = 0, const int renderer_idx = -1, const int n = 1, const bool mark = true, const Rcpp::NumericVector color = Rcpp::NumericVector::create(0,0,0,255), const int max_count = -1)
{
  Rcpp::DataFrame out;
  if (x == -1)
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, renderer_idx);
    out = SDLR_pick_point(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  } else if (x == -2)
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, renderer_idx);
    out = SDLR_pick_point(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  } else
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, x, y, 0, 0, renderer_idx);
    out = SDLR_pick_point(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  }
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  return out;
}

//[[Rcpp::export]]
Rcpp::DataFrame pick_point_numeric(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int x = 0, const int y = 0, const int renderer_idx = -1, const int n = 1, const bool mark = true, const Rcpp::NumericVector color = Rcpp::NumericVector::create(0,0,0,255), const int max_count = -1)
{
  Rcpp::DataFrame out;
  if (x == -1)
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, renderer_idx);
    out = SDLR_pick_point(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  } else if (x == -2)
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, renderer_idx);
    out = SDLR_pick_point(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  } else
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, x, y, 0, 0, renderer_idx);
    out = SDLR_pick_point(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  }
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  return out;
}

//[[Rcpp::export]]
Rcpp::DataFrame pick_line_raw(const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int x = 0, const int y = 0, const int renderer_idx = -1, const int n = 1, const bool mark = true, const Rcpp::NumericVector color = Rcpp::NumericVector::create(0,0,0,255), const int max_count = -1)
{
  Rcpp::DataFrame out;
  if (x == -1)
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, renderer_idx);
    out = SDLR_pick_line(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  } else if (x == -2)
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, renderer_idx);
    out = SDLR_pick_line(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  } else
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, x, y, 0, 0, renderer_idx);
    out = SDLR_pick_line(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  }
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  return out;
}

//[[Rcpp::export]]
Rcpp::DataFrame pick_line_numeric(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int x = 0, const int y = 0, const int renderer_idx = -1, const int n = 1, const bool mark = true, const Rcpp::NumericVector color = Rcpp::NumericVector::create(0,0,0,255), const int max_count = -1)
{
  Rcpp::DataFrame out;
  if (x == -1)
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, renderer_idx);
    out = SDLR_pick_line(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  } else if (x == -2)
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, renderer_idx);
    out = SDLR_pick_line(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  } else
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, x, y, 0, 0, renderer_idx);
    out = SDLR_pick_line(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  }
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  return out;
}

//[[Rcpp::export]]
Rcpp::DataFrame pick_rect_raw(const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int x = 0, const int y = 0, const int renderer_idx = -1, const int n = 1, const bool mark = true, const Rcpp::NumericVector color = Rcpp::NumericVector::create(0,0,0,255), const int max_count = -1)
{
  Rcpp::DataFrame out;
  if (x == -1)
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, renderer_idx);
    out = SDLR_pick_rect(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  } else if (x == -2)
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, renderer_idx);
    out = SDLR_pick_rect(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  } else
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, x, y, 0, 0, renderer_idx);
    out = SDLR_pick_rect(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  }
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  return out;
}

//[[Rcpp::export]]
Rcpp::DataFrame pick_rect_numeric(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int x = 0, const int y = 0, const int renderer_idx = -1, const int n = 1, const bool mark = true, const Rcpp::NumericVector color = Rcpp::NumericVector::create(0,0,0,255), const int max_count = -1)
{
  Rcpp::DataFrame out;
  if (x == -1)
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, renderer_idx);
    out = SDLR_pick_rect(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  } else if (x == -2)
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, renderer_idx);
    out = SDLR_pick_rect(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  } else
  {
    SDLRIMAGE img(vec, width, height, colfmt, chorder, title, x, y, 0, 0, renderer_idx);
    out = SDLR_pick_rect(img.window, img.renderer, img.texture, img.event, n, mark, color, max_count);
  }
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  return out;
}