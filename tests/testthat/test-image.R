test_that("image",
{

  expect(interactive == FALSE, "the r session is interactive...")

  dimhuman <- dim(human)
  dimxyc <- c(dimhuman[2:3], dimhuman[1])
  
  numhuman <- array(as.numeric(human), dim = dimhuman)
  
  xychuman <- array(as.raw(0), dim = dimxyc)
  xychuman[,,1] <- human[1,,]
  xychuman[,,2] <- human[2,,]
  xychuman[,,3] <- human[3,,]
  
  xycnumhuman <- array(0, dim = dimxyc)
  xycnumhuman[,,1] <- numhuman[1,,]
  xycnumhuman[,,2] <- numhuman[2,,]
  xycnumhuman[,,3] <- numhuman[3,,]
  
  greyhuman <- human[1,,]
  greynumhuman <- numhuman[1,,]
  
  rgbahuman <- array(as.raw(255), dim = c(4, dimhuman[2:3]))
  rgbahuman[1:3,,] <- human
  
  rgbanumhuman <- array(255, dim = c(4, dimhuman[2:3]))
  rgbanumhuman[1:3,,] <- numhuman
  
  xycrgbahuman <- array(as.raw(255), dim = c(dimhuman[2:3], 4))
  xycrgbahuman[,,1:3] <- xychuman
  
  xycrgbanumhuman <- array(255, dim = c(dimhuman[2:3], 4))
  xycrgbanumhuman[,,1:3] <- xycnumhuman
  
  expect_no_error(view(human))
  expect_no_error(view(numhuman))
  expect_no_error(view(xychuman, chorder = "xyc"))
  expect_no_error(view(xycnumhuman, chorder = "xyc"))
  expect_no_error(view(greyhuman))
  expect_no_error(view(greynumhuman))
  expect_no_error(view(rgbahuman))
  expect_no_error(view(rgbanumhuman))
  expect_no_error(view(xycrgbahuman, chorder = "xyc"))
  expect_no_error(view(xycrgbanumhuman, chorder = "xyc"))
  
  
  bighuman <- array(as.raw(0), dim = c(dimhuman[1], dimhuman[2:3] * 3))
  bighuman[,1:100, 1:100] <- human
  bighuman[,101:200, 101:200] <- human
  bighuman[,201:300, 201:300] <- human
  
  dimbighuman <- dim(bighuman)
  dimbigxyc <- c(dimbighuman[2:3], dimbighuman[1])
  
  numbighuman <- array(as.numeric(bighuman), dim = dimbighuman)
  
  xycbighuman <- array(as.raw(0), dim = dimbigxyc)
  xycbighuman[,,1] <- bighuman[1,,]
  xycbighuman[,,2] <- bighuman[2,,]
  xycbighuman[,,3] <- bighuman[3,,]
  
  xycnumbighuman <- array(0, dim = dimbigxyc)
  xycnumbighuman[,,1] <- numbighuman[1,,]
  xycnumbighuman[,,2] <- numbighuman[2,,]
  xycnumbighuman[,,3] <- numbighuman[3,,]
  
  greybighuman <- bighuman[1,,]
  greynumbighuman <- numbighuman[1,,]
  
  rgbabighuman <- array(as.raw(255), dim = c(4, dimbighuman[2:3]))
  rgbabighuman[1:3,,] <- bighuman
  
  rgbanumbighuman <- array(255, dim = c(4, dimbighuman[2:3]))
  rgbanumbighuman[1:3,,] <- numbighuman
  
  xycrgbabighuman <- array(as.raw(255), dim = c(dimbighuman[2:3], 4))
  xycrgbabighuman[,,1:3] <- xycbighuman
  
  xycrgbanumbighuman <- array(255, dim = c(dimbighuman[2:3], 4))
  xycrgbanumbighuman[,,1:3] <- xycnumbighuman
  
  expect_no_error(view(bighuman))
  expect_no_error(view(numbighuman))
  expect_no_error(view(xycbighuman, chorder = "xyc"))
  expect_no_error(view(xycnumbighuman, chorder = "xyc"))
  expect_no_error(view(greybighuman))
  expect_no_error(view(greynumbighuman))
  expect_no_error(view(rgbabighuman))
  expect_no_error(view(rgbanumbighuman))
  expect_no_error(view(xycrgbabighuman, chorder = "xyc"))
  expect_no_error(view(xycrgbanumbighuman, chorder = "xyc"))
  
  
  a <- sdlr:::SDLimage$new(human)
  expect_no_error(a$replace(array(0, dim = dimhuman[2:3])))
  expect_no_error(a$replace(array(as.raw(0), dim = dimhuman[2:3])))
  expect_no_error(a$destroy())
  expect_no_error(rm(a))
  expect_no_error(gc())
})