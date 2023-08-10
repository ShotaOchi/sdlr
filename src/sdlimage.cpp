#include <sdlr.h>

// [[Rcpp::export]]
void displaySDLRIMAGEnumeric(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int x = 0, const int y = 0, const int renderer_idx = 0)
{
  SDLRIMAGE img(vec, width, height, colfmt, chorder, title, x, y, 0, 0, renderer_idx);
  SDLR_wait(img.window, img.event);
}

// [[Rcpp::export]]
void displaySDLRIMAGEraw(const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int x = 0, const int y = 0, const int renderer_idx = 0)
{
  SDLRIMAGE img(vec, width, height, colfmt, chorder, title, x, y, 0, 0, renderer_idx);
  SDLR_wait(img.window, img.event);
}

// [[Rcpp::export]]
void displaySDLRIMAGEnumericC(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int renderer_idx = 0)
{
  SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, renderer_idx);
  SDLR_wait(img.window, img.event);
}

// [[Rcpp::export]]
void displaySDLRIMAGErawC(const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int renderer_idx = 0)
{
  SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, renderer_idx);
  SDLR_wait(img.window, img.event);
}

// [[Rcpp::export]]
void displaySDLRIMAGEnumericU(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int renderer_idx = 0)
{
  SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, renderer_idx);
  SDLR_wait(img.window, img.event);
}

// [[Rcpp::export]]
void displaySDLRIMAGErawU(const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int renderer_idx = 0)
{
  SDLRIMAGE img(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, renderer_idx);
  SDLR_wait(img.window, img.event);
}

// [[Rcpp::export]]
Rcpp::XPtr<SDLRIMAGE> makeSDLRIMAGEnumeric(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int x = 0, const int y = 0, const int renderer_idx = 0)
{
  SDLRIMAGE* p = new SDLRIMAGE(vec, width, height, colfmt, chorder, title, x, y, 0, 0, renderer_idx);
  Rcpp::XPtr<SDLRIMAGE> ptr(p);
  return ptr;
}

// [[Rcpp::export]]
Rcpp::XPtr<SDLRIMAGE> makeSDLRIMAGEraw(const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int x = 0, const int y = 0, const int renderer_idx = 0)
{
  SDLRIMAGE* p = new SDLRIMAGE(vec, width, height, colfmt, chorder, title, x, y, 0, 0, renderer_idx);
  Rcpp::XPtr<SDLRIMAGE> ptr(p);
  return ptr;
}

// [[Rcpp::export]]
Rcpp::XPtr<SDLRIMAGE> makeSDLRIMAGEnumericC(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int renderer_idx = 0)
{
  SDLRIMAGE* p = new SDLRIMAGE(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, renderer_idx);
  Rcpp::XPtr<SDLRIMAGE> ptr(p);
  return ptr;
}

// [[Rcpp::export]]
Rcpp::XPtr<SDLRIMAGE> makeSDLRIMAGErawC(const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int renderer_idx = 0)
{
  SDLRIMAGE* p = new SDLRIMAGE(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, renderer_idx);
  Rcpp::XPtr<SDLRIMAGE> ptr(p);
  return ptr;
}

// [[Rcpp::export]]
Rcpp::XPtr<SDLRIMAGE> makeSDLRIMAGEnumericU(const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int renderer_idx = 0)
{
  SDLRIMAGE* p = new SDLRIMAGE(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, renderer_idx);
  Rcpp::XPtr<SDLRIMAGE> ptr(p);
  return ptr;
}

// [[Rcpp::export]]
Rcpp::XPtr<SDLRIMAGE> makeSDLRIMAGErawU(const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder = "cxy", const std::string& title = "", const int renderer_idx = 0)
{
  SDLRIMAGE* p = new SDLRIMAGE(vec, width, height, colfmt, chorder, title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, renderer_idx);
  Rcpp::XPtr<SDLRIMAGE> ptr(p);
  return ptr;
}

// [[Rcpp::export]]
void deleteSDLRIMAGE(Rcpp::XPtr<SDLRIMAGE> ptr)
{
  ptr.release();
}

// [[Rcpp::export]]
void replaceSDLRIMAGEraw(Rcpp::XPtr<SDLRIMAGE> ptr, const Rcpp::RawVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder)
{
  ptr->replace(vec, width, height, colfmt, chorder);
}

// [[Rcpp::export]]
void replaceSDLRIMAGEnumeric(Rcpp::XPtr<SDLRIMAGE> ptr, const Rcpp::NumericVector& vec, const int width, const int height, const std::string& colfmt, const std::string& chorder)
{
  ptr->replace(vec, width, height, colfmt, chorder);
}