#' R6 class SDL Audio Interaface
#' 
#' SDLaudio 
#' @export
SDLaudio <- R6Class("SDLaudio",
  public = list(
    
    #' @field extptr external pointer of SDL audio.
    extptr = NULL,
    
    #' @field path to a wav file.
    path = NULL,
    
    #' @description
    #' create a SDLaudio object.
    #' @param path path to a wav file.
    #' @param dev audio device name to use. can be missing.
    #' @return a new SDLaudio object.
    initialize = function(path, dev)
    {
      assert_wav(path)
      self$path <- path
      if (missing(dev))
      {
        self$extptr <- makeSDLAUDIO(path)
      } else
      {
        if (is.character(dev))
        {
          assert_name(dev)
          self$extptr <- makeSDLAUDIOstring(path, dev)
        } else if (is.numeric(dev))
        {
          assert_idx(dev)
          self$extptr <- makeSDLAUDIOint(path, as.integer(dev))
        } else
        {
          stop("dev must be integer, string, or missing.")
        }
      }
      self$unpause()
      invisible(self)
    },
    
    #' @description
    #' clear queued audio.
    clear = function()
    {
      clearSDLAUDIO(self$extptr)
      invisible(self)
    },
    
    #' @description
    #' get status of audio device.
    #' @return a character vector.
    getstatus = function()
    {
      getstatusSDLAUDIO(self$extptr);
    },
    
    #' @description
    #' pause audio playback.
    pause = function()
    {
      pauseSDLAUDIO(self$extptr);
      invisible(self)
    },
    
    #' @description
    #' unpause audio playback.
    unpause = function()
    {
      unpauseSDLAUDIO(self$extptr);
      invisible(self)
    },
    
    #' @description
    #' queue audio.
    queue = function()
    {
      queueSDLAUDIO(self$extptr);
      invisible(self)
    }
  )
)

#' Get List of Audio Devices
#' 
#' return a character vector containg names of audio devices.
#' note that you may successfully open the default device even if you fail to get a list of audio devices.
#' @return a character vector
#' @export
getAudioDevices <- function()
{
  getAudioDevicesCpp()
}
