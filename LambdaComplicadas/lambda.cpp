#include <iostream>
#include <type_traits>
#include <functional>
#include <vector>

using namespace std;

// class I {
//   public:
//     I(int i): i(i) {}

//     template <typename V>
//     auto operator()( V v ) const {
//       return v[i];
//     }

//   private:
//     int i;
// };

template <typename Functor>
class Lambda {
public:
  Lambda( Functor f ): f(f) {}

  template <typename V>
  invoke_result_t< Functor, V > operator()( V v ) const {
    return f( v );
  }

  template <typename I>
  auto operator [] (I i) {
    // return I(i);
    return Lambda {
      [i] (auto v) -> decltype(v[0]) { return v[i]; }
    };
  }

private:
  Functor f;
};

auto aux = []( auto v ){ return v; };

Lambda x{aux};

template <typename A, typename B>
auto operator != (Lambda<A> i, B b) {
  return Lambda {
    [i,b](auto v) { return i(v) != b; }
  };
}

template <typename A, typename B>
auto operator |(Lambda<A> a, B b) {
  return Lambda {
    [a,b](auto v) -> decltype(a(v) | b) { return a(v) | b; }
  };
}

template <typename A>
auto operator << ( ostream& o, Lambda<A> opr ) { 
 return Lambda{ [&o,opr]( auto v ) -> decltype( o << opr(v) )
                { return o << opr(v); } };
}

#define DEFINE_LAMBDA( opr )                                           \
template <typename A, typename B>                                      \
auto operator opr ( Lambda<A> a, Lambda<B> b ) {                       \
  return Lambda{ [a,b]( auto v ) -> decltype( a(v) opr b(v) )          \
                 { return a(v) opr b(v); } };                          \
}                                                                      \
                                                                       \
template <typename A, typename B>                                      \
auto operator opr ( Lambda<A> a, B b ) {                               \
  return Lambda{ [a,b]( auto v ) -> decltype( a(v) opr b    )          \
                 { return a(v) opr b; } };                             \
}                                                                      \
                                                                       \
template <typename A, typename B>                                      \
auto operator opr ( A a, Lambda<B> b ) {                               \
 return Lambda{ [a,b]( auto v ) -> decltype( a opr b(v)    )           \
                { return a opr b(v); } };                              \
}                                                                      \

DEFINE_LAMBDA( + )
DEFINE_LAMBDA( - )
DEFINE_LAMBDA( * )
DEFINE_LAMBDA( / )
DEFINE_LAMBDA( % )
DEFINE_LAMBDA( == )
// DEFINE_LAMBDA( != )
DEFINE_LAMBDA( << )

template<typename T, typename F>
auto operator | ( const T& iterable, F function ) {
    if constexpr ( is_same_v< bool, invoke_result_t< F, decltype( *begin(iterable) ) > > ) {
        vector< decay_t< decltype( *begin(iterable) ) > > result;

        for( auto element : iterable ) 
            if( invoke( function, element ) )
                result.push_back( element );
        
        return result;
    }
    else if constexpr ( is_same_v< void, invoke_result_t< F, decltype( *begin(iterable) ) > > ||
                        is_reference_v< invoke_result_t< F, decltype( *begin(iterable) ) > > ) {
        for( auto element : iterable ) 
            invoke( function, element );
    }
    else {
        std::vector< decay_t< invoke_result_t< F, decltype( *begin(iterable) ) > > > result;

        for( auto element : iterable ) 
            result.push_back( invoke( function, element ) );
        
        return result;
    }
}


int main() {
  // Test 1:
  std::cout << "Teste 1:\t";
  // std::vector<int> l{ 1, 0, 2, 0, 3 };
  // l | []( auto v ) { return v != 0; } | std::cout << x << ' ';
  std::cout << "\n\n";

  // Test 2:
  std::cout << "Teste 2:\t";
  // std::vector<std::vector<int>> m{ { 1, 2, 3 }, { 0, 3, 7 }, { 1, 3 } };
  // m | []( auto v ) { return v[0] != 0; } | (x | std::cout << x << ' ');
  std::cout << "\n\n";

  // Test 3:
  std::cout << "Teste 3:\t";
  std::vector<std::vector<int>> n{ { 1, 2, 3 }, { 0, 3, 7 }, { 1, 3 } };
  n | x[0] != 0 | ( x | std::cout << x << ' ' );
  std::cout << "\n\n";

  // Test 4:
  std::cout << "Teste 4:\t";
  // std::vector<std::vector<int>> o{ { 1, 2, 3 }, { 0, 3, 7 }, { 1, 3 } };
  // o | x[0] != 0 | (( x + 7 )| std::cout << x << ' ' );
  std::cout << "\n\n";
}