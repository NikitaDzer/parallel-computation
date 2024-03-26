#include "grid.hh"
#include "plot.hh"

#include <format>

namespace app
{

void
initPlot( visual::GNUPlot& plot,
          std::string image_dir,
          std::string plot_name )
{
    std::string title = std::format(
        "F(x) for various t."
    );

    plot.setTitle( title, 20);
    plot.setOutputFile( 
        image_dir + plot_name,
        "svg", 800, 600 
    );

    plot.setGrid();

    plot.setMTicsX( 1 );
    plot.setMTicsY( 1 );

    plot.setAutoscale();

    plot.setLabelX( "x" );
    plot.setLabelY( "F(x)" );

    plot.withLinesPoints();
} // initPlot

void
drawGrid( const pde::Grid& grid,
          visual::GNUPlot& plot)
{
    auto [x_size, t_size] = grid.getSize();
    auto [h, a] = grid.getPitch();

    int t_step = static_cast<int>( 1.0 / a);

    for ( int t = 0; t < t_size; t += t_step )
    {
        std::vector<visual::GNUPlot::Point> results{};
        results.push_back( {0, grid.getLeftNeighbour()->getAbs( 0, t)});

        for ( int x = 0; x < x_size; x++ )
        {
            results.push_back( {h*(x + 1), grid.getAbs( x, t)});
        }

        std::string fmt = std::format( "t = {}", a*t);
        plot.drawPlot( results, fmt);
    }
} // drawGrid

} // namespace app
