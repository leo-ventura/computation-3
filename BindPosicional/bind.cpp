#include <tuple>
#include <type_traits>
#include <functional>
#include <iostream>
#include <utility>

// tuples: https://www.geeksforgeeks.org/tuples-in-c/

#define compareWithPlaceHolder(t) std::is_same_v<PlaceHolder,decltype(t)>

class PlaceHolder {};

PlaceHolder __;


// implementação do stackoverflow para reverter uma tupla
template<typename T, typename TT = typename std::remove_reference<T>::type, size_t... I>
auto reverse_impl(T&& t, std::index_sequence<I...>) -> std::tuple<typename std::tuple_element<sizeof...(I) - 1 - I, TT>::type...> {
    return std::make_tuple(std::get<sizeof...(I) - 1 - I>(std::forward<T>(t))...);
}

template<typename T, typename TT = typename std::remove_reference<T>::type>
auto reverse(T&& t) -> decltype(reverse_impl(std::forward<T>(t), std::make_index_sequence<std::tuple_size<TT>::value>())) {
    return reverse_impl(std::forward<T>(t), std::make_index_sequence<std::tuple_size<TT>::value>());
}

// usando a ideia dita em sala para separar em head e tail...
template <typename ...T>
inline constexpr auto head(std::tuple<T...> t) {
    if constexpr (sizeof...(T) > 0) {
        return std::get<0>(t);
    } else {}
}

// implementação de tail feita em sala
template <typename T, size_t... indices>
inline constexpr auto tail_impl(T tp, std::index_sequence<indices...>) {
    return std::tuple{ std::get<indices+1>(tp)... };
}

template <typename ...T>
inline constexpr auto tail(std::tuple<T...> tp) {
    return tail_impl(tp, std::make_index_sequence<sizeof...(T)-1>());
}

// função para substituir os PlaceHolders do final
template <typename ...T>
inline constexpr auto replacePlaceHolderAtEnd(std::tuple<T...> tp) {
    auto reversed = reverse(tp);
    if constexpr (compareWithPlaceHolder(head(reversed))) {
        return replacePlaceHolderAtEnd(reverse(tail(reversed)));
    } else {
        return tp;
    }
}

// função para substituir os PlaceHolders no meio,
// substituindo-os pelos valores em t2
template<typename ...T1, typename ...T2>
auto replacePlaceHolder(std::tuple<T1...> t1, std::tuple<T2...> t2) {
    if constexpr (sizeof...(T1) == 0 && sizeof...(T2) != 0) {
        return t2;
    } else if constexpr(sizeof...(T1) != 0 && sizeof...(T2) == 0) {
        return t1;
    } else if constexpr (compareWithPlaceHolder(head(t1))) {
        // assumindo que t2 nao possui placeholder
        return std::tuple_cat(std::tuple{head(t2)}, replacePlaceHolder(tail(t1),tail(t2)));
    } else {
        return std::tuple_cat(std::tuple{head(t1)}, replacePlaceHolder(tail(t1), t2));
    }
}

template <typename F, typename ...Args1>
class Bind;

template <typename F, typename ...Args1>
auto bind( F f, Args1... args1 ) {
    return Bind{ f, replacePlaceHolderAtEnd(std::tuple{args1...}) };
}

template <typename F, typename ...T>
auto bind(F f, std::tuple<T...> args1) {
    return Bind{f, replacePlaceHolderAtEnd(args1)};
}

template <typename F, typename ...T>
inline constexpr auto apply(F f, std::tuple<T...> args) {
    if constexpr (std::is_invocable_v<F,T...>) {
        return std::apply(f, args);
    } else {
        return ::bind(f, args);
    }
}

template <typename F, typename ...Args1>
class Bind {
    public:
        Bind( F f, std::tuple<Args1...> args1 ): f(f), args1(args1) {}

        template <typename ...Args2>
        auto operator()( Args2... args2 ) {
            auto args = replacePlaceHolder(args1, std::tuple{args2...});
            return apply(f, args);
        }

    private:
        F f;
        std::tuple<Args1...> args1;
};

int mdc(int a, int b) { 
    while(b != 0) {
        int _a = a;
        a = b;
        b = _a % b;
    }
    return a;
}

int main() {
    // teste 1
    // output: 2 3 4 1 6 1 4 3 2 1 12
    // auto f2 = ::bind( mdc, __ );
    // auto f1 = ::bind( f2, __, 12 );
    // for( int i = 2; i <= 12; i++ )
    //     std::cout << f1( i ) <<  " ";

    // teste 2
    // output: 2 3 2 1 6 1 2 9 2 1 6 1 2 3 2 1 18
    // auto f2 = ::bind( mdc );
    // auto f1 = f2( 18, __ );
    // for( int i = 2; i <= 18; i++ )
    //     std::cout << f1( i ) <<  " ";


    // teste 3
    // output: 0, 10, 20, 30, 40,
    // auto f1 =  []( int x, int y, int z ){ return std::to_string(x*y) + (char) z ; };
    // auto f2 = ::bind(f1, __, __, ',' );
    // for( int i = 0; i < 5; i++ )
        // std::cout << f2( i, 10 ) << " ";
    // auto f2 = ::bind( []( int x, int y, int z ){ std::cout << x*y << (char) z << " " ; }, __, __, ',' );
    // for( int i = 0; i < 5; i++ )
    //     f2( i, 10 );

    // teste 4
    // output: 0; 5; 10; 15; 20;
    // auto f3 = ::bind( []( int x, int y, int z ){ std::cout << x*z << (char) y << " " ; } );
    // auto f1 = f3( __, ';' );
    // for( int i = 0; i < 5; i++ )
    //     f1( 5 )( i );

    // teste 5
    // output: 6 6 12 30 6 42 24 18 30 66 12
    // MMC mmc;
    // auto f1 = ::bind( mmc, __, 6, __ );
    // for( int i = 2; i <= 12; i++ )
    //     cout << f1( i ) << " ";


    // teste 6
    // output: 6 6 12 30 6 42 24 18 30 66 12
    // MMC mmc;
    // auto f2 = ::bind( mmc );
    // auto f1 = f2( __, 6 );
    // for( int i = 2; i <= 12; i++ )
    //     cout << f1( i ) << " ";

    // teste 7
    // output: 0 1 2 6 9 @ a b c d e s
    // 0 1 2 6 9 a b c d e s ~
    // auto f1 = ::bind( ordena, "a", "b", __, "6", __, "1", "0", "c", __, __, "e" );
    // auto f2 = f1( "9", __, "2", "d", "s" );
    // cout << f2( "@" ) << endl;
    // cout << f2( "~" );

    // teste 8
    // output: ba
    // 7.4
    // auto f2=::bind( BarraPesada(), __, (string) "a" );
    // cout << f2( (string) "b" ) << endl;
    // cout << ::bind( BarraPesada(), __, 3.1)( 4.3 );


    // teste 9
    // output: 73
    // 75
    // BarraPesada barraMaisPesada;
    // auto f=::bind( &BarraPesada::foo, __, 7, __ );
    // cout << f( barraMaisPesada )( 3 ) << endl;
    // auto ff = f( __, 5 );
    // cout << ff( barraMaisPesada );

    // teste 10
    // output: 22.14
    // auto f1 = ::bind( []( auto... args ){ return (args + ... + 0 ); }, __, __, 10 );
    // auto f3 = f1( 9, 3.14 );
    // cout << f3 << endl;
}