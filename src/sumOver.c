/***************************************************************************
 Public methods:
 SEXP sumOver(SEXP x, SEXP idxs, SEXP naRm, SEXP mode)

 Copyright Henrik Bengtsson, 2014
 **************************************************************************/
/* Include R packages */
#include <Rdefines.h>

/* 
TEMPLATE sumOver_<Integer|Real>(...):
  SEXP sumOver_Real(SEXP x, SEXP idxs, SEXP naRm, SEXP mode)
  SEXP sumOver_Integer(SEXP x, SEXP idxs, SEXP naRm, SEXP mode)
 */
#define METHOD sumOver

#define X_TYPE 'i'
#include "sumOver_TYPE-template.h"

#define X_TYPE 'r'
#include "sumOver_TYPE-template.h"

#undef METHOD 


SEXP sumOver(SEXP x, SEXP idxs, SEXP naRm, SEXP mode) {
  SEXP ans = NILSXP;
  void *xp;
  int nx;
  int *idxsp;
  int nidxs;
  int narm, mode2;
  double sum;

  /* Argument 'x': */
  if (!isVector(x))
    error("Argument 'x' must be a vector.");
  nx = XLENGTH(x);

  /* Argument 'idxs': */
  if (isNull(idxs)) {
    idxsp = NULL;
    nidxs = 0;
  } else if (isVector(idxs)) {
    idxsp = INTEGER(idxs);
    nidxs = XLENGTH(idxs);
  } else {
    error("Argument 'idxs' must be NULL or a vector.");
  }

  /* Argument 'naRm': */
  if (!isLogical(naRm))
    error("Argument 'naRm' must be a single logical.");
  if (length(naRm) != 1)
    error("Argument 'naRm' must be a single logical.");
  narm = LOGICAL(naRm)[0];
  if (narm != TRUE && narm != FALSE) {
    error("Argument 'naRm' must be either TRUE or FALSE.");
  }

  /* Argument 'mode': */
  if (!isInteger(mode)) {
    error("Argument 'mode' must be a single integer.");
  }
  mode2 = asInteger(mode);


  /* Dispatch to low-level C function */
  if (isReal(x)) {
    xp = REAL(x);
    sum = sumOver_Real(xp, nx, idxsp, nidxs, narm, mode2);
  } else if (isInteger(x)) {
    xp = INTEGER(x);
    sum = sumOver_Integer(xp, nx, idxsp, nidxs, narm, mode2);
  } else {
    error("Argument 'x' must be numeric.");
  }


  /* Return results */
  switch (mode2) {
  case 1: /* integer */
    PROTECT(ans = allocVector(INTSXP, 1));
    if (ISNAN(sum)) {
      INTEGER(ans)[0] = NA_INTEGER;
    } else if (sum > R_INT_MAX || sum < R_INT_MIN) {
      Rf_warning("Integer overflow. Use sumOver(..., mode=\"numeric\") to avoid this.");
      INTEGER(ans)[0] = NA_INTEGER;
    } else {
      INTEGER(ans)[0] = (int)sum;
    }
    UNPROTECT(1);
    break;

  case 2: /* numeric */
    PROTECT(ans = allocVector(REALSXP, 1));
    if (sum > DBL_MAX) {
      REAL(ans)[0] = R_PosInf;
    } else if (sum < -DBL_MAX) {
      REAL(ans)[0] = R_NegInf;
    } else {
      REAL(ans)[0] = sum;
    }
    UNPROTECT(1);
    break;

  default:
    /* To please the compiler; should never happen. */
    ans = NILSXP;
    break;
  }

  return(ans);
} // sumOver()


/***************************************************************************
 HISTORY:
 2014-11-06 [HB]
  o Moved validation of arguments and construction of return object
    to this function.
 2014-11-02 [HB]
  o Created.
 **************************************************************************/
