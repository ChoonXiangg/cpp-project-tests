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
    SquareNode* node = grid.GetNode(3, 2);
    EXPECT_EQ(node->x_, 3);
    EXPECT_EQ(node->y_, 2);
}

// ========== Square Pathfinding Tests ==========

TEST(SquarePathfindingTest, FindsPathOnOpenGrid) {
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_FALSE(path.empty());
}

TEST(SquarePathfindingTest, PathStartsAtTarget) {
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), target);
}

TEST(SquarePathfindingTest, PathDoesNotIncludeStart) {
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    for (NodeBase* node : path)
        EXPECT_NE(node, start);
}

TEST(SquarePathfindingTest, PathLengthCorrectOnOpenGrid) {
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_EQ(path.size(), 8);
}

TEST(SquarePathfindingTest, NoPathWhenTargetWalledOff) {
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 4);

    grid.SetWall(3, 4);
    grid.SetWall(4, 3);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_TRUE(path.empty());
}

TEST(SquarePathfindingTest, NoPathWhenStartWalledOff) {
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 4);

    grid.SetWall(1, 0);
    grid.SetWall(0, 1);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_TRUE(path.empty());
}

TEST(SquarePathfindingTest, StartEqualsTargetReturnsEmpty) {
    SquareGrid grid(5, 5);
    SquareNode* node = grid.GetNode(2, 2);

    std::vector<NodeBase*> path = Pathfinding::FindPath(node, node);
    EXPECT_TRUE(path.empty());
}

TEST(SquarePathfindingTest, PathNavigatesAroundWall) {
    SquareGrid grid(3, 3);
    grid.SetWall(1, 1);

    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(2, 2);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    SquareNode* wall = grid.GetNode(1, 1);
    for (NodeBase* node : path)
        EXPECT_NE(node, wall);
}

TEST(SquarePathfindingTest, PathOnlyContainsWalkableNodes) {
    SquareGrid grid(5, 5);
    grid.SetWall(2, 0);
    grid.SetWall(2, 1);
    grid.SetWall(2, 2);

    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 0);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    for (NodeBase* node : path)
        EXPECT_TRUE(node->IsWalkable());
}

TEST(SquarePathfindingTest, AdjacentStartAndTarget) {
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(1, 0);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.size(), 1);
    EXPECT_EQ(path[0], target);
}

TEST(SquarePathfindingTest, NarrowCorridorPath) {
    SquareGrid grid(5, 1);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 0);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
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
    HexNode* node = grid.GetNode(3, 2);
    EXPECT_EQ(node->x_, 3);
    EXPECT_EQ(node->y_, 2);
}

TEST(HexGridTest, CenterNodeHasSixNeighbors) {
    HexGrid grid(5, 5);
    HexNode* center = grid.GetNode(2, 2);
    EXPECT_EQ(center->GetNeighbors().size(), 6);
}

TEST(HexGridTest, CornerNodeHasFewerNeighbors) {
    HexGrid grid(5, 5);
    HexNode* corner = grid.GetNode(0, 0);
    EXPECT_LT(corner->GetNeighbors().size(), 6);
}

// ========== Hex Pathfinding Tests ==========

TEST(HexPathfindingTest, FindsPathOnOpenGrid) {
    HexGrid grid(5, 5);
    HexNode* start = grid.GetNode(0, 0);
    HexNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_FALSE(path.empty());
}

TEST(HexPathfindingTest, PathStartsAtTarget) {
    HexGrid grid(5, 5);
    HexNode* start = grid.GetNode(0, 0);
    HexNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), target);
}

TEST(HexPathfindingTest, PathDoesNotIncludeStart) {
    HexGrid grid(5, 5);
    HexNode* start = grid.GetNode(0, 0);
    HexNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    for (NodeBase* node : path)
        EXPECT_NE(node, start);
}

TEST(HexPathfindingTest, HexPathShorterThanSquareDiagonal) {
    // Hex grids allow diagonal-like movement, so the path from
    // (0,0) to (4,4) should be shorter than on a square grid (8 steps)
    HexGrid hexGrid(5, 5);
    HexNode* hexStart = hexGrid.GetNode(0, 0);
    HexNode* hexTarget = hexGrid.GetNode(4, 4);
    std::vector<NodeBase*> hexPath = Pathfinding::FindPath(hexStart, hexTarget);

    SquareGrid sqGrid(5, 5);
    SquareNode* sqStart = sqGrid.GetNode(0, 0);
    SquareNode* sqTarget = sqGrid.GetNode(4, 4);
    std::vector<NodeBase*> sqPath = Pathfinding::FindPath(sqStart, sqTarget);

    ASSERT_FALSE(hexPath.empty());
    ASSERT_FALSE(sqPath.empty());
    EXPECT_LT(hexPath.size(), sqPath.size());
}

TEST(HexPathfindingTest, NoPathWhenTargetWalledOff) {
    HexGrid grid(5, 5);
    HexNode* start = grid.GetNode(0, 0);
    HexNode* target = grid.GetNode(4, 4);

    // Hex node at (4,4) is on an even row, so its neighbors are:
    // (3,4) (3,3) (4,3) — wall all of them plus the odd-row ones
    grid.SetWall(3, 4);
    grid.SetWall(3, 3);
    grid.SetWall(4, 3);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_TRUE(path.empty());
}

TEST(HexPathfindingTest, PathNavigatesAroundWall) {
    HexGrid grid(5, 5);
    grid.SetWall(2, 2);

    HexNode* start = grid.GetNode(0, 0);
    HexNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    HexNode* wall = grid.GetNode(2, 2);
    for (NodeBase* node : path)
        EXPECT_NE(node, wall);
}

TEST(HexPathfindingTest, PathOnlyContainsWalkableNodes) {
    HexGrid grid(5, 5);
    grid.SetWall(1, 1);
    grid.SetWall(2, 1);
    grid.SetWall(1, 2);

    HexNode* start = grid.GetNode(0, 0);
    HexNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    for (NodeBase* node : path)
        EXPECT_TRUE(node->IsWalkable());
}

TEST(HexPathfindingTest, AdjacentStartAndTarget) {
    HexGrid grid(5, 5);
    HexNode* start = grid.GetNode(2, 2);
    HexNode* target = grid.GetNode(3, 2);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.size(), 1);
    EXPECT_EQ(path[0], target);
}

TEST(HexPathfindingTest, StartEqualsTargetReturnsEmpty) {
    HexGrid grid(5, 5);
    HexNode* node = grid.GetNode(2, 2);

    std::vector<NodeBase*> path = Pathfinding::FindPath(node, node);
    EXPECT_TRUE(path.empty());
}