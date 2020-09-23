#include <vector>
#include <iterator>
#include <type_traits>
#include <functional>
#include <iostream>

// #define INT_MAX 2147483647
#define INT_MAX 100

int nPrimeiros = INT_MAX;

template<typename T, typename F>
auto operator | ( const T& iterable, F function ) {
    if constexpr ( std::is_same_v< bool, std::invoke_result_t< F, decltype( *std::begin(iterable) ) > > ) {
        std::vector< std::decay_t< decltype( *std::begin(iterable) ) > > result;

        for( auto element : iterable ) 
            if( std::invoke( function, element ) )
                result.push_back( element );
        
        return result;
    }
    else if constexpr ( std::is_same_v< void, std::invoke_result_t< F, decltype( *std::begin(iterable) ) > > ) {
        for( auto element : iterable ) 
            std::invoke( function, element );
    }
    else {
        std::vector< std::decay_t< std::invoke_result_t< F, decltype( *std::begin(iterable) ) > > > result;

        for( auto element : iterable ) {
            if(nPrimeiros-->0)
                result.push_back(std::invoke(function, element));
            else break;
        }

        // resetando nPrimeiros
        nPrimeiros = INT_MAX;
        return result;
    }
}

// https://stackoverflow.com/questions/37031805/preparation-for-stditerator-being-deprecated/38103394#38103394
template<int FROM, int TO = INT_MAX>
class Range {
public:
    class iterator {
        long num = FROM;
    public:
        iterator(long _num = 0) : num(_num) {}
        iterator& operator++() {num = TO >= FROM ? num + 1: num - 1; return *this;}
        iterator operator++(int) {iterator retval = *this; ++(*this); return retval;}
        bool operator==(iterator other) const {return num == other.num;}
        bool operator!=(iterator other) const {return !(*this == other);}
        long operator*() {return num;}
        // iterator traits
        using difference_type = long;
        using value_type = long;
        using pointer = const long*;
        using reference = const long&;
        using iterator_category = std::forward_iterator_tag;
    };
    iterator begin() const {return FROM;}
    iterator end() const {return TO >= FROM? TO+1 : TO-1;}
};

// usando macros para fazer todas as resoluções
// em tempo de compilação
#define RangeFrom(from) Range<from>()
#define RangeFromTo(from, to) Range<from, to-1>()

#define GET_MACRO(_1,_2,NAME,...) NAME
#define Intervalo(...) GET_MACRO(__VA_ARGS__, RangeFromTo, RangeFrom)(__VA_ARGS__)

class NPrimeiros {
    public:
        NPrimeiros(int n) {
            nPrimeiros = n;
        }

        template <typename T>
        auto operator () (T t) {
            return t;
        }
};

// aqui pra baixo não é pra enviar
// #include <iostream>

struct Impares {
  bool operator()( int n ) {
    return n % 2 == 0;
  }
};

int main() {

    // teste 1
    // auto v = vector{ 0, 2, 4, 6, 8, 10 };
    // for( auto x : v | []( auto x ){ return x + 1; } )
    //     cout << x << ";";
    


    // teste 2
    // Intervalo( 9, 12 )  | []( auto x ) { std::cout << x << " "; };
    // => 9 10 11 12

    // teste 3
    // Intervalo( 9, 20 ) | Impares() | []( auto x ) { std::cout << x << " "; };
    // => 10, 12, 14, 16, 18, 20

  // Test 6: Intervalo infinito e NPrimeiros
//   auto filtro1 = []( int n ) { return n % 3 == 0; };
//   auto filtro2 = []( int n ) { return n % 2 == 0; };
//   auto st = Intervalo( 0 ) | filtro1 | filtro2;
//   st | NPrimeiros( 10 ) | [](int x){ return x/10.0; } | []( auto x ){ std::cout << x << ";"; };
  //=> 0;0.6;1.2;1.8;2.4;3;3.6;4.2;4.8;5.4;

  // Test 8: Strings e vector
//   auto toString = []( int n ){ char buf[20]; sprintf( buf, "%d", n ); return std::string( buf ); };
//   std::vector<std::string> v;
//   Intervalo( 1 ) | Impares() | NPrimeiros( 10 ) | toString | [&v]( auto x ){ v.push_back( x ); };
//   v | []( auto x ){ std::cout << x + "0" << ";"; };
  //=> 20;40;60;80;100;120;140;160;180;200;

  // Test 9: Arrays 
//   std::string _st[] = { "a", "e", "i", "o", "u" };
//   _st | NPrimeiros( 3 ) | []( auto x ){ std::cout << x + "!" << ";"; };
  //=> a!;e!;i!;
}