#include "pch.h"
#include <gtest/gtest.h>
#include "../cpp-project/SquareGrid.h"
#include "../cpp-project/SquareGrid.cpp"
#include "../cpp-project/SquareNode.h"
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

// ========== Pathfinding Tests ==========
// Note: FindPath returns the path from target back to (but NOT including) start.

TEST(PathfindingTest, FindsPathOnOpenGrid) {
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_FALSE(path.empty());
}

TEST(PathfindingTest, PathStartsAtTarget) {
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), target);
}

TEST(PathfindingTest, PathDoesNotIncludeStart) {
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    for (NodeBase* node : path) {
        EXPECT_NE(node, start);
    }
}

TEST(PathfindingTest, PathLengthCorrectOnOpenGrid) {
    // Manhattan distance from (0,0) to (4,4) = 8 steps
    // FindPath excludes start, so path size = 8
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_EQ(path.size(), 8);
}

TEST(PathfindingTest, NoPathWhenTargetWalledOff) {
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 4);

    grid.SetWall(3, 4);
    grid.SetWall(4, 3);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_TRUE(path.empty());
}

TEST(PathfindingTest, NoPathWhenStartWalledOff) {
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 4);

    grid.SetWall(1, 0);
    grid.SetWall(0, 1);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_TRUE(path.empty());
}

TEST(PathfindingTest, StartEqualsTargetReturnsEmpty) {
    SquareGrid grid(5, 5);
    SquareNode* node = grid.GetNode(2, 2);

    std::vector<NodeBase*> path = Pathfinding::FindPath(node, node);
    EXPECT_TRUE(path.empty());
}

TEST(PathfindingTest, PathNavigatesAroundWall) {
    SquareGrid grid(3, 3);
    grid.SetWall(1, 1);

    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(2, 2);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    SquareNode* wall = grid.GetNode(1, 1);
    for (NodeBase* node : path) {
        EXPECT_NE(node, wall);
    }
}

TEST(PathfindingTest, PathOnlyContainsWalkableNodes) {
    SquareGrid grid(5, 5);
    grid.SetWall(2, 0);
    grid.SetWall(2, 1);
    grid.SetWall(2, 2);

    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 0);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    for (NodeBase* node : path) {
        EXPECT_TRUE(node->IsWalkable());
    }
}

TEST(PathfindingTest, AdjacentStartAndTarget) {
    SquareGrid grid(5, 5);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(1, 0);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.size(), 1);
    EXPECT_EQ(path[0], target);
}

TEST(PathfindingTest, NarrowCorridorPath) {
    SquareGrid grid(5, 1);
    SquareNode* start = grid.GetNode(0, 0);
    SquareNode* target = grid.GetNode(4, 0);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.size(), 4);
}