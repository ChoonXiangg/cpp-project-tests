#include "pch.h"
#include <gtest/gtest.h>
#include "../cpp-project/Grid.h"
#include "../cpp-project/Grid.cpp"
#include "../cpp-project/GridNode.h"
#include "../cpp-project/NodeBase.h"
#include "../cpp-project/Pathfinding.h"

// ========== Grid Tests ==========

TEST(GridTest, CreatesCorrectDimensions) {
    Grid grid(5, 3);
    EXPECT_EQ(grid.GetWidth(), 5);
    EXPECT_EQ(grid.GetHeight(), 3);
}

TEST(GridTest, AllNodesWalkableByDefault) {
    Grid grid(4, 4);
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            EXPECT_TRUE(grid.GetNode(x, y)->IsWalkable());
}

TEST(GridTest, SetWallMakesNodeUnwalkable) {
    Grid grid(5, 5);
    grid.SetWall(2, 2);
    EXPECT_FALSE(grid.GetNode(2, 2)->IsWalkable());
}

TEST(GridTest, NodeHasCorrectCoordinates) {
    Grid grid(5, 5);
    GridNode* node = grid.GetNode(3, 2);
    EXPECT_EQ(node->x_, 3);
    EXPECT_EQ(node->y_, 2);
}

// ========== Pathfinding Tests ==========
// Note: FindPath returns the path from target back to (but NOT including) start.
// So for a path from (0,0) to (4,4), the returned vector contains
// target -> ... -> first step, without the start node.

TEST(PathfindingTest, FindsPathOnOpenGrid) {
    Grid grid(5, 5);
    GridNode* start = grid.GetNode(0, 0);
    GridNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_FALSE(path.empty());
}

TEST(PathfindingTest, PathStartsAtTarget) {
    Grid grid(5, 5);
    GridNode* start = grid.GetNode(0, 0);
    GridNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), target);
}

TEST(PathfindingTest, PathDoesNotIncludeStart) {
    Grid grid(5, 5);
    GridNode* start = grid.GetNode(0, 0);
    GridNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    for (NodeBase* node : path) {
        EXPECT_NE(node, start);
    }
}

TEST(PathfindingTest, PathLengthCorrectOnOpenGrid) {
    // Manhattan distance from (0,0) to (4,4) = 8 steps
    // FindPath excludes start, so path size = 8
    Grid grid(5, 5);
    GridNode* start = grid.GetNode(0, 0);
    GridNode* target = grid.GetNode(4, 4);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_EQ(path.size(), 8);
}

TEST(PathfindingTest, NoPathWhenTargetWalledOff) {
    Grid grid(5, 5);
    GridNode* start = grid.GetNode(0, 0);
    GridNode* target = grid.GetNode(4, 4);

    // Wall off the target completely
    grid.SetWall(3, 4);
    grid.SetWall(4, 3);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_TRUE(path.empty());
}

TEST(PathfindingTest, NoPathWhenStartWalledOff) {
    Grid grid(5, 5);
    GridNode* start = grid.GetNode(0, 0);
    GridNode* target = grid.GetNode(4, 4);

    // Wall off the start completely
    grid.SetWall(1, 0);
    grid.SetWall(0, 1);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    EXPECT_TRUE(path.empty());
}

TEST(PathfindingTest, StartEqualsTargetReturnsEmpty) {
    // FindPath's backtrack loop exits immediately when start == target,
    // returning an empty path (already at destination)
    Grid grid(5, 5);
    GridNode* node = grid.GetNode(2, 2);

    std::vector<NodeBase*> path = Pathfinding::FindPath(node, node);
    EXPECT_TRUE(path.empty());
}

TEST(PathfindingTest, PathNavigatesAroundWall) {
    // 3x3 grid with a wall in the middle
    Grid grid(3, 3);
    grid.SetWall(1, 1);

    GridNode* start = grid.GetNode(0, 0);
    GridNode* target = grid.GetNode(2, 2);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    // Verify no node in the path is the wall
    GridNode* wall = grid.GetNode(1, 1);
    for (NodeBase* node : path) {
        EXPECT_NE(node, wall);
    }
}

TEST(PathfindingTest, PathOnlyContainsWalkableNodes) {
    Grid grid(5, 5);
    grid.SetWall(2, 0);
    grid.SetWall(2, 1);
    grid.SetWall(2, 2);

    GridNode* start = grid.GetNode(0, 0);
    GridNode* target = grid.GetNode(4, 0);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());

    for (NodeBase* node : path) {
        EXPECT_TRUE(node->IsWalkable());
    }
}

TEST(PathfindingTest, AdjacentStartAndTarget) {
    // One step away, path excludes start so size = 1
    Grid grid(5, 5);
    GridNode* start = grid.GetNode(0, 0);
    GridNode* target = grid.GetNode(1, 0);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.size(), 1);
    EXPECT_EQ(path[0], target);
}

TEST(PathfindingTest, NarrowCorridorPath) {
    // 5x1 grid: path excludes start so size = 4
    Grid grid(5, 1);
    GridNode* start = grid.GetNode(0, 0);
    GridNode* target = grid.GetNode(4, 0);

    std::vector<NodeBase*> path = Pathfinding::FindPath(start, target);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.size(), 4);
}