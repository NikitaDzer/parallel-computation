#include <cmath>
#include <cstdlib>
#include <iostream>

#include "seq.hh"
#include "par.hh"

namespace
{

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
    if ( argc != 4 && argc != 5 )
    {
        std::cout << "Bad input.\n";
        return EXIT_FAILURE;
    }
    
    double A = std::atof( argv[1]);
    double B = std::atof( argv[2]);
    double epsilon = std::atof( argv[3]);

    integr::TaskCtx base_ctx{};
    base_ctx.setFunction( sin1x);
    base_ctx.setEpsilon( epsilon);
    base_ctx.setSegment( A, B);

    double J = 0;

    if ( argc == 4 )
    {
        integr::SeqTaskCtx seq_ctx{ base_ctx};
        seq_ctx.integrate();

        J = seq_ctx.getJ();
    } else
    {
        integr::ParTaskCtx par_ctx{ base_ctx};
        par_ctx.setThreadsNumber( std::atoi( argv[4]));
        par_ctx.integrate();

        J = par_ctx.getJ();
    }

    std::cout << J << std::endl;

    return EXIT_SUCCESS;
} // main
