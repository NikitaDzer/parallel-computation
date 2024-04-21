#include <cassert>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <stack>

namespace
{

using Function = std::function<double( double)>;

struct Task
{
    double A;
    double B;
    double fnA;
    double fnB;
    double s;
}; // struct Task

double
integrateLocalStack( Function fn,
                     double A,
                     double B,
                     double eps)
{
    assert( fn);
    assert( A < B);
    assert( eps > 0.0);

    std::stack<Task> tasks{};

    double J = 0;

    double fnA = fn( A);
    double fnB = fn( B);

    double sAB = (fnA + fnB) * (B - A) / 2;

    while ( true )
    {
        double C = (A + B) / 2;
        double fnC = fn( C);

        double sAC = (fnA + fnC) * (C - A) / 2;
        double sCB = (fnC + fnB) * (B - C) / 2;

        double sACB = sAC + sCB;

        if ( std::abs( sAB - sACB) > eps * std::abs( sACB) )
        {
            tasks.push( {A, C, fnA, fnC, sAC});

            A = C;
            fnA = fnC;
            sAB = sCB;
        } else
        {
            J += sACB;

            if ( tasks.empty() )
            {
                break;
            }

            Task& task = tasks.top();
            A = task.A;
            B = task.B;
            fnA = task.fnA;
            fnB = task.fnB;
            sAB = task.s;

            tasks.pop();
        }
    }

    return J;
} // integrateLocalStack

double
sin1x( double x)
{
    return std::sin( 1.0 / x);
} // sin1x

} // anonymous namespace

int
main( const int argc,
      const char *argv[])
{
    if ( argc != 4 )
    {
        std::cout << "Bad input.\n";
        return EXIT_FAILURE;
    }

    double A = std::atof( argv[1]);
    double B = std::atof( argv[2]);
    double eps = std::atof( argv[3]);

    std::cout << integrateLocalStack( sin1x, A, B, eps) << "\n";

    return EXIT_SUCCESS;
} // main
