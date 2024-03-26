#include "grid.hh"
#include "plot.hh"
#include "drawer.hh"
#include "solver.hh"

double
gridFunction( double x,
              double t)
{
    return 0;
}

pde::Grid
createGrid()
{
    int x_size = 1000;
    int t_size = 10000;

    double h = 0.01;
    double a = 0.001;

    pde::Grid grid{ x_size, t_size, h, a, gridFunction};

    for ( int x = 0; x < x_size; x++ )
    {
        grid.setAbs( h*(x + 1), x, 0);
    }

    return grid;
} // createGrid

pde::Grid
createBorderGrid( int t_size, double a)
{
    pde::Grid grid{ 1, t_size, 0, 0, nullptr};

    for ( int t = 0; t < t_size; t++ )
    {
        grid.setAbs( -a*t, 0, t);
    }

    return grid;
} // createBorderGrid

int
main()
{
    const std::string k_dir = "assets/";

    visual::GNUPlot plot{};
    app::initPlot( plot, k_dir, "asset");

    pde::Grid grid = createGrid();
    pde::Grid border_grid = 
        createBorderGrid( grid.getSize().second, grid.getPitch().second);

    grid.setLeftNeighbour( &border_grid);
    pde::solveGridSeq( grid);

    app::drawGrid( grid, plot);
    plot.makePlot();

    // auto [x_size, t_size] = grid.getSize();
    //
    // for ( int t = t_size - 1; t >= 0; t-- )
    // {
    //     std::cout << border_grid.getAbs( 0, t) << "\t";
    //
    //     for ( int x = 0; x < x_size; x++ )
    //     {
    //         std::cout << grid.getAbs( x, t) << "\t";
    //     }
    //
    //     std::cout << std::endl;
    // }

    return 0;
}
