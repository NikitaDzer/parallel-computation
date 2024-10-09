#pragma once

#include <cstring>
#include <cstdio>
#include <vector>
#include <cassert>
#include <iostream>

#include <omp.h>

namespace utils
{

enum MatrixLayout
{
    RowMajor,
    ColMajor
};

template <typename T, MatrixLayout Layout = RowMajor>
class SquareMatrix final
{

public:
    SquareMatrix(int size):
        m_elems(size * size, 0),
        m_size{size}
    {}

    const T& get(int row, int col) const 
    { 
        return m_elems[getPos(row, col)];
    }

    void set(int row, int col, const T& value)
    { 
        m_elems[getPos(row, col)] = value;
    }

    void set(const std::vector<T>& elems)
    {
        m_elems = elems;
    }

    int size() const { return m_size; }

    void dump(std::ostream& ostream = std::cout) const
    {
        ostream << "\n";

        for ( int row = 0; row < m_size; row++ )
        {
            for ( int col = 0; col < m_size; col++ )
            {
                ostream << "\t" << get(row, col);
            }

            ostream << "\n";
        }
    }

    template <typename U, MatrixLayout LayoutA, MatrixLayout LayoutB>
    friend SquareMatrix<U, RowMajor> MatmulSeq(const SquareMatrix<U, LayoutA>& lhs, const SquareMatrix<U, LayoutB>& rhs);

    template <typename U, MatrixLayout LayoutA, MatrixLayout LayoutB>
    friend SquareMatrix<U, RowMajor> MatmulPar(const SquareMatrix<U, LayoutA>& lhs, const SquareMatrix<U, LayoutB>& rhs);

    template <typename U, MatrixLayout LayoutA, MatrixLayout LayoutB>
    friend SquareMatrix<U, RowMajor> operator * (const SquareMatrix<U, LayoutA>& lhs, const SquareMatrix<U, LayoutB>& rhs);

    template <typename U, MatrixLayout LayoutA>
    friend bool operator == (const SquareMatrix<U, LayoutA>& lhs, const SquareMatrix<U, LayoutA>& rhs);

private:
    int getPos(int row, int col) const
    { 
        return Layout == RowMajor 
            ? row * m_size + col 
            : row + col * m_size;
    }

private:
    std::vector<T> m_elems;
    int m_size;

}; // class SquareMatrix

template <typename T, MatrixLayout LayoutA, MatrixLayout LayoutB>
inline SquareMatrix<T, RowMajor> MatmulSeq(const SquareMatrix<T, LayoutA>& lhs, const SquareMatrix<T, LayoutB>& rhs)
{
    assert(lhs.size() == rhs.size());

    int size = lhs.size();
    SquareMatrix<T, RowMajor> res{size};

    for ( int lhs_row = 0; lhs_row < size; lhs_row++ ) 
    {
        for ( int rhs_col = 0; rhs_col < size; rhs_col++ ) 
        {
            T value{};

            for ( int i = 0; i < size; i++ )
            {
                value += lhs.get(lhs_row, i) * rhs.get(i, rhs_col);
            }

            res.set(lhs_row, rhs_col, value);
        }
    }

    return res;
} // MatmulSeq

template <typename T, MatrixLayout LayoutA, MatrixLayout LayoutB>
inline SquareMatrix<T, RowMajor> MatmulPar(const SquareMatrix<T, LayoutA>& lhs, const SquareMatrix<T, LayoutB>& rhs)
{
    assert(lhs.size() == rhs.size());

    int size = lhs.size();
    SquareMatrix<T, RowMajor> res{size};

    #pragma omp parallel for
    for ( int lhs_row = 0; lhs_row < size; lhs_row++ ) 
    {
        for ( int rhs_col = 0; rhs_col < size; rhs_col++ ) 
        {
            T value{};

            for ( int i = 0; i < size; i++ )
            {
                value += lhs.get(lhs_row, i) * rhs.get(i, rhs_col);
            }

            res.set(lhs_row, rhs_col, value);
        }
    }

    return res;
} // MatmulPar

template <typename T, MatrixLayout LayoutA, MatrixLayout LayoutB>
inline SquareMatrix<T, RowMajor> operator * (const SquareMatrix<T, LayoutA>& lhs, const SquareMatrix<T, LayoutB>& rhs)
{
    return MatmulSeq(lhs, rhs);
} // operator * (const SquareMatrix<T, MatrixLayout>&, const SquareMatrix<T, MatrixLayout>&)

template <typename T, MatrixLayout Layout>
inline bool operator == (const SquareMatrix<T, Layout>& lhs, const SquareMatrix<T, Layout>& rhs)
{
    assert(lhs.size() == rhs.size());
    return (std::memcmp(lhs.m_elems.data(), rhs.m_elems.data(), lhs.m_elems.size() * sizeof(T)) == 0);
} // operator == (const SquareMatrix<T, MatrixLayout>&, const SquareMatrix<T, MatrixLayout>&)

} // namespace utils

