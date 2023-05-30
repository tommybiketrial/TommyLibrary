#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <thread>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <stdexcept>
#include <set>
#include <string>
#include <sstream>
#include <iomanip>
#include <climits>
#include <fstream>
#include <map>
using namespace std;

struct Node {
    int x, y;
    int g, h, f;
    Node* parent;

    Node(int x_, int y_, int g_ = 0, int h_ = 0, int f_ = 0, Node* parent_ = nullptr)
        : x(x_), y(y_), g(g_), h(h_), f(f_), parent(parent_) {}

    bool operator<(const Node& other) const {
        return f > other.f;
    }
};

int heuristic(const Node& node, const Node& goal) {
    return abs(node.x - goal.x) + abs(node.y - goal.y);
}

vector<pair<int, int>> a_star(int sx, int sy, int gx, int gy, vector<vector<int>>& grid) {
    // Define start and goal nodes
    Node start(sx, sy);
    Node goal(gx, gy);

    // Create open and closed sets
    priority_queue<Node> open;
    unordered_set<Node*> closed;

    // Add starting node to open set
    start.g = 0;
    start.h = heuristic(start, goal);
    start.f = start.g + start.h;
    open.push(start);

    while (!open.empty()) {
        // Get node with lowest f score
        Node current = open.top();
        open.pop();

        // If goal node, return path
        if (current.x == goal.x && current.y == goal.y) {
            vector<pair<int, int>> path;
            Node* node = &current;
            while (node) {
                path.push_back({ node->x, node->y });
                node = node->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // Add current node to closed set
        closed.insert(&current);

        // Generate children nodes and update their scores
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int nx = current.x + dx;
                int ny = current.y + dy;
                if (dx == 0 && dy == 0) continue;
                if (nx < 0 || ny < 0 || nx >= grid.size() || ny >= grid[0].size() || grid[nx][ny] == 1) continue;

                Node* child = new Node(nx, ny);
                auto child_ptr = closed.find(child);
                if (child_ptr != closed.end()) {
                    delete child;
                    continue;
                }

                child->g = current.g + 1;
                child->h = heuristic(*child, goal);
                child->f = child->g + child->h;
                child->parent = &current;

                auto dup_it = find_if(begin(open), end(open), [&](const Node& n) { return n.f == child->f && n.x == child->x && n.y == child->y; });
                if (dup_it != end(open)) {
                    if (!(dup_it->f <= child->f)) {
                        dup_it->g = child->g;
                        dup_it->parent = child->parent;
                    }
                }
                else {
                    open.push(*child);
                }
            }
        }
    }
}

int main() {
    vector<vector<int>> grid = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
    };
    auto path = a_star(0, 0, 4, 4, grid);
    for (auto p : path) {
        cout << "(" << p.first << "," << p.second << ") ";
    }
    cout << endl;
    return 0;
}