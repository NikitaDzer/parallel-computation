#pragma once

#include <cassert>
#include <memory>

namespace pde
{

class Grid
{

public:
    using Function = double(double x, double t);

public:
    Grid( int x_size, int t_size,
          double h, double a,
          Grid::Function* function,
          Grid* neighbour_left = nullptr,
          Grid* neighbour_right = nullptr):
        m_x_size{ x_size}, m_t_size{ t_size},
        m_x_cur{ 0}, m_t_cur{ 0},
        m_h{ h}, m_a{ a},
        m_function{ function}, 
        m_neighbour_left{ neighbour_left},
        m_neighbour_right{ neighbour_right},
        m_grid{ nullptr}
    {
        m_grid = std::make_unique<double[]>( x_size * t_size);
    }
    
    double get( int x = 0, int t = 0) const
        { return accessNode( m_x_cur + x, m_t_cur + t); }
    void set( double value, int x = 0, int t = 0)
        { accessNode( m_x_cur + x, m_t_cur + t) = value; }

    double getAbs( int x, int t) const
        { return accessNode( x, t); }
    void setAbs( double value, int x, int t)
        { accessNode( x, t) = value; }

    double getLeft( int t) const
        { return getAbs( 0, t); }
    double getRight( int t) const
        { return getAbs( m_x_size - 1, t); }

    std::pair<int, int> getCurrentNode() const
        { return {m_x_cur, m_t_cur}; }
    void setCurrentNode( int x, int t)
        { m_x_cur = x; m_t_cur = t; }
    void moveCurrentNode( int step = 1)
    {
        int x_new = (m_x_cur + step) % m_x_size;
        int t_new = m_t_cur + (m_x_cur + step) / m_x_size;

        setCurrentNode( x_new, t_new);
    }
    bool isCurrentNodeLeft() const
        { return m_x_cur == 0; }
    bool isCurrentNodeRight() const
        { return m_x_cur == (m_x_size - 1); }

    std::pair<int, int> getSize() const
        { return {m_x_size, m_t_size}; }

    std::pair<double, double> getPitch() const
        { return {m_h, m_a}; }

    Grid::Function* getFunction() const
        { return m_function; }

    Grid* getLeftNeighbour() const
        { return m_neighbour_left; }
    void setLeftNeighbour( Grid* neighbour_left)
        { m_neighbour_left = neighbour_left; }
    bool hasLeftNeighbour() const
        { return m_neighbour_left != nullptr; }

    Grid* getRightNeighbour() const
        { return m_neighbour_right; }
    void setRightNeighbour( Grid* neighbour_right)
        { m_neighbour_right = neighbour_right; }
    bool hasRightNeighbour() const
        { return m_neighbour_right != nullptr; }


private:
    int m_x_size;
    int m_t_size;

    int m_x_cur;
    int m_t_cur;

    double m_h;
    double m_a;

    Grid::Function* m_function;

    Grid* m_neighbour_left;
    Grid* m_neighbour_right;

    std::unique_ptr<double[]> m_grid;


private:
    double& accessNode( int x, int t) const
    {
        assert( 0 <= x && x < m_x_size);
        assert( 0 <= t && t < m_t_size);

        return m_grid[(t * m_x_size) + x];
    }

}; // class Grid

const inline Grid kDefaultGrid{
    1, 1, 0, 0, nullptr
};

} // namespace pde
