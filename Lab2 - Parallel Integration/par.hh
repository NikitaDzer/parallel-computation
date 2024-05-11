#pragma once

#include "task.hh"

#include <mutex>
#include <semaphore>
#include <atomic>
#include <thread>

namespace integr
{

/**
 * Forwarding declarations.
 */
class ParTaskCtx;

void
integrateParallel( ParTaskCtx& ctx);

class ParTaskCtx final : public TaskCtx
{

public:
    using Base = TaskCtx;

public:
    ParTaskCtx( const Base& base, uint32_t threads_number):
        Base{ base},
        m_threads_number{ threads_number}
    {
    }

    std::lock_guard<std::mutex> getGuard()
        { return std::lock_guard{ m_mutex}; }

    void waitTask() const
        { m_task_present.acquire(); }
    void postTask() const
        { m_task_present.release(); }

    uint32_t getActive() const
        { return m_active_number.load(); }
    void incrementActive()
        { m_active_number.fetch_add( 1); }
    void decrementActive()
        { m_active_number.fetch_sub( 1); }

    double getJ() const override
        { return m_j.load(); }
    void setJ( double j) override
        { m_j.store( j); }
    void addJ( double j)
        { m_j.fetch_add( j); }

    uint32_t getThreadsNumber() const noexcept
        { return m_threads_number; }
    void setThreadsNumber( uint32_t threads_number) noexcept
        { assert( threads_number > 0); m_threads_number = threads_number; }

    void integrate() override
    {
        assert( m_function);
        assert( m_epsilon > 0.0);
        assert( m_tasks.size() > 0);
        assert( m_threads_number > 0);

        std::vector<std::jthread> threads{};
        
        for ( uint32_t i = 0; i < m_threads_number; i++ )
        {
            threads.push_back( std::jthread{ integrateParallel, std::ref( *this)});
        }
    }

private:
    mutable std::mutex m_mutex;
    mutable std::binary_semaphore m_task_present{ 1};

    std::atomic<uint32_t> m_active_number;
    std::atomic<double> m_j;

    uint32_t m_threads_number;

}; // class ParTaskCtx

} // namespace integr
