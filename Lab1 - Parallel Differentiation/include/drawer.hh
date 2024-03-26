#pragma once

#include "plot.hh"
#include "grid.hh"

#include <string>

namespace app
{

void
initPlot( visual::GNUPlot& plot,
          std::string image_dir,
          std::string plot_name );

void
drawGrid( const pde::Grid& grid,
          visual::GNUPlot& plot);

} // namespace app
