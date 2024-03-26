#include "grid.hh"
#include "scheme.hh"

namespace pde
{

void
solveGridSeq( pde::Grid& grid)
{
    grid.setCurrentNode( 0, 0);

    auto [x_size, t_size] = grid.getSize();
    int n_visit_nodes = (t_size - 1) * x_size;

    for ( int i = 0; i < n_visit_nodes; i++ )
    {
        double value = pde::calcGridCentral( grid);
        grid.set( value, 0, 1);
        grid.moveCurrentNode();
    }
} // solveGridSeq

} // namespace pde
