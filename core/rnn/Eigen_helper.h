#ifndef EIGEN_HELPER_H_
#define EIGEN_HELPER_H_

#include <fstream>

namespace Eigen
{

template<class Matrix>
bool write_binary(const std::string &filename, const Matrix &matrix)
{
    std::ofstream out(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    typename Matrix::Index rows = matrix.rows(), cols = matrix.cols();
    out.write((char*) (&rows), sizeof(typename Matrix::Index));
    out.write((char*) (&cols), sizeof(typename Matrix::Index));
    out.write((char*) matrix.data(), rows * cols * sizeof(typename Matrix::Scalar));
    return out.is_open();
}

template<class Matrix>
bool read_binary(const std::string &filename, Matrix &matrix)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    typename Matrix::Index rows = 0, cols = 0;
    in.read((char*) (&rows), sizeof(typename Matrix::Index));
    in.read((char*) (&cols), sizeof(typename Matrix::Index));
    matrix.resize(rows, cols);
    in.read((char *) matrix.data(), rows * cols * sizeof(typename Matrix::Scalar));
    return in.is_open();
}

template<class Matrix> // to circumvent problems with allFinite or hasNaN functions
bool is_all_finite(const Matrix &matrix)
{
    bool allFinite = true;
    typedef typename Matrix::Scalar EScalar;
    const EScalar *beg  = matrix.data();
    const EScalar *end  = matrix.data() + matrix.size();
    for (const EScalar *it = beg; it != end; it++)
    {
        const auto &i = *it;
        if (isinff(i) == true || isnanf(i) == true || isinf(i) == true || isnan(i) == true)
        {
            allFinite = false;
            break;
        } 
    }
    return allFinite;
}

}

#endif /* EIGEN_HELPER_H_ */