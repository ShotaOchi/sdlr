#' hello
#' 
#' hello
#' @export
hello <- function()
{
  hello_cpp()
}

#' hello audio
#' 
#' hello audio
#' @param file filename of wav file
#' @export
hello_audio <- function(file)
{
  hello_audio_cpp(file)
}