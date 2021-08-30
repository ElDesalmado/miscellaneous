#pragma once

#include <type_traits>

namespace eld
{
    namespace detail
    {
        template<typename...>
        using void_t = void;
    }

    struct no_options_t
    {
    } constexpr no_options{};

    struct no_policy_t
    {
    } constexpr no_policy{};

    namespace impl
    {
        template<typename GraphNodeT, typename OptionsT>
        struct find_path;

        template<typename GraphNodeT, typename OptionsT>
        constexpr auto make_find_path(const OptionsT &options)
        {
            return find_path<GraphNodeT, OptionsT>(options);
        }

        template<typename GraphNodeT>
        constexpr auto make_find_path(const no_options_t &)
        {
            return find_path<GraphNodeT, no_options_t>();
        }

    }

    template<template<typename...> class TOutputContainer,
             typename... M,
             typename GraphNodeT,
             typename OptionsT,
             typename GraphT>
    constexpr TOutputContainer<const GraphNodeT*, M...> find_path(const GraphNodeT &from,
                                                             const GraphNodeT &to,
                                                             const GraphT &graph,
                                                             const OptionsT &options)
    {
        return impl::make_find_path<GraphNodeT>(options)
            .template operator()<TOutputContainer, M...>(from, to, graph);
    }

    template<template<typename...> class TOutputContainer,
             typename... M,
             typename GraphNodeT,
             typename GraphT>
    constexpr TOutputContainer<GraphNodeT *, M...> find_path(const GraphNodeT &from,
                                                             const GraphNodeT &to,
                                                             const GraphT &graph)
    {
        return find_path<TOutputContainer, M...>(from, to, graph, no_options);
    }

}   // namespace eld