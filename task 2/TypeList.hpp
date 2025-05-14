#ifndef TYPELIST_H
#define TYPELIST_H


namespace t2_inner {
    template <int N, class U, typename... Ts>
    struct size_;

    template <int N, class U=void, typename... Ts>
    struct size_ : size_<N + 1, Ts...> {};

    template <int N>
    struct size_<N> {
        static constexpr int value = N;
    };

    template <class OG, class T, typename... Ts>
    struct contains_: contains_<OG, Ts...> {};

    template <class OG, typename... Ts>
    struct contains_<OG, OG, Ts...> {
        static constexpr bool value = true;
    };

    template <class OG>
    struct contains_<OG, OG> {
        static constexpr bool value = true;
    };

    template <class OG, class T>
    struct contains_<OG, T> {
        static constexpr bool value = false;
    };

    template <int N, int N2, class OG, class T, typename... Ts>
    struct get_index_ : get_index_<N, N2 - 1, OG, Ts...> {};

    template <int N, class OG, class T, typename... Ts>
    struct get_index_<N, 0, OG, T, Ts...> : get_index_<N + 1, 0, OG, Ts...> {};

    template <int N, class OG, typename... Ts>
    struct get_index_<N, 0, OG, OG, Ts...> {
        static constexpr int value = N;
    };

    template <int N, class OG>
    struct get_index_<N, 0, OG, OG> {
        static constexpr int value = N;
    };

    template <int N, typename T, typename... Ts>
    struct get_type_ : get_type_<N - 1, Ts...> {};

    template <typename T, typename... Ts>
    struct get_type_<0, T, Ts...> {
        using type = T;
    };
} // namespace end


template<class... Ts>
struct TypeList {
    static constexpr int size = t2_inner::size_<0, Ts...>::value;

    template <class T>
    static constexpr bool contains = t2_inner::contains_<T, Ts...>::value;

    // equals to t1.size if no such type exists...
    template <class T, int StartFrom=0>
    static constexpr int get_index = t2_inner::get_index_<StartFrom, StartFrom, T, Ts...>::value;

    // template <int N>
    // static constexpr get_type_<N, Ts...> get_type_struct_{};
    // template <int N>
    // concept index_range_check = N < size && 0 <= N;

    template <int N>
    using type_by_index = typename t2_inner::get_type_<N, Ts...>::type;

    template <class T>
    static constexpr TypeList<T, Ts...> push_front{};

    template <class T>
    static constexpr TypeList<Ts..., T> push_back{};
};

#endif //TYPELIST_H
