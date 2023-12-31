assert_color <- function(color)
{
  assert_numeric(color, lower = 0, upper = 255, finite = TRUE, any.missing = FALSE, len = 4)
}

assert_count <- function(count)
{
  assert_number(count, finite = TRUE)
}

assert_idx <- function(idx)
{
  assert_number(idx, lower = 0, finite = TRUE)
}

assert_idx_renderer <- function(idx)
{
  assert_number(idx, lower = -1, finite = TRUE)
}

assert_image <- function(img)
{
  assert_array(img, any.missing = FALSE, min.d = 2, max.d = 3)
  if (!is.raw(img) && !is.numeric(img))
  {
    stop("image must be a raw or numeric.")
  }
  if (is.numeric(img))
  {
    assert_numeric(img, lower = 0, upper = 255, finite = TRUE, any.missing = FALSE)
  }
}

assert_mark <- function(mark)
{
  assert_flag(mark)
}

assert_n <- function(n)
{
  assert_number(n, lower = 1, finite = TRUE)
}

assert_name <- function(name)
{
  assert_string(name, min.chars = 0)
}

assert_pos <- function(pos)
{
  if (is.numeric(pos))
  {
    assert_numeric(pos, lower = 0, finite = TRUE, any.missing = FALSE, len = 2)
  } else
  {
    if (is.character(pos))
    {
      if (pos != "centered" && pos != "undefined")
      {
        stop(sprintf('%s must be a numeric vector that represents position, "centered", or "undefined".', deparse(substitute(pos))))
      }
    } else
    {
      stop(sprintf('%s must be a numeric vector that represents position, "centered", or "undefined".', deparse(substitute(pos))))
    }
  }
}

assert_rgba <- function(rgba)
{
  assert_numeric(rgba, lower = 0, upper = 255, finite = TRUE, any.missing = FALSE, len = 4)
}

assert_size <- function(size)
{
  assert_number(size, lower = 1, finite = TRUE)
}

assert_type <- function(type)
{
  assert_string(type, min.chars = 1, pattern = "point|line|rect")
}

assert_wav <- function(file)
{
  assert_file(file, access = "r", extension = ".wav")
}

#' R Interface to SDL_Quit
#'
#' calls SDLR_Quit function immediately.
#' @export
SDLR_Quit <- function()
{
  SDLR_QUIT()
}