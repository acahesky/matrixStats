############################################################################/**
# @RdocFunction meanOver
#
# @title "Fast averaging over subset of vector elements"
#
# @synopsis
#
# \description{
#   Computes the  sample mean of all or a subset of values.
# }
#
# \arguments{
#   \item{x}{A @numeric @vector of length N.}
#   \item{idxs}{A @numeric index @vector in [1,N] of elements to mean over.
#      If @NULL, all elements are considered.}
#   \item{na.rm}{If @TRUE, missing values are skipped, otherwise not.}
#   \item{refine}{If @TRUE and \code{x} is @numeric, then extra effort is
#      used to calculate the average with greater numerical precision,
#      otherwise not.}
#   \item{...}{Not used.}
# }
#
# \value{
#   Returns a @numeric scalar.
# }
#
# \details{
#   \code{meanOver(x, idxs)} gives equivalent results as
#   \code{mean(x[idxs])}, but is faster and more memory efficient
#   since it avoids the actual subsetting which requires copying
#   of elements and garbage collection thereof.
#
#   If \code{x} is @numeric and \code{refine=TRUE}, then a two-pass scan
#   is used to calculate the average.  The first scan calculates the total
#   sum and divides by the number of (non-missing) values.  In the second
#   scan, this average is refined by adding the residuals towards the first
#   average.  The @see "base::mean" uses this approach.
#   \code{meanOver(..., refine=FALSE)} is almost twice as fast as
#   \code{meanOver(..., refine=TRUE)}.
# }
#
# @examples "../incl/meanOver.Rex"
#
# \seealso{
#   @see "base::mean".
#   To efficiently sum over a subset, see @see "sumOver".
# }
#
# @author
#
# @keyword univar
# @keyword internal
#*/############################################################################
meanOver <- function(x, idxs=NULL, na.rm=FALSE, refine=TRUE, ...) {
  # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  # Validate arguments
  # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  # Argument 'x':
  if (!is.numeric(x)) {
    stop("Argument 'x' is not numeric: ", mode(x));
  }
  n <- length(x);

  # Argument 'na.rm':
  if (!is.logical(na.rm)) {
    stop("Argument 'na.rm' is not logical: ", mode(na.rm));
  }

  # Argument 'idxs':
  if (is.null(idxs)) {
  } else if (is.integer(idxs)) {
  } else if (is.logical(idxs)) {
    if (length(idxs) != n) {
      stop(sprintf("Lengths of arguments 'idxs' and 'x' do not match: %d != %d", length(idxs), n));
    }
    idxs <- which(idxs);
  } else {
    idxs <- as.integer(idxs);
  }

  # Argument 'refine':
  if (!is.logical(refine)) {
    stop("Argument 'refine' is not logical: ", mode(refine));
  }

  # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  # Averaging
  # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  .Call("meanOver", x, idxs, na.rm, refine, PACKAGE="matrixStats");
} # meanOver()


############################################################################
# HISTORY:
# 2014-11-02 [HB]
# o Created.
############################################################################
