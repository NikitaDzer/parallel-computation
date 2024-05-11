#pragma once

#include "task.hh"

namespace integr
{

/**
 * Forwarding declarations.
 */
class SeqTaskCtx;

void
integrateSequentially( SeqTaskCtx& ctx);

class SeqTaskCtx final : public TaskCtx
{

public:
    using Base = TaskCtx;

public:
    SeqTaskCtx() = default;

    SeqTaskCtx( const Base& base):
        Base{ base}
    {
    }

    double getJ() const noexcept override
        { return m_j; }
    void setJ( double j) noexcept override
        { m_j = j; }
    void addJ( double j) noexcept
        { m_j += j; }

    void integrate() override
    {
        assert( m_function);
        assert( m_epsilon > 0.0);
        assert( m_tasks.size() > 0);

        integrateSequentially( *this);
    }

private:
    double m_j;

}; // class SeqTaskCtx

} // namespace integr
