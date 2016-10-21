#pragma once

#define STRINGIFY_DETAIL(x) #x
#define STRINGIFY(x) STRINGIFY_DETAIL(x)

namespace detail
{
    template<typename> struct SfinaeDecorate { using type = int; };

    struct TagSpecialMemberFunctionsNotFound {};
    struct TagSpecialMemberFunctionsDetected : TagSpecialMemberFunctionsNotFound {};
    struct TagSpecialMemberFunctionsRequired : TagSpecialMemberFunctionsDetected {};

    template <class TypeToCheckFor, class TypeInQuestion,
              typename SfinaeDecorate<decltype(&TypeInQuestion::get)>::type = 0,
              typename SfinaeDecorate<decltype(TypeToCheckFor::getStatic)>::type = 0>
    static bool isa (TypeInQuestion* ptr, TagSpecialMemberFunctionsDetected)
    {
        return ptr != nullptr && ptr->get() == TypeToCheckFor::getStatic();
    }

    template <class TypeToCheckFor, class TypeInQuestion>
    static bool isa (TypeInQuestion* ptr, TagSpecialMemberFunctionsNotFound)
    {
        __pragma(message(
            __FILE__ "(" STRINGIFY(__LINE__) "): "
            "Warning: for live builds on Windows your type needs a special macro!"));

        return nullptr;
    }
}

struct WindowsRtti
{
    template <class TypeToCheckFor, class TypeInQuestion>
    static bool isa (TypeInQuestion* ptr)
    {
        return detail::isa<TypeToCheckFor> (ptr, detail::TagSpecialMemberFunctionsRequired());
    }
};

#undef STRINGIFY
#undef STRINGIFY_DETAIL
