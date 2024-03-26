#include <mpi.h>
#include <format>
#include <iostream>

namespace
{

int
get_terms_count( int argc,
                 char *argv[],
                 int size)
{
    const int kDefaultN = 8192;

    int n = kDefaultN;

    if ( argc >= 2 )
    {
        n = std::atoi( argv[1]);
    }

    /**
     * Round number of terms.
     */
    n = n / (size - 1) * (size - 1);
    
    return n;
} // get_terms_count

double
calc_pi_one( int n)
{
    return 8.0 / ((4*n + 1) * (4*n + 3));
} // calc_pi_one

double
calc_pi_partial( int n_start,
                 int count)
{
    double partial = 0.0;

    for ( int n = n_start; n < (n_start + count); n++ )
    {
        partial += calc_pi_one( n);
    }

    return partial;
} // calc_pi_partial

double
collect_pi( int size)
{
    double sum = 0.0;

    for ( int i = 1; i < size; i++ )
    {
        double partial = 0.0;

        MPI_Recv(
            &partial, 1, MPI_DOUBLE, 
            MPI_ANY_SOURCE, MPI_ANY_TAG, 
            MPI_COMM_WORLD, nullptr
        );

        sum += partial;
    }

    return sum;
} // collect_pi

} // anonymous namespace

int
main( int argc,
      char *argv[])
{
    const int kMasterRank = 0;

    int size = 0;
    int rank = 0;

    MPI_Init( &argc, &argv);
    MPI_Comm_size( MPI_COMM_WORLD, &size);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);

    if ( size == 1 )
    {
        std::cout << "Size 1 is incorrect.\n";
        return EXIT_FAILURE;
    }

    int n_max = get_terms_count( argc, argv, size);
    int n_per_process = n_max / (size - 1);

    if ( rank == kMasterRank )
    {
        double pi = collect_pi( size);

        std::string fmt = std::format(
            "Size = {}, N = {}, PI = {}\n",
            size, n_max, pi 
        );
        std::cout << fmt;
    } else
    {
        double partial = calc_pi_partial(
            n_per_process * (rank - 1),
            n_per_process
        );
        MPI_Send(
            &partial, 1, MPI_DOUBLE, 
            kMasterRank, 0,
            MPI_COMM_WORLD
        );
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
} // main
