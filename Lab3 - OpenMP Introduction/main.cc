#include "matrix/matrix.hh"

#include <functional>

namespace
{

auto RunMatmulRowRow(const std::vector<int>& elems, int size, bool is_seq)
{
    using namespace utils;

    SquareMatrix<int, RowMajor> lhs{size};
    SquareMatrix<int, RowMajor> rhs{size};

    lhs.set(elems);
    rhs.set(elems);

    return is_seq
        ? MatmulSeq(lhs, rhs)
        : MatmulPar(lhs, rhs);
} // RunMatmulRowRow

auto RunMatmulRowCol(const std::vector<int>& elems, int size, bool is_seq)
{
    using namespace utils;

    SquareMatrix<int, RowMajor> lhs{size};
    SquareMatrix<int, ColMajor> rhs{size};

    lhs.set(elems);
    rhs.set(elems);

    return is_seq
        ? MatmulSeq(lhs, rhs)
        : MatmulPar(lhs, rhs);
} // RunMatmulRowCol

auto RunMatmulColRow(const std::vector<int>& elems, int size, bool is_seq)
{
    using namespace utils;

    SquareMatrix<int, ColMajor> lhs{size};
    SquareMatrix<int, RowMajor> rhs{size};

    lhs.set(elems);
    rhs.set(elems);

    return is_seq
        ? MatmulSeq(lhs, rhs)
        : MatmulPar(lhs, rhs);
} // RunMatmulColRow

auto RunMatmulColCol(const std::vector<int>& elems, int size, bool is_seq)
{
    using namespace utils;

    SquareMatrix<int, ColMajor> lhs{size};
    SquareMatrix<int, ColMajor> rhs{size};

    lhs.set(elems);
    rhs.set(elems);

    return is_seq
        ? MatmulSeq(lhs, rhs)
        : MatmulPar(lhs, rhs);
} // RunMatmulColCol

} // anonymous namespace

int main(int argc, char* argv[])
{
    using namespace utils;

    if ( argc != 5 )
    {
        return EXIT_FAILURE;
    }

    int size = std::atoi(argv[1]);
    std::string mode = std::string(argv[2]);
    std::string layout_a = std::string(argv[3]);
    std::string layout_b = std::string(argv[4]);

    std::vector<int> elems(size * size);

    for ( int row = 0; row < size; row++ )
    {
        for ( int col = 0; col < size; col++ )
        {
            elems.push_back(row * size + col);
        }
    }

    std::function<SquareMatrix<int, RowMajor>(const std::vector<int>&, int, bool)> Matmul;

    if ( layout_a == "-row" && layout_b == "-row" )
    {
        Matmul = RunMatmulRowRow;
    } else if ( layout_a == "-row" && layout_b == "-col" )
    {
        Matmul = RunMatmulRowCol;
    } else if ( layout_a == "-col" && layout_b == "-row" )
    {
        Matmul = RunMatmulColRow;
    } else if ( layout_a == "-col" && layout_b == "-col" )
    {
        Matmul = RunMatmulColCol;
    } else
    {
        return EXIT_FAILURE;
    }

    if ( mode == "-seq" )
    {
        Matmul(elems, size, true);
    } else if ( mode == "-par" )
    {
        Matmul(elems, size, false);
    } else if ( mode == "-check" )
    {
        auto res_seq = Matmul(elems, size, true);
        auto res_par = Matmul(elems, size, false);

        if ( res_seq == res_par )
        {
            std::cout << "Success\n";
        } else 
        {
            std::cout << "Failure\n";
            res_seq.dump();
            res_par.dump();
        }
    } else 
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
} // main
