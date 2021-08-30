#pragma once

#include "find_path/reactive.h"

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

    namespace impl
    {
        // TODO: options with mixins?
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
    }   // namespace impl

    template<template<typename...> class TOutputContainer,
             typename... M,
             typename GraphNodeT,
             typename OptionsT,
             typename ReactiveT = no_reactive_t>
    constexpr TOutputContainer<const GraphNodeT *, M...> find_path(
        const GraphNodeT &from,
        const GraphNodeT &to,
        const OptionsT &options,
        const ReactiveT &reactive = no_reactive)
    {
        return impl::make_find_path<GraphNodeT>(options)
            .template operator()<TOutputContainer, M...>(from, to, reactive);
    }

    // TODO: do I need this?
    //    template<template<typename...> class TOutputContainer,
    //             typename... M,
    //             typename GraphNodeT>
    //    constexpr TOutputContainer<GraphNodeT *, M...> find_path(const GraphNodeT &from,
    //                                                             const GraphNodeT &to)
    //    {
    //        return find_path<TOutputContainer, M...>(from, to, no_options);
    //    }

}   // namespace eld