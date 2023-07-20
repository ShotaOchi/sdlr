assert_wav <- function(file)
{
  assert_file(file, access = "r", extension = ".wav")
}

assert_idx <- function(idx)
{
  assert_number(idx, lower = 0)
}

assert_name <- function(name)
{
  assert_string(name, min.chars = 1)
}
