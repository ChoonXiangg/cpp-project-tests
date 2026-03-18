#include "pch.h"
#include <gtest/gtest.h>
#include "../cpp-project/SquareGrid.h"
#include "../cpp-project/SquareGrid.cpp"
#include "../cpp-project/SquareNode.h"
#include "../cpp-project/HexGrid.h"
#include "../cpp-project/HexGrid.cpp"
#include "../cpp-project/HexNode.h"
#include "../cpp-project/NodeBase.h"
#include "../cpp-project/Pathfinding.h"
#include "../cpp-project/Heuristic.h"

// ========== SquareGrid Tests ==========

TEST(SquareGridTest, CreatesCorrectDimensions) {
    SquareGrid grid(5, 3);
    EXPECT_EQ(grid.GetWidth(), 5);
    EXPECT_EQ(grid.GetHeight(), 3);
}

TEST(SquareGridTest, AllNodesWalkableByDefault) {
    SquareGrid grid(4, 4);
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            EXPECT_TRUE(grid.GetNode(x, y)->IsWalkable());
}

TEST(SquareGridTest, SetWallMakesNodeUnwalkable) {
    SquareGrid grid(5, 5);
    grid.SetWall(2, 2);
    EXPECT_FALSE(grid.GetNode(2, 2)->IsWalkable());
}

TEST(SquareGridTest, NodeHasCorrectCoordinates) {
    SquareGrid grid(5, 5);
    auto node = grid.GetNode(3, 2);
    EXPECT_EQ(node->x_, 3);
    EXPECT_EQ(node->y_, 2);
}

// ========== Square Pathfinding Tests ==========

TEST(SquarePathfindingTest, FindsPathOnOpenGrid) {
    SquareGrid grid(5, 5);
    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 4);

    auto path = Pathfinding::FindPath(start, target);
    EXPECT_FALSE(path.empty());
}

TEST(SquarePathfindingTest, PathStartsAtTarget) {
    SquareGrid grid(5, 5);
    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 4);

    auto path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), target);
}

TEST(SquarePathfindingTest, PathDoesNotIncludeStart) {
    SquareGrid grid(5, 5);
    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 4);

    auto path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    for (auto node : path)
        EXPECT_NE(node, start);
}

TEST(SquarePathfindingTest, PathLengthCorrectOnOpenGrid) {
    SquareGrid grid(5, 5);
    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 4);

    auto path = Pathfinding::FindPath(start, target);
    EXPECT_EQ(path.size(), 8);
}

TEST(SquarePathfindingTest, NoPathWhenTargetWalledOff) {
    SquareGrid grid(5, 5);
    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 4);

    grid.SetWall(3, 4);
    grid.SetWall(4, 3);

    auto path = Pathfinding::FindPath(start, target);
    EXPECT_TRUE(path.empty());
}

TEST(SquarePathfindingTest, NoPathWhenStartWalledOff) {
    SquareGrid grid(5, 5);
    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 4);

    grid.SetWall(1, 0);
    grid.SetWall(0, 1);

    auto path = Pathfinding::FindPath(start, target);
    EXPECT_TRUE(path.empty());
}

TEST(SquarePathfindingTest, StartEqualsTargetReturnsEmpty) {
    SquareGrid grid(5, 5);
    auto node = grid.GetNode(2, 2);

    auto path = Pathfinding::FindPath(node, node);
    EXPECT_TRUE(path.empty());
}

TEST(SquarePathfindingTest, PathNavigatesAroundWall) {
    SquareGrid grid(3, 3);
    grid.SetWall(1, 1);

    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(2, 2);

    auto path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    auto wall = grid.GetNode(1, 1);
    for (auto node : path)
        EXPECT_NE(node, wall);
}

TEST(SquarePathfindingTest, PathOnlyContainsWalkableNodes) {
    SquareGrid grid(5, 5);
    grid.SetWall(2, 0);
    grid.SetWall(2, 1);
    grid.SetWall(2, 2);

    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 0);

    auto path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    for (auto node : path)
        EXPECT_TRUE(node->IsWalkable());
}

TEST(SquarePathfindingTest, AdjacentStartAndTarget) {
    SquareGrid grid(5, 5);
    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(1, 0);

    auto path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.size(), 1);
    EXPECT_EQ(path[0], target);
}

TEST(SquarePathfindingTest, NarrowCorridorPath) {
    SquareGrid grid(5, 1);
    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 0);

    auto path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.size(), 4);
}

// ========== HexGrid Tests ==========

TEST(HexGridTest, CreatesCorrectDimensions) {
    HexGrid grid(6, 4);
    EXPECT_EQ(grid.GetWidth(), 6);
    EXPECT_EQ(grid.GetHeight(), 4);
}

TEST(HexGridTest, AllNodesWalkableByDefault) {
    HexGrid grid(4, 4);
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            EXPECT_TRUE(grid.GetNode(x, y)->IsWalkable());
}

TEST(HexGridTest, SetWallMakesNodeUnwalkable) {
    HexGrid grid(5, 5);
    grid.SetWall(2, 2);
    EXPECT_FALSE(grid.GetNode(2, 2)->IsWalkable());
}

TEST(HexGridTest, NodeHasCorrectCoordinates) {
    HexGrid grid(5, 5);
    auto node = grid.GetNode(3, 2);
    EXPECT_EQ(node->x_, 3);
    EXPECT_EQ(node->y_, 2);
}

TEST(HexGridTest, CenterNodeHasSixNeighbors) {
    HexGrid grid(5, 5);
    auto center = grid.GetNode(2, 2);
    EXPECT_EQ(center->GetNeighbors().size(), 6);
}

TEST(HexGridTest, CornerNodeHasFewerNeighbors) {
    HexGrid grid(5, 5);
    auto corner = grid.GetNode(0, 0);
    EXPECT_LT(corner->GetNeighbors().size(), 6);
}

// ========== Hex Pathfinding Tests ==========

TEST(HexPathfindingTest, FindsPathOnOpenGrid) {
    HexGrid grid(5, 5);
    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 4);

    auto path = Pathfinding::FindPath(start, target);
    EXPECT_FALSE(path.empty());
}

TEST(HexPathfindingTest, PathStartsAtTarget) {
    HexGrid grid(5, 5);
    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 4);

    auto path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), target);
}

TEST(HexPathfindingTest, PathDoesNotIncludeStart) {
    HexGrid grid(5, 5);
    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 4);

    auto path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    for (auto node : path)
        EXPECT_NE(node, start);
}

TEST(HexPathfindingTest, HexPathShorterThanSquareDiagonal) {
    HexGrid hexGrid(5, 5);
    auto hexStart = hexGrid.GetNode(0, 0);
    auto hexTarget = hexGrid.GetNode(4, 4);
    auto hexPath = Pathfinding::FindPath(hexStart, hexTarget);

    SquareGrid sqGrid(5, 5);
    auto sqStart = sqGrid.GetNode(0, 0);
    auto sqTarget = sqGrid.GetNode(4, 4);
    auto sqPath = Pathfinding::FindPath(sqStart, sqTarget);

    ASSERT_FALSE(hexPath.empty());
    ASSERT_FALSE(sqPath.empty());
    EXPECT_LT(hexPath.size(), sqPath.size());
}

TEST(HexPathfindingTest, NoPathWhenTargetWalledOff) {
    HexGrid grid(5, 5);
    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 4);

    grid.SetWall(3, 4);
    grid.SetWall(3, 3);
    grid.SetWall(4, 3);

    auto path = Pathfinding::FindPath(start, target);
    EXPECT_TRUE(path.empty());
}

TEST(HexPathfindingTest, PathNavigatesAroundWall) {
    HexGrid grid(5, 5);
    grid.SetWall(2, 2);

    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 4);

    auto path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    auto wall = grid.GetNode(2, 2);
    for (auto node : path)
        EXPECT_NE(node, wall);
}

TEST(HexPathfindingTest, PathOnlyContainsWalkableNodes) {
    HexGrid grid(5, 5);
    grid.SetWall(1, 1);
    grid.SetWall(2, 1);
    grid.SetWall(1, 2);

    auto start = grid.GetNode(0, 0);
    auto target = grid.GetNode(4, 4);

    auto path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    for (auto node : path)
        EXPECT_TRUE(node->IsWalkable());
}

TEST(HexPathfindingTest, AdjacentStartAndTarget) {
    HexGrid grid(5, 5);
    auto start = grid.GetNode(2, 2);
    auto target = grid.GetNode(3, 2);

    auto path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.size(), 1);
    EXPECT_EQ(path[0], target);
}

TEST(HexPathfindingTest, StartEqualsTargetReturnsEmpty) {
    HexGrid grid(5, 5);
    auto node = grid.GetNode(2, 2);

    auto path = Pathfinding::FindPath(node, node);
    EXPECT_TRUE(path.empty());
}

// ========== Heuristic Comparison Tests ==========

TEST(HeuristicTest, SquareDefaultIsManhattan) {
    SquareGrid grid(5, 5);
    EXPECT_EQ(grid.GetNode(0, 0)->GetHeuristic(), Heuristic::Manhattan);
}

TEST(HeuristicTest, SquareCanSetEuclidean) {
    SquareGrid grid(5, 5, Heuristic::Euclidean);
    EXPECT_EQ(grid.GetNode(0, 0)->GetHeuristic(), Heuristic::Euclidean);
}

TEST(HeuristicTest, HexDefaultIsManhattan) {
    HexGrid grid(5, 5);
    EXPECT_EQ(grid.GetNode(0, 0)->GetHeuristic(), Heuristic::Manhattan);
}

TEST(HeuristicTest, HexCanSetEuclidean) {
    HexGrid grid(5, 5, Heuristic::Euclidean);
    EXPECT_EQ(grid.GetNode(0, 0)->GetHeuristic(), Heuristic::Euclidean);
}

TEST(HeuristicTest, BothHeuristicsFindPathOnOpenSquareGrid) {
    SquareGrid manhattan(5, 5, Heuristic::Manhattan);
    auto mPath = Pathfinding::FindPath(manhattan.GetNode(0, 0), manhattan.GetNode(4, 4));

    SquareGrid euclidean(5, 5, Heuristic::Euclidean);
    auto ePath = Pathfinding::FindPath(euclidean.GetNode(0, 0), euclidean.GetNode(4, 4));

    EXPECT_FALSE(mPath.empty());
    EXPECT_FALSE(ePath.empty());
}

TEST(HeuristicTest, BothHeuristicsFindPathOnOpenHexGrid) {
    HexGrid manhattan(5, 5, Heuristic::Manhattan);
    auto mPath = Pathfinding::FindPath(manhattan.GetNode(0, 0), manhattan.GetNode(4, 4));

    HexGrid euclidean(5, 5, Heuristic::Euclidean);
    auto ePath = Pathfinding::FindPath(euclidean.GetNode(0, 0), euclidean.GetNode(4, 4));

    EXPECT_FALSE(mPath.empty());
    EXPECT_FALSE(ePath.empty());
}

TEST(HeuristicTest, SquareOpenGridSameOptimalLength) {
    SquareGrid manhattan(5, 5, Heuristic::Manhattan);
    auto mPath = Pathfinding::FindPath(manhattan.GetNode(0, 0), manhattan.GetNode(4, 4));

    SquareGrid euclidean(5, 5, Heuristic::Euclidean);
    auto ePath = Pathfinding::FindPath(euclidean.GetNode(0, 0), euclidean.GetNode(4, 4));

    ASSERT_FALSE(mPath.empty());
    ASSERT_FALSE(ePath.empty());
    EXPECT_EQ(mPath.size(), ePath.size());
}

TEST(HeuristicTest, HexOpenGridSameOptimalLength) {
    HexGrid manhattan(5, 5, Heuristic::Manhattan);
    auto mPath = Pathfinding::FindPath(manhattan.GetNode(0, 0), manhattan.GetNode(4, 4));

    HexGrid euclidean(5, 5, Heuristic::Euclidean);
    auto ePath = Pathfinding::FindPath(euclidean.GetNode(0, 0), euclidean.GetNode(4, 4));

    ASSERT_FALSE(mPath.empty());
    ASSERT_FALSE(ePath.empty());
    EXPECT_EQ(mPath.size(), ePath.size());
}

TEST(HeuristicTest, SquareWalledGridSameOptimalLength) {
    SquareGrid manhattan(5, 5, Heuristic::Manhattan);
    manhattan.SetWall(2, 0);
    manhattan.SetWall(2, 1);
    manhattan.SetWall(2, 2);
    auto mPath = Pathfinding::FindPath(manhattan.GetNode(0, 0), manhattan.GetNode(4, 0));

    SquareGrid euclidean(5, 5, Heuristic::Euclidean);
    euclidean.SetWall(2, 0);
    euclidean.SetWall(2, 1);
    euclidean.SetWall(2, 2);
    auto ePath = Pathfinding::FindPath(euclidean.GetNode(0, 0), euclidean.GetNode(4, 0));

    ASSERT_FALSE(mPath.empty());
    ASSERT_FALSE(ePath.empty());
    EXPECT_EQ(mPath.size(), ePath.size());
}

TEST(HeuristicTest, HexWalledGridSameOptimalLength) {
    HexGrid manhattan(5, 5, Heuristic::Manhattan);
    manhattan.SetWall(2, 2);
    manhattan.SetWall(2, 1);
    manhattan.SetWall(1, 2);
    auto mPath = Pathfinding::FindPath(manhattan.GetNode(0, 0), manhattan.GetNode(4, 4));

    HexGrid euclidean(5, 5, Heuristic::Euclidean);
    euclidean.SetWall(2, 2);
    euclidean.SetWall(2, 1);
    euclidean.SetWall(1, 2);
    auto ePath = Pathfinding::FindPath(euclidean.GetNode(0, 0), euclidean.GetNode(4, 4));

    ASSERT_FALSE(mPath.empty());
    ASSERT_FALSE(ePath.empty());
    EXPECT_EQ(mPath.size(), ePath.size());
}

TEST(HeuristicTest, BothHeuristicsReturnEmptyWhenNoPath) {
    SquareGrid manhattan(5, 5, Heuristic::Manhattan);
    manhattan.SetWall(1, 0);
    manhattan.SetWall(0, 1);
    auto mPath = Pathfinding::FindPath(manhattan.GetNode(0, 0), manhattan.GetNode(4, 4));

    SquareGrid euclidean(5, 5, Heuristic::Euclidean);
    euclidean.SetWall(1, 0);
    euclidean.SetWall(0, 1);
    auto ePath = Pathfinding::FindPath(euclidean.GetNode(0, 0), euclidean.GetNode(4, 4));

    EXPECT_TRUE(mPath.empty());
    EXPECT_TRUE(ePath.empty());
}

// ========== Weighted Terrain Tests ==========

TEST(WeightTest, DefaultWeightIsOne) {
    SquareGrid grid(5, 5);
    EXPECT_DOUBLE_EQ(grid.GetNode(0, 0)->GetWeight(), 1.0);
}

TEST(WeightTest, CanSetWeight) {
    SquareGrid grid(5, 5);
    grid.SetWeight(2, 2, 5.0);
    EXPECT_DOUBLE_EQ(grid.GetNode(2, 2)->GetWeight(), 5.0);
}

TEST(WeightTest, HexCanSetWeight) {
    HexGrid grid(5, 5);
    grid.SetWeight(2, 2, 3.0);
    EXPECT_DOUBLE_EQ(grid.GetNode(2, 2)->GetWeight(), 3.0);
}

TEST(WeightTest, PathAvoidsHighWeightCells) {
    // 3x1 grid: (0,0) -> (1,0) -> (2,0)
    // Make middle cell very expensive
    // Compare with unweighted: path should still go through it
    // since it's the only route in a 3x1 grid
    // Use a wider grid to give an alternative route
    //
    // 5x3 grid, start (0,1) to (4,1)
    // Weight the direct path (1,1)(2,1)(3,1) at 9 each
    // A* should route around via row 0 or row 2 instead
    SquareGrid weighted(5, 3);
    weighted.SetWeight(1, 1, 9);
    weighted.SetWeight(2, 1, 9);
    weighted.SetWeight(3, 1, 9);
    auto wPath = Pathfinding::FindPath(weighted.GetNode(0, 1), weighted.GetNode(4, 1));

    SquareGrid unweighted(5, 3);
    auto uPath = Pathfinding::FindPath(unweighted.GetNode(0, 1), unweighted.GetNode(4, 1));

    ASSERT_FALSE(wPath.empty());
    ASSERT_FALSE(uPath.empty());

    // Weighted path should take more steps (longer route to avoid cost)
    EXPECT_GT(wPath.size(), uPath.size());
}

TEST(WeightTest, PathStillFindsRouteWithWeights) {
    SquareGrid grid(5, 5);
    grid.SetWeight(1, 0, 5);
    grid.SetWeight(2, 0, 5);
    grid.SetWeight(3, 0, 5);

    auto path = Pathfinding::FindPath(grid.GetNode(0, 0), grid.GetNode(4, 0));
    EXPECT_FALSE(path.empty());
}

TEST(WeightTest, UniformWeightSameAsUnweighted) {
    // All weight-1 grid should produce same path as default
    SquareGrid grid(5, 5);
    for (int y = 0; y < 5; y++)
        for (int x = 0; x < 5; x++)
            grid.SetWeight(x, y, 1.0);

    auto path = Pathfinding::FindPath(grid.GetNode(0, 0), grid.GetNode(4, 4));
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.size(), 8);
}

TEST(WeightTest, HexPathAvoidsHighWeightCells) {
    HexGrid weighted(5, 3);
    weighted.SetWeight(1, 1, 9);
    weighted.SetWeight(2, 1, 9);
    weighted.SetWeight(3, 1, 9);
    auto wPath = Pathfinding::FindPath(weighted.GetNode(0, 1), weighted.GetNode(4, 1));

    HexGrid unweighted(5, 3);
    auto uPath = Pathfinding::FindPath(unweighted.GetNode(0, 1), unweighted.GetNode(4, 1));

    ASSERT_FALSE(wPath.empty());
    ASSERT_FALSE(uPath.empty());

    // Weighted path takes a longer route to avoid expensive cells
    EXPECT_GT(wPath.size(), uPath.size());
}

TEST(WeightTest, WeightDoesNotAffectWalls) {
    SquareGrid grid(5, 5);
    grid.SetWall(1, 0);
    grid.SetWall(0, 1);
    // Weight a wall — shouldn't matter, path is still blocked
    grid.SetWeight(1, 0, 1.0);

    auto path = Pathfinding::FindPath(grid.GetNode(0, 0), grid.GetNode(4, 4));
    EXPECT_TRUE(path.empty());
}