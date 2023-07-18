#ifndef SDLR_H
#define SDLR_H
#include <unordered_map>
#include <Rcpp.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_audio.h>

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
#endif