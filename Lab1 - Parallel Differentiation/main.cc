#include "grid.hh"
#include "plot.hh"
#include "drawer.hh"
#include "solver.hh"

#include <iostream>
#include <mpi.h>

namespace
{

double
gridFunction( double x,
              double t)
{
    return 0;
} // gridFunction

pde::Grid
createGrid( int x_size, 
            int t_size, 
            int rank)
{
    double h = 0.01;
    double a = 0.001;

    pde::Grid grid{ x_size, t_size, h, a, gridFunction};

    for ( int x = 0; x < x_size; x++ )
    {
        grid.setAbs( h*(rank * x_size + x + 1), x, 0);
    }

    return grid;
} // createGrid

pde::Grid
createNeighbourGrid( int t_size)
{
    return pde::Grid{ 1, t_size, 0, 0, nullptr};
} // createNeighbourGrid

pde::Grid
createBorderGrid( int t_size, 
                  double a)
{
    pde::Grid grid{ 1, t_size, 0, 0, nullptr};

    for ( int t = 0; t < t_size; t++ )
    {
        grid.setAbs( -a*t, 0, t);
    }

    return grid;
} // createBorderGrid

void
sendGrid( const pde::Grid& grid, int rank)
{
    auto [x_size, t_size] = grid.getSize();
    int data_size = x_size * t_size;

    MPI_Send(
        &rank, 1, MPI_INT,
        0, 0, 
        MPI_COMM_WORLD
    );

    MPI_Send(
        grid.getGrid(), data_size, MPI_DOUBLE,
        0, 0, 
        MPI_COMM_WORLD
    );
} // sendGrid

pde::Grid
collectGrid( pde::Grid&& master_grid,
             int size)
{
    auto [x_size, t_size] = master_grid.getSize();
    auto [h, a] = master_grid.getPitch();

    pde::Grid grid{
        x_size * size, t_size, h, a,
        master_grid.getFunction(), master_grid.getLeftNeighbour()
    };

    const double* raw_grid = master_grid.getGrid();
    grid.copyGrid( raw_grid, x_size, t_size, 0, 0);

    std::vector<double> data{};
    int data_size = x_size * t_size;

    data.resize( data_size);

    for ( int i = 1; i < size; i++ )
    {
        int rank = 0;

        MPI_Recv(
            &rank, 1, MPI_INT,
            MPI_ANY_SOURCE, MPI_ANY_TAG, 
            MPI_COMM_WORLD, nullptr
        );

        MPI_Recv(
            data.data(), data_size, MPI_DOUBLE,
            rank, MPI_ANY_TAG,
            MPI_COMM_WORLD, nullptr
        );

        grid.copyGrid( data.data(), x_size, t_size, i * x_size, 0);
    }

    return grid;
} // collectGrid

} // anonymous namespace

int
main( int argc, 
      char* argv[])
{
    const std::string kPlotDir = "assets/";
    const int kMasterRank = 0;

    const int x_size = 1000;
    const int t_size = 10000;

    int size = 0;
    int rank = 0;

    MPI_Init( &argc, &argv);
    MPI_Comm_size( MPI_COMM_WORLD, &size);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);

    if ( argc == 1 )
    {
        visual::GNUPlot plot{};

        pde::Grid grid = createGrid( x_size, t_size, 0);
        pde::Grid border_grid = 
            createBorderGrid( grid.getSize().second, grid.getPitch().second);

        grid.setLeftNeighbour( &border_grid);
        pde::solveGridSeq( grid);

        app::initPlot( plot, kPlotDir, "seq");
        app::drawGrid( grid, plot);
        plot.makePlot();
    } else
    {
        visual::GNUPlot plot{};

        double start = MPI_Wtime();

        pde::Grid grid = createGrid( x_size / size, t_size, rank);
        pde::Grid neighbour_left{};
        pde::Grid neighbour_right{};

        /**
         * All processes have left neighbour.
         */
        if ( rank == kMasterRank )
        {
            neighbour_left = createBorderGrid( t_size, grid.getPitch().second);
        } else
        {
            neighbour_left = createNeighbourGrid( t_size);
        }
        grid.setLeftNeighbour( &neighbour_left);

        /**
         * Last (size - 1) process doesn't have right neighbour.
         */
        if ( rank < (size - 1) )
        {
            neighbour_right = createNeighbourGrid( t_size);
            grid.setRightNeighbour( &neighbour_right);
        }

        pde::solveGridPar( grid, rank);

        if ( rank == kMasterRank )
        {
            pde::Grid collection = collectGrid( std::move( grid), size);

            std::cout << MPI_Wtime() - start << std::endl;

            app::initPlot( plot, kPlotDir, "par");
            app::drawGrid( collection, plot);

            plot.makePlot();
        } else
        {
            sendGrid( grid, rank);
        }
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
} // main
