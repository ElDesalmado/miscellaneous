#pragma once

#include "graph_search/reactive.h"
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
    }   // namespace options

    constexpr options::dfs_t<impl_std> dfs_std{};
    constexpr options::bfs_t<impl_std> bfs_std{};

    namespace impl
    {
        template<typename GraphNodeT>
        struct find_path<GraphNodeT, options::dfs_t<impl_std>>
        {
            explicit find_path(options::dfs_t<impl_std>)   //
            {
            }

            // TODO: remove GraphT structure, use only nodes/vertices?
            template<template<typename...> class TOutputContainerT,
                     typename... M,
                     typename ReactiveT>
            TOutputContainerT<const GraphNodeT *, M...> operator()(const GraphNodeT &from,
                                                                   const GraphNodeT &to,
                                                                   const ReactiveT &reactive)
            {
                // TODO: this must not be a set or a map or any self-balancing tree/self-sorting
                //  container. THis check must be within a public API layer functions
                using out_type = TOutputContainerT<const GraphNodeT *, M...>;

                reactive::onStart(reactive);

                // parent stack is used to track current path (chain of nodes)
                std::stack<const GraphNodeT *> parentStack{};
                parentStack.template emplace(std::addressof(from));

                using const_iterator = std::decay_t<decltype(from.cbegin())>;
                std::stack<const_iterator> childrenStack{};
                childrenStack.template emplace(from.cbegin());

                while (!parentStack.empty())
                {
                    // TODO: throw exception?
                    if (reactive::abortRequested(reactive))
                        break;

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
                    reactive::processingNode(reactive, *nextNode);
                }

                out_type out{};
                out.reserve(parentStack.size());

                reactive::onFinished(reactive);

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
            explicit find_path(options::bfs_t<impl_std>)   //
            {
            }

            template<template<typename...> class TOutputContainerT,
                     typename... M,
                     typename ReactiveT>
            TOutputContainerT<const GraphNodeT*, M...> operator()(const GraphNodeT &from,
                                                           const GraphNodeT &to,
                                                           const ReactiveT &reactive)
            {
                using out_type = TOutputContainerT<const GraphNodeT *, M...>;
                reactive::onStart(reactive);

                std::stack<const GraphNodeT *> parentStack{};

                // stack for tracking processed children of current parent.
                std::stack<std::queue<const GraphNodeT *>> childrenStack{};
                //                childrenStack.template emplace();

                std::queue<const GraphNodeT*> nodeQueue{};
                nodeQueue.push(std::addressof(from));

                while (!nodeQueue.empty())
                {
                    // TODO: throw exception?
                    if (reactive::abortRequested(reactive))
                        break;

                    parentStack.template emplace(nodeQueue.front());
                    childrenStack.template emplace();

                    // parse children
                    bool finished = false;

                    auto iter = nodeQueue.front()->cbegin();
                    const auto iterEnd = nodeQueue.front()->cend();
                    while (iter != iterEnd)
                    {
                        // TODO: throw exception?
                        if (reactive::abortRequested(reactive))
                        {
                            finished = true;
                            break;
                        }

                        reactive::processingNode(reactive, *iter);

                        // if found
                        if (*iter == to)
                        {
                            parentStack.push(std::addressof(*iter));
                            finished = true;
                            break;
                        }

                        nodeQueue.push(std::addressof(*iter));
                        childrenStack.top().push(std::addressof(*iter));
                        ++iter;
                    }

                    if (finished)
                        break;

                    nodeQueue.pop();
                    if (!childrenStack.top().empty())
                        childrenStack.top().pop();

                    if (childrenStack.top().empty())
                    {
                        parentStack.pop();
                        childrenStack.pop();
                    }
                }

                out_type out{};
                out.reserve(parentStack.size());

                reactive::onFinished(reactive);

                while (!parentStack.empty())
                {
                    out.insert(out.begin(), std::move(parentStack.top()));
                    parentStack.pop();
                }

                return out;
            }
        };
    }   // namespace impl

}   // namespace eld
