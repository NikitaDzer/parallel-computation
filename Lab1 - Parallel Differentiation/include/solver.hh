#pragma once

#include "grid.hh"

namespace pde
{

void
solveGridSeq( pde::Grid& grid);

void
solveGridPar( pde::Grid& grid, int rank);

} // namespace pde
