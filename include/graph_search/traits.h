#pragma once

#include <type_traits>

namespace eld
{
    // TODO: get allow_self_loop from options/mixins?
    namespace traits
    {
        /**
         * TODO:
         *  - is weighted
         *  - is bidirectional
         *  - can be self-looped
         */

        template <typename GraphNodeT>
        struct allowed_self_loop : std::false_type {};
    }
}
