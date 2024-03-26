#include "grid.hh"

namespace
{

double
calcDSLeftCorner( double left, 
                  double current,
                  double f,
                  double h,
                  double a)
{
    return f*a - a/h * (current - left) + current;
} // calcDSLeftCorner

double
calcDSRightCorner( double current,
                   double right,
                   double f,
                   double h,
                   double a)
{
    return f*a - a/h * (right - current) + current;
} // calcDSRightCorner

double
calcDSCentral( double left,
               double right,
               double f,
               double h,
               double a)
{
    return f*a - a/(2*h) * (right - left) + 0.5 * (right + left);
} // calcDSCentral

} // namespace anonymous

namespace pde
{

double
calcGridCentral( const pde::Grid& grid)
{
    auto [h, a] = grid.getPitch();

    double left = 0.0;
    double right = 0.0;
    double current = 0.0;

    auto [x, t] = grid.getCurrentNode();

    double f = grid.getFunction()( h*x, a*t);

    if ( grid.isCurrentNodeLeft() )
    {
        if ( !grid.hasLeftNeighbour() )
        {
            current = grid.get();
            right = grid.get( 1);

            return calcDSRightCorner( current, right, f, h, a);
        }

        left = grid.getLeftNeighbour()->getRight( t);
        right = grid.get( 1);
    } else if ( grid.isCurrentNodeRight() )
    {
        if ( !grid.hasRightNeighbour() )
        {
            left = grid.get( -1);
            current = grid.get();

            return calcDSLeftCorner( left, current, f, h, a);
        }

        left = grid.get( -1);
        right = grid.getRightNeighbour()->getLeft( t);
    } else
    {
        left = grid.get( -1);
        right = grid.get( 1);
    }

    return calcDSCentral( left, right, f, h, a);
} // calcGridCentral

} // namespace pde
