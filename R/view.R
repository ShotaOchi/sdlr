#' Display an image.
#' 
#' @param img an array that represents image.
#' @param pos position of window. a numeric vector that represents position, "centered", or "undefined".
#' @param chorder channel order of image. "cxy" or "xyc".
#' @param title title of window.
#' @param renderer_idx number of rendering drived to be initialized.
#' @export
view <- function(img, pos = "undefined", chorder = "cxy", title = "", renderer_idx = -1)
{
  assert_image(img)
  assert_pos(pos)
  assert_name(title)
  assert_idx_renderer(renderer_idx)
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
  
  if (is.numeric(img))
  {
    if (max(img) <= 1)
    {
      img <- 255 * img
    }
    if (is.numeric(pos))
    {
      displaySDLRIMAGEnumeric(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, x = pos[1], y = pos[2], renderer_idx = renderer_idx, max_count = max_count)
    } else if (pos == "undefined")
    {
      displaySDLRIMAGEnumericU(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, renderer_idx = renderer_idx, max_count = max_count)
    } else
    {
      displaySDLRIMAGEnumericC(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, renderer_idx = renderer_idx, max_count = max_count)
    }
  } else
  {
    if (is.numeric(pos))
    {
      displaySDLRIMAGEraw(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, x = pos[1], y = pos[2], renderer_idx = renderer_idx, max_count = max_count)
    } else if (pos == "undefined")
    {
      displaySDLRIMAGErawU(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, renderer_idx = renderer_idx, max_count = max_count)
    } else
    {
      displaySDLRIMAGErawC(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, renderer_idx = renderer_idx, max_count = max_count)
    }
  }
}

#$' R6 class SDL Image Interaface (SDLimage is implemented to test SDLIMAGE class)
#$' 
#$' SDLimage 
#$' @export
SDLimage <- R6Class("SDLimage",
  public = list(
    
    #$' @field extptr external pointer of SDL image.
    extptr = NULL,
    
    #$' @description
    #$' create a SDLimage object.
    #$' @param img an array that represents image.
    #$' @param pos position of window. a numeric vector that represents position, "centered", or "undefined".
    #$' @param chorder channel order of image. "cxy" or "xyc".
    #$' @param title title of window.
    #$' @param renderer_idx number of rendering drived to be initialized.
    #$' @return a new SDLimage object.
    initialize = function(img, pos = "undefined", chorder = "cxy", title = "", renderer_idx = -1)
    {
      assert_image(img)
      assert_pos(pos)
      assert_name(title)
      assert_idx(renderer_idx)
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
      
      if (is.numeric(img))
      {
        if (max(img) <= 1)
        {
          img <- 255 * img
        }
        if (is.numeric(pos))
        {
          self$extptr <- makeSDLRIMAGEnumeric(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, x = pos[1], y = pos[2], renderer_idx = renderer_idx)
        } else if (pos == "undefined")
        {
          self$extptr <- makeSDLRIMAGEnumericU(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, renderer_idx = renderer_idx)
        } else
        {
          self$extptr <- makeSDLRIMAGEnumericC(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, renderer_idx = renderer_idx)
        }
      } else
      {
        if (is.numeric(pos))
        {
          self$extptr <- makeSDLRIMAGEraw(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, x = pos[1], y = pos[2], renderer_idx = renderer_idx)
        } else if (pos == "undefined")
        {
          self$extptr <- makeSDLRIMAGErawU(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, renderer_idx = renderer_idx)
        } else
        {
          self$extptr <- makeSDLRIMAGErawC(vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder, title = title, renderer_idx = renderer_idx)
        }
      }
      invisible(self)
    },
    
    #$' @description
    #$' destroy SDLimage object.
    destroy = function()
    {
      deleteSDLRIMAGE(self$extptr)
      invisible(self)
    },
    
    #$' @description
    #$' replace SDLimage object with a new image.
    #$' @param img an array that represents image.
    #$' @param chorder channel order of image. "cxy" or "xyc".
    replace = function(img, chorder = "cxy")
    {
      assert_image(img)
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
      
      if (is.numeric(img))
      {
        if (max(img) <= 1)
        {
          img <- 255 * img
        }
        replaceSDLRIMAGEnumeric(ptr = self$extptr, vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder)
      } else
      {
        replaceSDLRIMAGEraw(ptr = self$extptr, vec = img, width = width, height = height, colfmt = colfmt, chorder = chorder)
      }
      invisible(self)
    }
  ),
  private = list(
    finalize = function()
    {
      deleteSDLRIMAGE(self$extptr)
    }
  )
)