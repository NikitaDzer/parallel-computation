#include "seq.hh"

namespace integr
{

void
integrateSequentially( SeqTaskCtx& ctx)
{
    double J = 0;

    double A = 0;
    double B = 0;
    double fnA = 0;
    double fnB = 0;
    double sAB = 0;

    ctx.extractTop( A, B, fnA, fnB, sAB);

    while ( true )
    {
        double C = (A + B) / 2;
        double fnC = ctx.getFunction()( C);

        double sAC = (fnA + fnC) * (C - A) / 2;
        double sCB = (fnC + fnB) * (B - C) / 2;

        double sACB = sAC + sCB;

        if ( std::abs( sAB - sACB) > ctx.getEpsilon() * std::abs( sACB) )
        {
            ctx.pushTop( A, C, fnA, fnC, sAC);

            A = C;
            fnA = fnC;
            sAB = sCB;
        } else
        {
            J += sACB;

            if ( ctx.isEmpty() )
            {
                break;
            }

            ctx.extractTop( A, B, fnA, fnB, sAB);
        }
    }

    ctx.setJ( J);
} // integrateSequentially

} // namespace integr
