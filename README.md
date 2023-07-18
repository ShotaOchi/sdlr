# sdlr

[![Build Status](https://github.com/ShotaOchi/sdlr/workflows/R-CMD-check/badge.svg)](https://github.com/ShotaOchi/sdlr/actions)
[![CRAN Version](https://www.r-pkg.org/badges/version/sdlr)](https://cran.r-project.org/package=sdlr)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![codecov](https://codecov.io/gh/ShotaOchi/sdlr/branch/main/graph/badge.svg?token=EDRYIIMXCY)](https://codecov.io/gh/ShotaOchi/sdlr)

## About
sdlr is an R package that offers R interface to video API and sound API of SDL2 library. 

## Installation
You can install sdlr from GitHub.


Run the following R code to install sdlr.
```r
# install from GitHub
devtools::install_github("ShotaOchi/sdlr")
```

## Examples

### Audio
we can use an R6 class SDLaudio to play audio.
```r
library(sdlr)
path <- system.file("extdata", "train-whistle.wav", package = "sdlr")
# load wav file
a <- SDLaudio$new(path)
# play audio
a$queue()
```

### Video
not implemented yet...

## Contribution
You can create issues for any bug report or suggestion on the [issues page](https://github.com/ShotaOchi/sdlr/issues).


You're welcome to fork this repository and send me a pull request for bug fixes or additional features.
