#' Pick points, lines, or rectangles from image
#' 
#' Pick points, lines, or rectangles from image. note that the origin is top left.
#' @param img an array that represents image.
#' @param type "point", "line", or "rectangle".
#' @param n the number of points, lines, or rectangles to be picked.
#' @param mark whether to mark or not.
#' @param color a vector that represents a color (c(R, G, B, A)).
#' @param pos position of window. a numeric vector that represents position, "centered", or "undefined".
#' @param chorder channel order of image. "cxy" or "xyc".
#' @param title title of window.
#' @param renderer_idx the number of rendering device to be initialized.
#' @return a data.frame. note that x and y are the coordinates of upper left corner of rectangle when type is "rect".
#' @examples
#' if (interactive())
#' {
#'   pick(human)
#'   pick(human, type = "line")
#'   pick(human, type = "rect")
#' }
#' @export
pick <- function(img, type = "point", n = 1, mark = TRUE, color = c(0,0,0,255), pos = "undefined", chorder = "cxy", title = "", renderer_idx = -1)
{
  assert_image(img)
  assert_pos(pos)
  assert_name(title)
  assert_idx_renderer(renderer_idx)
  assert_type(type)
  assert_n(n)
  assert_mark(mark)
  assert_color(color)
  max_count <- getOption("sdlr_max_count")
  if (is.null(max_count))
  {
    max_count <- -1
  } else
  {
    assert_count(max_count)
  }
  dim_img <- dim(img)
  if (length(dim_img) == 2)
  {
    colfmt <- "greyscale"
  } else if (chorder == "cxy")
  {
    if (dim_img[1] == 3)
    {
      colfmt <- "RGB"
    } else if (dim_img[1] == 4)
    {
      colfmt <- "RGBA"
    } else
    {
      stop("unknown color format.")
    }
  } else
  {
    if (dim_img[3] == 3)
    {
      colfmt <- "RGB"
    } else if (dim_img[3] == 4)
    {
      colfmt <- "RGBA"
    } else
    {
      stop("unknown color format.")
    }
  }
  if (colfmt == "greyscale")
  {
    width <- dim_img[1]
    height <- dim_img[2]
  } else
  {
    if (chorder == "cxy")
    {
      width <- dim_img[2]
      height <- dim_img[3]
    } else
    {
      width <- dim_img[1]
      height <- dim_img[2]
    }
  }
  if (is.character(pos))
  {
    pos <- ifelse(pos == "undefined", c(-1,-1), c(-2,-2))
  }
  if (is.numeric(img))
  {
    if (max(img) <= 1)
    {
      img <- 255 * img
    }
  }
  if (type == "point")
  {
    if (is.numeric(img))
    {
      out <- pick_point_numeric(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, x = pos[1], y = pos[2], renderer_idx = renderer_idx, n = n, mark = mark, color = color, max_count = max_count)
    } else
    {
      out <- pick_point_raw(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, x = pos[1], y = pos[2], renderer_idx = renderer_idx, n = n, mark = mark, color = color, max_count = max_count)
    }
  } else if (type == "line")
  {
    if (is.numeric(img))
    {
      out <- pick_line_numeric(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, x = pos[1], y = pos[2], renderer_idx = renderer_idx, n = n, mark = mark, color = color, max_count = max_count)
    } else
    {
      out <- pick_line_raw(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, x = pos[1], y = pos[2], renderer_idx = renderer_idx, n = n, mark = mark, color = color, max_count = max_count)
    }
  } else
  {
    if (is.numeric(img))
    {
      out <- pick_rect_numeric(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, x = pos[1], y = pos[2], renderer_idx = renderer_idx, n = n, mark = mark, color = color, max_count = max_count)
    } else
    {
      out <- pick_rect_raw(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, x = pos[1], y = pos[2], renderer_idx = renderer_idx, n = n, mark = mark, color = color, max_count = max_count)
    }
  }
  out + 1 #R index starts from 1 but C++ index starts from 0.
}