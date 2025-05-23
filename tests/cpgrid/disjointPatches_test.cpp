//===========================================================================
//
// File: disjointPatches_test.cpp
//
// Created: May 30 2023 16:34:00
//
// Author(s): Antonella Ritorto   <antonella.ritorto@opm-op.com>
//
// $Date$
//
// $Revision$
//
//===========================================================================
/*
  Copyright 2023 Equinor ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "config.h"

#define BOOST_TEST_MODULE LGRTests
#include <boost/test/unit_test.hpp>
#include <boost/version.hpp>
#if BOOST_VERSION / 100000 == 1 && BOOST_VERSION / 100 % 1000 < 71
#include <boost/test/floating_point_comparison.hpp>
#else
#include <boost/test/tools/floating_point_comparison.hpp>
#endif
#include <opm/grid/CpGrid.hpp>


#include <sstream>
#include <iostream>

struct Fixture
{
    Fixture()
    {
        int m_argc = boost::unit_test::framework::master_test_suite().argc;
        char** m_argv = boost::unit_test::framework::master_test_suite().argv;
        Dune::MPIHelper::instance(m_argc, m_argv);
        Opm::OpmLog::setupSimpleDefaultLogging();
    }

    static int rank()
    {
        int m_argc = boost::unit_test::framework::master_test_suite().argc;
        char** m_argv = boost::unit_test::framework::master_test_suite().argv;
        return Dune::MPIHelper::instance(m_argc, m_argv).rank();
    }
};

BOOST_GLOBAL_FIXTURE(Fixture);

void disjointPatches_check(Dune::CpGrid& grid,
                           const std::vector<std::array<int,3>>& startIJK_vec,
                           const std::vector<std::array<int,3>>& endIJK_vec)
{
    const auto& data = grid.currentData();
    try
    {
        std::cout << std::boolalpha;
        std::cout << (*data[0]).disjointPatches(startIJK_vec, endIJK_vec) << '\n';

    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

BOOST_AUTO_TEST_CASE(lgrs_disjointPatches)
{
    // Create a grid
    Dune::CpGrid grid;
    const std::array<double, 3> cell_sizes = {1.0, 1.0, 1.0};
    const std::array<int, 3> grid_dim = {4,3,3};
    grid.createCartesian(grid_dim, cell_sizes);
    const std::vector<std::array<int,3>> startIJK_vec = {{0,0,0}, {0,0,2}, {3,2,2}};
    const std::vector<std::array<int,3>> endIJK_vec = {{2,1,1}, {1,1,3}, {4,3,3}};
    disjointPatches_check(grid, startIJK_vec, endIJK_vec);
}

BOOST_AUTO_TEST_CASE(lgrs_disjointPatchesB)
{
    // Create a grid
    Dune::CpGrid grid;
    const std::array<double, 3> cell_sizes = {1.0, 1.0, 1.0};
    const std::array<int, 3> grid_dim = {4,3,3};
    grid.createCartesian(grid_dim, cell_sizes);
    const std::vector<std::array<int,3>> startIJK_vec = {{0,0,0}, {3,2,0}};
    const std::vector<std::array<int,3>> endIJK_vec = {{2,2,1}, {4,3,3}};
    disjointPatches_check(grid, startIJK_vec, endIJK_vec);
}

BOOST_AUTO_TEST_CASE(patches_share_corner)
{
    // Create a grid
    Dune::CpGrid grid;
    const std::array<double, 3> cell_sizes = {1.0, 1.0, 1.0};
    const std::array<int, 3> grid_dim = {4,3,3};
    grid.createCartesian(grid_dim, cell_sizes);
    const std::vector<std::array<int,3>> startIJK_vec = {{0,0,0}, {1,1,1}, {3,2,2}};
    const std::vector<std::array<int,3>> endIJK_vec = {{1,1,1}, {2,2,2}, {4,3,3}};
    disjointPatches_check(grid, startIJK_vec, endIJK_vec);
}

BOOST_AUTO_TEST_CASE(patches_share_corners)
{
    // Create a grid
    Dune::CpGrid grid;
    const std::array<double, 3> cell_sizes = {1.0, 1.0, 1.0};
    const std::array<int, 3> grid_dim = {4,3,3};
    grid.createCartesian(grid_dim, cell_sizes);
    const std::vector<std::array<int,3>> startIJK_vec = {{0,0,0}, {2,0,1}, {3,2,2}};
    const std::vector<std::array<int,3>> endIJK_vec = {{2,1,1}, {3,1,2}, {4,3,3}};
    disjointPatches_check(grid, startIJK_vec, endIJK_vec);
}

BOOST_AUTO_TEST_CASE(pathces_share_face)
{
    // Create a grid
    Dune::CpGrid grid;
    const std::array<double, 3> cell_sizes = {1.0, 1.0, 1.0};
    const std::array<int, 3> grid_dim = {4,3,3};
    grid.createCartesian(grid_dim, cell_sizes);
    const std::vector<std::array<int,3>> startIJK_vec = {{0,0,0}, {2,0,0}, {3,2,2}};
    const std::vector<std::array<int,3>> endIJK_vec = {{2,1,1}, {3,1,1}, {4,3,3}};
    disjointPatches_check(grid, startIJK_vec, endIJK_vec);
}

BOOST_AUTO_TEST_CASE(pathces_share_faceB)
{
    // Create a grid
    Dune::CpGrid grid;
    const std::array<double, 3> cell_sizes = {1.0, 1.0, 1.0};
    const std::array<int, 3> grid_dim = {4,3,3};
    grid.createCartesian(grid_dim, cell_sizes);
    const std::vector<std::array<int,3>> startIJK_vec = {{0,0,0}, {0,0,1}, {1,1,2}};
    const std::vector<std::array<int,3>> endIJK_vec = {{2,2,1}, {3,2,2}, {4,3,3}};
    disjointPatches_check(grid, startIJK_vec, endIJK_vec);
}

BOOST_AUTO_TEST_CASE(invalid_argument_sizes)
{
    // Create a grid
    Dune::CpGrid grid;
    const std::array<double, 3> cell_sizes = {1.0, 1.0, 1.0};
    const std::array<int, 3> grid_dim = {4,3,3};
    grid.createCartesian(grid_dim, cell_sizes);
    const std::vector<std::array<int,3>> startIJK_vec = {{0,0,0}, {0,0,1}, {1,1,2}};
    const std::vector<std::array<int,3>> endIJK_vec = {{2,2,1}, {3,2,1}};
    disjointPatches_check(grid, startIJK_vec, endIJK_vec);
}

BOOST_AUTO_TEST_CASE(invalid_argument_sizesB)
{
    // Create a grid
    Dune::CpGrid grid;
    const std::array<double, 3> cell_sizes = {1.0, 1.0, 1.0};
    const std::array<int, 3> grid_dim = {4,3,3};
    grid.createCartesian(grid_dim, cell_sizes);
    const std::vector<std::array<int,3>> startIJK_vec = {{0,0,0}, {0,0,2}, {3,2,2}};
    const std::vector<std::array<int,3>> endIJK_vec = {{2,1,1}, {1,1,3}, {4,3,3}, {2,1,1}};
    disjointPatches_check(grid, startIJK_vec, endIJK_vec);
}


BOOST_AUTO_TEST_CASE(disjoint_patches_C)
{
    // Create a grid
    Dune::CpGrid grid;
    const std::array<double, 3> cell_sizes = {1.0, 1.0, 1.0};
    const std::array<int, 3> grid_dim = {4,3,3};
    grid.createCartesian(grid_dim, cell_sizes);
    const std::vector<std::array<int,3>> startIJK_vec = {{2,2,2}, {0,0,0}};
    const std::vector<std::array<int,3>> endIJK_vec = {{3,3,3}, {4,1,1}};
    disjointPatches_check(grid, startIJK_vec, endIJK_vec);
}
