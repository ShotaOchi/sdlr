#include <sdlr.h>

// [[Rcpp::export]]
Rcpp::StringVector getAudioDevicesCpp()
{
  if (SDL_Init(SDL_INIT_AUDIO) != 0)
  {
    Rcpp::stop("failed to initialize SDL AUDIO.");
  }
  const int num_audiodev = SDL_GetNumAudioDevices(0);
  if (num_audiodev == -1)
  {
    Rcpp::Rcout << "failed to get list of audio devices.\n";
    Rcpp::StringVector out(0);
    SDL_Quit();
    return out;
  }
  Rcpp::StringVector out(num_audiodev);
  for (int i = 0; i < num_audiodev; ++i)
  {
    out[i] = std::string(SDL_GetAudioDeviceName(i, 0));
  }
  SDL_Quit();
  return out;
}

// [[Rcpp::export]]
Rcpp::XPtr<SDLAUDIO> makeSDLAUDIO(const std::string& file)
{
  SDLAUDIO* p = new SDLAUDIO(file);
  Rcpp::XPtr<SDLAUDIO> ptr(p);
  return ptr;
}

// [[Rcpp::export]]
Rcpp::XPtr<SDLAUDIO> makeSDLAUDIOint(const std::string& file, const int idx_dev)
{
  SDLAUDIO* p = new SDLAUDIO(file, idx_dev);
  Rcpp::XPtr<SDLAUDIO> ptr(p);
  return ptr;
}

// [[Rcpp::export]]
Rcpp::XPtr<SDLAUDIO> makeSDLAUDIOstring(const std::string& file, const std::string& devname)
{
  SDLAUDIO* p = new SDLAUDIO(file, devname);
  Rcpp::XPtr<SDLAUDIO> ptr(p);
  return ptr;
}

// [[Rcpp::export]]
void clearSDLAUDIO(Rcpp::XPtr<SDLAUDIO> ptr)
{
  ptr->clear();
}

// [[Rcpp::export]]
Rcpp::StringVector getstatusSDLAUDIO(Rcpp::XPtr<SDLAUDIO> ptr)
{
  Rcpp::StringVector out = ptr->getstatus();
  return out;
}

// [[Rcpp::export]]
void pauseSDLAUDIO(Rcpp::XPtr<SDLAUDIO> ptr)
{
  ptr->pause();
}

// [[Rcpp::export]]
void unpauseSDLAUDIO(Rcpp::XPtr<SDLAUDIO> ptr)
{
  ptr->unpause();
}

// [[Rcpp::export]]
void queueSDLAUDIO(Rcpp::XPtr<SDLAUDIO> ptr)
{
  ptr->queue();
}
