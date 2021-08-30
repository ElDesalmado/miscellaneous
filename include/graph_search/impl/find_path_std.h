#pragma once

#include "graph_search/traits.h"

#include <memory>
#include <queue>
#include <stack>

namespace eld
{
    struct impl_std
    {
    };

    namespace options
    {
        template<typename...>
        struct dfs_t;

        template<typename... M>
        struct dfs_t<impl_std, M...>
        {
        };

        template<typename...>
        struct bfs_t;

        template<typename... M>
        struct bfs_t<impl_std, M...>
        {
        };
    }

    constexpr options::dfs_t<impl_std> dfs_std{};
    constexpr options::bfs_t<impl_std> bfs_std{};

    namespace impl
    {
        template<typename GraphNodeT>
        struct find_path<GraphNodeT, options::dfs_t<impl_std>>
        {
            explicit find_path(options::dfs_t<impl_std>) {}

            // TODO: remove GraphT structure, use only nodes/vertices?
            template<template<typename...> class TOutputContainerT, typename... M>
            TOutputContainerT<const GraphNodeT *, M...> operator()(const GraphNodeT &from,
                                                                   const GraphNodeT &to)
            {
                // TODO: this must not be a set or a map or any self-balancing tree/self-sorting
                //  container
                using out_type = TOutputContainerT<const GraphNodeT *, M...>;

                // parent stack is used to track current path (chain of nodes)
                std::stack<const GraphNodeT *> parentStack{};
                parentStack.template emplace(std::addressof(from));

                using const_iterator = std::decay_t<decltype(from.cbegin())>;
                std::stack<const_iterator> childrenStack{};
                childrenStack.template emplace(from.cbegin());

                while (!parentStack.empty())
                {
                    // break when found
                    if (*parentStack.top() == to)
                        break;

                    if (childrenStack.top() == parentStack.top()->cend())
                    {
                        parentStack.pop();
                        childrenStack.pop();
                        continue;
                    }

                    const GraphNodeT *nextNode = std::addressof(*(childrenStack.top()++));
                    parentStack.template emplace(nextNode);
                    childrenStack.template emplace(nextNode->cbegin());
                }

                out_type out{};
                out.reserve(parentStack.size());

                while (!parentStack.empty())
                {
                    out.insert(out.begin(), std::move(parentStack.top()));
                    parentStack.pop();
                }

                return out;
            }
        };

        template<typename GraphNodeT>
        struct find_path<GraphNodeT, options::bfs_t<impl_std>>
        {
            template<template<typename...> class TOutputContainerT, typename... M, typename GraphT>
            TOutputContainerT<GraphNodeT, M...> operator()(const GraphNodeT &from,
                                                           const GraphNodeT &to,
                                                           const GraphT &)
            {
                using out_type = TOutputContainerT<const GraphNodeT *, M...>;
                if (traits::allowed_self_loop<GraphNodeT>() && from == to)
                {
                    out_type out{};
                    out.push_back(std::addressof(from));
                    return out;
                }

                // TODO: implement
                return {};
            }
        };

        //
        //        template<typename GraphNodeT>
        //        struct find_path<GraphNodeT, options::dfs_t<std>>
        //        {
        //            using graph_node_type = GraphNodeT;
        //
        //            template<template<typename...> class TContainerOutT, typename... T>
        //            bool operator()(const graph_node_type &root,
        //                            TContainerOutT<graph_node_type *, T...> &containerOut)
        //            {
        //                // TODO: implement
        //                return false;
        //            }
        //        };
        //
        //        template<typename GraphNodeT>
        //        struct find_path<GraphNodeT, options::bfs_t<std>>
        //        {
        //            using graph_node_type = GraphNodeT;
        //
        //            template<template<typename...> class TContainerOutT, typename... T>
        //            bool operator()(const graph_node_type &root,
        //                            TContainerOutT<graph_node_type *, T...> &containerOut)
        //            {
        //                // TODO: implement
        //                return false;
        //            }
        //        };
    }

}
