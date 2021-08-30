#pragma once

#include <type_traits>

namespace eld
{
    namespace detail
    {
        template<typename...>
        using void_t = void;
    }

    namespace reactive
    {
        namespace detail
        {
            template<typename, typename = void>
            struct has_onStart : std::false_type
            {
            };

            template<typename T>
            struct has_onStart<T, eld::detail::void_t<decltype(std::declval<T>().onStart())>> :
              std::true_type
            {
            };

            template<typename ReactiveT>
            void onStart(const ReactiveT &, std::false_type /*has_onStart*/)
            {
                // do nothing
            }

            template<typename ReactiveT>
            void onStart(const ReactiveT &r, std::true_type /*has_onStart*/)
            {
                r.onStart();
            }

            template<typename, typename = void>
            struct has_onFinished : std::false_type
            {
            };

            template<typename T>
            struct has_onFinished<T,
                                  eld::detail::void_t<decltype(std::declval<T>().onFinished())>> :
              std::true_type
            {
            };

            template<typename ReactiveT>
            void onFinished(const ReactiveT &, std::false_type /*has_onFinished*/)
            {
                // do nothing
            }

            template<typename ReactiveT>
            void onFinished(const ReactiveT &r, std::true_type /*has_onFinished*/)
            {
                r.onFinished();
            }

            template<typename, typename = void>
            struct has_abortRequested : std::false_type
            {
            };

            template<typename T>
            struct has_abortRequested<
                T,
                eld::detail::void_t<decltype((bool)std::declval<T>().abortRequested())>> :
              std::true_type
            {
            };

            template<typename ReactiveT>
            bool abortRequested(const ReactiveT &, std::false_type /*has_abortRequested*/)
            {
                // do nothing
                return false;
            }

            template<typename ReactiveT>
            bool abortRequested(const ReactiveT &r, std::true_type /*has_abortRequested*/)
            {
                return r.abortRequested();
            }

            template<typename, typename, typename = void>
            struct has_processingNode : std::false_type
            {
            };

            template<typename T, typename GraphNodeT>
            struct has_processingNode<T,
                                      GraphNodeT,
                                      eld::detail::void_t<decltype(std::declval<T>().processingNode(
                                          std::declval<GraphNodeT>()))>> : std::true_type
            {
            };

            template<typename ReactiveT, typename GraphNodeT>
            void processingNode(const ReactiveT &,
                                const GraphNodeT &,
                                std::false_type /*has_processingNode*/)
            {
                // do nothing
            }

            template<typename ReactiveT, typename GraphNodeT>
            void processingNode(const ReactiveT &r,
                                const GraphNodeT &graphNode,
                                std::true_type /*has_processingNode*/)
            {
                return r.processingNode(graphNode);
            }

        }   // namespace detail

        template<typename ReactiveT>
        void onStart(const ReactiveT &r)
        {
            detail::onStart(r, detail::has_onStart<ReactiveT>());
        }

        template<typename ReactiveT>
        void onFinished(const ReactiveT &r)
        {
            detail::onFinished(r, detail::has_onFinished<ReactiveT>());
        }

        template<typename ReactiveT>
        bool abortRequested(const ReactiveT &r)
        {
            return detail::abortRequested(r, detail::has_abortRequested<ReactiveT>());
        }

        template<typename ReactiveT, typename GraphNodeT>
        void processingNode(const ReactiveT &reactive, const GraphNodeT &node)
        {
            detail::processingNode(reactive,
                                   node,
                                   detail::has_processingNode<ReactiveT, GraphNodeT>());
        }

        // TODO: make_reactive with designated callbacks
        namespace event_tag
        {
            struct onStart;
            struct onFinished;
            struct abortRequested;
            struct processingNode;
        }



    }   // namespace reactive

    struct no_reactive_t
    {
    } constexpr no_reactive{};
}   // namespace eld