#include "dtwclust++.h"

namespace dtwclust {

RcppExport SEXP distmat_loop(SEXP D, SEXP X, SEXP Y,
                             SEXP DIST, SEXP DIST_ARGS,
                             SEXP FILL_TYPE, SEXP MAT_TYPE, SEXP ENDPOINTS)
{
    BEGIN_RCPP
    auto distmat = DistmatFactory().create(MAT_TYPE, D);
    auto dist_calculator = DistanceCalculatorFactory().create(DIST, DIST_ARGS);
    auto distmat_filler = DistmatFillerFactory().create(FILL_TYPE, distmat, ENDPOINTS,
                                                        dist_calculator);
    distmat_filler->fill(X, Y);
    return R_NilValue;
    END_RCPP
}

} // namespace dtwclust