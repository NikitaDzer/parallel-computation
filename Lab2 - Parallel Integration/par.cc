#include "par.hh"
#include "seq.hh"

namespace integr
{

void
integrateParallel( ParTaskCtx& ctx)
{
    SeqTaskCtx tasks{};

    double A = 0;
    double B = 0;
    double fnA = 0;
    double fnB = 0;
    double sAB = 0;

    double J = 0;

    while ( true )
    {
        {
            std::lock_guard guard{ ctx.getGuard()};

            if ( ctx.isEmpty() && ctx.getActive() == 0 )
            {
                for ( int i = 0; i < ctx.getThreadsNumber(); i++ )
                {
                    ctx.pushTop( 2, 1, 0, 0, 0);
                    ctx.postTask();
                }
            }
        }

        ctx.waitTask();

        {
            std::lock_guard guard{ ctx.getGuard()};
            ctx.extractTop( A, B, fnA, fnB, sAB);
        }

        if ( A > B )
        {
            break;
        }

        ctx.incrementActive();

        while ( true )
        {
            double C = (A + B) / 2;
            double fnC = ctx.getFunction()( C);

            double sAC = (fnA + fnC) * (C - A) / 2;
            double sCB = (fnC + fnB) * (B - C) / 2;
            double sACB = sAC + sCB;

            if ( std::abs( sAB - sACB) > ctx.getEpsilon() * std::abs( sACB) )
            {
                tasks.pushTop( A, C, fnA, fnC, sAC);

                A = C;
                fnA = fnC;
                sAB = sCB;
            } else
            {
                J += sACB;

                if ( tasks.isEmpty() )
                {
                    break;
                }

                tasks.extractTop( A, B, fnA, fnB, sAB);
            }

            if ( tasks.getSize() > 2 && ctx.isEmpty() )
            {
                std::lock_guard guard{ ctx.getGuard()};

                while ( tasks.getSize() > 1 )
                {
                    tasks.moveTopTo( ctx);
                    ctx.postTask();
                }
            }
        }

        ctx.decrementActive();
    }

    ctx.addJ( J);
} // integrateParallel

} // namespace integr
