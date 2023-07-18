test_that("audio",
{
  sound <- system.file("extdata", "train-whistle.wav", package = "sdlr")
  expect_no_error(getAudioDevices())
  dev <- getAudioDevices()
  skip_if(length(dev) < 1, "A list of audio devices is not available.")
  expect_no_error(SDLaudio$new(sound))
  num_dev = -1
  for (i in 0:(length(dev) - 1))
  {
    tmp <- try(SDLaudio$new(sound, i), silent = TRUE)
    if (class(tmp)[1] != "try-error")
    {
      num_dev <- i
      break;
    }
  }
  if (num_dev == -1)
  {
    skip("failed to open audio device.")
  }
  expect_no_error(SDLaudio$new(sound, num_dev))
  expect_no_error(SDLaudio$new(sound, dev[num_dev + 1]))
  expect_error(SDLaudio$new(sound, TRUE))
  expect_error(SDLaudio$new(sound, -1))
  expect_error(SDLaudio$new(sound, "i believe no device name is like this."))
  audio <- SDLaudio$new(sound)
  expect(audio$getstatus() == "playing")
  expect_no_error(audio$queue())
  expect_no_error(audio$clear())
  expect_no_error(audio$pause())
  expect(audio$getstatus() == "paused")
  expect_no_error(audio$unpause())
  expect(audio$getstatus() == "playing")
})
