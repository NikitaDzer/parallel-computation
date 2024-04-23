#pragma once

#include <stack>
#include <cassert>
#include <functional>

namespace integr
{

using Function1D = std::function<double( double)>;

struct Task
{
    double A;
    double B;
    double fnA;
    double fnB;
    double s;
}; // struct Task

class TaskCtx
{

public:
    TaskCtx() = default;
    
    Function1D getFunction() const noexcept
        { return m_function; }
    void setFunction( Function1D function) noexcept
        { assert( function); m_function = function; }

    double getEpsilon() const noexcept
        { return m_epsilon; }
    void setEpsilon( double epsilon) noexcept
        { assert( epsilon > 0.0); m_epsilon = epsilon; }

    void setSegment( double A, double B)
    {
        assert( A < B);
        assert( m_function);
        assert( m_tasks.empty());

        double fnA = m_function( A);
        double fnB = m_function( B);
        double sAB = (fnA + fnB) * (B - A) / 2;

        m_tasks.push( {A, B, fnA, fnB, sAB});
    }

    void extractTop( double& A, double& B, 
                     double& fnA, double& fnB,
                     double& sAB)
    {
        assert( m_tasks.size() > 0);

        const Task& tmp = m_tasks.top();
        A = tmp.A;
        B = tmp.B;
        fnA = tmp.fnA;
        fnB = tmp.fnB;
        sAB = tmp.s;

        m_tasks.pop();
    }
    void moveTopTo( TaskCtx& ctx)
    {
        assert( m_tasks.size() > 0);
    
        ctx.m_tasks.push( m_tasks.top());
        m_tasks.pop();
    }
    void pushTop( double A, double B,
                  double fnA, double fnB,
                  double sAB)
    { 
        m_tasks.push( {A, B, fnA, fnB, sAB});
    }

    bool isEmpty() const
        { return m_tasks.empty(); }
    std::size_t getSize() const
        { return m_tasks.size(); }

    virtual double getJ() const
    { 
        assert( 0 && "Unimplemented TaskCtx::getJ.");

        *(volatile char*)0 = 0;
        std::abort();
    }
    virtual void setJ( double j)
    {
        assert( 0 && "Unimplemented TaskCtx::setJ.");

        *(volatile char*)0 = 0;
        std::abort();
    }

    virtual void integrate()
    {
        assert( 0 && "Unimplemented TaskCtx::integrate.");

        *(volatile char*)0 = 0;
        std::abort();
    }
    
protected:
    Function1D m_function;
    double m_epsilon;
    std::stack<Task> m_tasks;

}; // class TaskCtx

} // namespace integr
