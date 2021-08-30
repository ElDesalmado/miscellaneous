
#include "graph_search/find_path.h"
#include "graph_search/impl/find_path_std.h"


#include <cstddef>
#include <vector>

#include <iostream>

struct Node
{
    size_t value;
    std::vector<Node> children;

    auto cbegin() const { return children.cbegin(); }

    auto cend() const { return children.cend(); }
};

constexpr inline bool operator==(const Node& lhv, const Node &rhv)
{
    return lhv.value == rhv.value;
}

struct graph{};


int main(int, char **)
{
    Node root{ 4,
               { Node{ 8 },                            //
                 Node{ 15,                             //
                       { Node{ 23 }, Node{ 42 } } },   //
                 Node{ 16 } } };


    auto vecRes = eld::find_path<std::vector>(root, Node{42}, eld::dfs_std);

    std::vector<Node *> path{};

    return 0;
}