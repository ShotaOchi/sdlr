#include <Rcpp.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

// [[Rcpp::export]]
void hello_cpp()
{
    SDL_Window *win;
    SDL_Renderer *renderer;
    SDL_Event event;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Rcpp::stop("failed to initialize SDL.");
    }
    SDL_CreateWindowAndRenderer(320, 240, SDL_WINDOW_RESIZABLE, &win, &renderer); 
 
    while (true) {
      SDL_PollEvent(&event);
      if (event.type == SDL_QUIT) {
          break;
      }
      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
      SDL_RenderClear(renderer);
      SDL_RenderPresent(renderer);
    }
    // destroy renderer
    SDL_DestroyRenderer(renderer);
 
    // destroy window
    SDL_DestroyWindow(win);
     
    // close SDL
    SDL_Quit();
}

static Uint8* tmp_buf;
static Uint32 tmp_len;
static SDL_AudioFormat tmp_formant;
void f(void *userdata, Uint8 *stream, int len)
{
  if (tmp_len > 0)
  {
      len = static_cast<Uint32>(len) > tmp_len ? static_cast<int>(tmp_len) : len; // tmp_len is in the range of int if tmp_len is smaller than len.
      SDL_MixAudioFormat(stream, tmp_buf, tmp_formant, len, SDL_MIX_MAXVOLUME);
      tmp_buf += len;
      tmp_len -= len;
  }
}

// [[Rcpp::export]]
void hello_audio_cpp(const Rcpp::String& filenamer)
{
    const char* filename = filenamer.get_cstring();
    SDL_AudioSpec spec;
    Uint32 len;
    Uint8 *buf;
    if (SDL_Init(SDL_INIT_AUDIO) != 0)
    {
        Rcpp::stop("failed to initialize SDL AUDIO.");
    }
    if (SDL_Init(SDL_INIT_EVENTS) != 0)
    {
        Rcpp::stop("failed to initialize SDL EVENTS.");
    }
    if (SDL_LoadWAV(filename, &spec, &buf, &len) == NULL)
    {
        std::string tmp_filename(filename);
        Rcpp::stop("failed to load " + tmp_filename);
    }
    tmp_buf = buf;
    tmp_len = len;
    tmp_formant = spec.format;
    spec.callback = f;
    spec.userdata = NULL;
    
    SDL_AudioDeviceID id = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
    if (!id)
    {
        const std::string msg(SDL_GetError());
        SDL_FreeWAV(buf);
        Rcpp::stop("failed to open audio device.\n" + msg + "\n");
    }
    
    SDL_PauseAudioDevice(id, 0);
    while (tmp_len > 0)
    {
      SDL_Delay(1); 
    }
    
    SDL_CloseAudioDevice(id);
    SDL_FreeWAV(buf);
    SDL_Quit();
}
