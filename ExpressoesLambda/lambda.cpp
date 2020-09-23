#include <iostream>
#include <cmath>
#include <vector>
#include <iterator>
#include <type_traits>
#include <functional>

using namespace std;

class X {
    public:
        X() {}

        double operator()( double v ) {
            return v;
        }
};

// template <typename T = double>// #define x X()

class Cte {
    public:
        Cte( double c ): c(c) {}

        template <typename V>
        double operator()( V v ) {
            return c;
        }

    private:
        double c;
};

// Multiplicação
template <typename F1, typename F2>
class Multiplica {
    public:
        Multiplica( F1 f1, F2 f2 ): f1(f1), f2(f2) {}

        template <typename V>
        auto operator()( V v ) {
            return f1(v) * f2(v);
        }

    private:
        F1 f1;
        F2 f2;
};

// Adição
template <typename F1, typename F2>
class Adicao {
    public:
        Adicao( F1 f1, F2 f2 ): f1(f1), f2(f2) {}

        template <typename V>
        auto operator () (V v) {
            return f1(v) + f2(v);
        }

    private:
        F1 f1;
        F2 f2;
};

// Subtração
template <typename F1, typename F2>
class Sub {
    public:
        Sub( F1 f1, F2 f2 ): f1(f1), f2(f2) {}

        template <typename V>
        auto operator()( V v ) {
            return f1(v) - f2(v);
        }

    private:
        F1 f1;
        F2 f2;
};

// Divisão
template <typename F1, typename F2>
class Div {
    public:
        Div( F1 f1, F2 f2 ): f1(f1), f2(f2) {}

        template <typename V>
        auto operator()( V v ) {
            return f1(v) / f2(v);
        }

    private:
        F1 f1;
        F2 f2;
};

// Potenciação
template <typename F1>
class Pot {
    public:
        Pot( F1 f1, int n ): f1(f1), n(n) {}

        template <typename V>
        auto operator()( V v ) {
            return std::pow(f1(v), n);
        }

    private:
        F1 f1;
        int n;
};

template <typename F1>
class Print {
    public:
        Print(ostream& o, F1 f1, bool b): stream(o.rdbuf()), f1(f1), hasEndl(b) {}

        template <typename V>
        void operator () (V v) {
            if constexpr (is_same_v<F1, char>) {
                // using std::operator <<;
                // stream << f1;
            } else {
                stream << f1(v);
                if(hasEndl)
                    stream << '\n';
            }
        }

        void addEndl() {
            hasEndl = true;
        }

    // private:
    public:
        ostream stream;
        F1 f1;
        bool hasEndl = false;
};

// Multiplicação
template <typename F1, typename F2> 
Multiplica<F1,F2> operator * ( F1 f1, F2 f2 ) {
    return Multiplica<F1,F2>( f1, f2 );
}

template <typename F2> 
Multiplica<Cte,F2> operator * ( double n, F2 f2 ) {
    return Multiplica<Cte,F2>( n, f2 );
}

template <typename F1> 
Multiplica<F1,Cte> operator * ( F1 f1, int n ) {
    return Multiplica<F1,Cte>( f1, double(n) );
}

template <typename F2> 
Multiplica<Cte,F2> operator * ( int n, F2 f2 ) {
    return Multiplica<Cte,F2>( double(n), f2 );
}

template <typename F1> 
Multiplica<F1,Cte> operator * ( F1 f1, double n ) {
    return Multiplica<F1,Cte>( f1, n );
}

// Adição
template <typename F1, typename F2>
Adicao<F1,F2> operator + (F1 f1, F2 f2) {
    return Adicao<F1,F2> (f1, f2);
}

template <typename F1>
Adicao<F1,Cte> operator + (F1 f1, double n) {
    return Adicao<F1,Cte> (f1, n);
}

template <typename F2>
Adicao<Cte,F2> operator + (double n, F2 f2) {
    return Adicao<Cte,F2> (n, f2);
}

template <typename F2>
Adicao<Cte,F2> operator + (int n, F2 f2) {
    return Adicao<Cte,F2> (double(n), f2);
}

// Subtração
template <typename F1, typename F2>
Sub<F1,F2> operator - (F1 f1, F2 f2) {
    return Sub<F1,F2> (f1, f2);
}

template <typename F1>
Sub<F1,Cte> operator - (F1 f1, double n) {
    return Sub<F1,Cte> (f1, n);
}

template <typename F1>
Sub<F1,Cte> operator - (F1 f1, int n) {
    return Sub<F1,Cte> (f1, double(n));
}

template <typename F2>
Sub<Cte,F2> operator - (double n, F2 f2) {
    return Sub<Cte,F2> (n, f2);
}

// Divisão
template <typename F1, typename F2>
Div<F1,F2> operator / (F1 f1, F2 f2) {
    return Div<F1,F2> (f1, f2);
}

template <typename F1>
Div<F1,Cte> operator / (F1 f1, double n) {
    return Div<F1,Cte> (f1, n);
}

template <typename F2>
Div<Cte,F2> operator / (double n, F2 f2) {
    return Div<Cte,F2> (n, f2);
}

template <typename F2>
Div<Cte,F2> operator / (int n, F2 f2) {
    return Div<Cte,F2> (double(n), f2);
}

// Redefining print
template <typename F1>
Print<F1> operator << (ostream& o, F1 f) {
    return Print<F1>(o, f, false);
}

template <typename F1, typename F2>
Print<F2> operator << (Print<F2> p, F1 f) {
    return Print<F2>(p.stream, p.f1, true);
}

template<typename T, typename F>
auto operator | ( const T& iterable, F function ) {
    if constexpr ( is_same_v< bool, invoke_result_t< F, decltype( *begin(iterable) ) > > ) {
        vector< decay_t< decltype( *begin(iterable) ) > > result;

        for( auto element : iterable ) 
            if( invoke( function, element ) )
                result.push_back( element );
        
        return result;
    }
    else if constexpr ( is_same_v< void, invoke_result_t< F, decltype( *begin(iterable) ) > > ) {
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

X x;

int main() {
    // X x; // Declarar em algum lugar global.
    int tab[10] =  { 1, 2, 3, 2, 3, 4, 6, 0, 1, 8 };
    vector<int> v;
    tab | cout << x*x << '\n';
    // tab | cout << x*x;
    // tab | [ &v ]( int x ) { v.push_back( x ); };
    // tab | [] (auto x) { cout << x << endl;};
    // v | x % 2 == 0 | cout << x*x + 1 << '\n';
    // v | x % 2 == 1 | x*x | cout << x << '\n';
}