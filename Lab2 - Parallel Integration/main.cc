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

    std::unique_ptr<integr::TaskCtx> ctx{};

    if ( argc == 4 )
    {
        ctx = std::make_unique<integr::SeqTaskCtx>( base_ctx);
    } else
    {
        uint32_t threads_number = std::atoi( argv[4]);
        ctx = std::make_unique<integr::ParTaskCtx>( base_ctx, threads_number);
    }

    ctx->integrate();

    std::cout << ctx->getJ() << std::endl;

    return EXIT_SUCCESS;
} // main
