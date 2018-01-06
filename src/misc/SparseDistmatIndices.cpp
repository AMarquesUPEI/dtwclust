#include "misc.h"

#include <unordered_set>
#include <vector>

#include <RcppArmadillo.h>

namespace dtwclust {

class SparseDistmatIndices
{
public:
    SparseDistmatIndices(int num_rows)
        : num_rows_(num_rows)
    { }

    Rcpp::IntegerMatrix getNewIndices(
            const Rcpp::IntegerVector& i,
            const Rcpp::IntegerVector& j,
            const bool symmetric)
    {
        std::vector<int> new_i, new_j;
        for (int ii = 0; ii < i.length(); ii++) {
            for (int jj = 0; jj < j.length(); jj++) {
                int this_i = i[ii];
                int this_j = j[jj];
                if (symmetric && this_j > this_i) {
                    int swap = this_i;
                    this_i = this_j;
                    this_j = swap;
                }
                int ij = this_i + (this_j - 1) * num_rows_;
                bool new_index = existing_indices_.find(ij) == existing_indices_.end();
                if (new_index) {
                    existing_indices_.insert(ij);
                    new_i.push_back(this_i);
                    new_j.push_back(this_j);
                }
            }
        }
        Rcpp::IntegerMatrix new_ids(new_i.size(), 2);
        for (int k = 0; k < new_ids.nrow(); k++) {
            new_ids(k, 0) = new_i[k];
            new_ids(k, 1) = new_j[k];
        }
        return new_ids;
    }

private:
    int num_rows_;
    std::unordered_set<int> existing_indices_;
};

RcppExport SEXP SparseDistmatIndices__new(SEXP num_rows)
{
    BEGIN_RCPP
    Rcpp::XPtr<SparseDistmatIndices> ptr(new SparseDistmatIndices(Rcpp::as<int>(num_rows)), true);
    return ptr;
    END_RCPP
}

RcppExport SEXP SparseDistmatIndices__getNewIndices(SEXP xptr, SEXP i, SEXP j, SEXP symmetric)
{
    BEGIN_RCPP
    Rcpp::XPtr<SparseDistmatIndices> ptr(xptr);
    return ptr->getNewIndices(i, j, Rcpp::as<bool>(symmetric));
    END_RCPP
}

} // namespace dtwclust
