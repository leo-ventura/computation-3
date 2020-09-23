#include <iostream>
#include <cmath>

#define x X()

using namespace std;

// Função seno
template <typename F1>
class seno {
    public:
        seno(F1 f1): f1(f1) {}

        double operator () (double v) {
            return std::sin(f1(v));
        }

        double e(double v) {
            return std::sin(f1(v));
        }

        double dx(double v) {
            return std::cos(f1(v)) * f1.dx(v);
        }

    private:
        F1 f1;
};

// Função cosseno
template <typename F1>
class cosseno {
    public:
        cosseno(F1 f1): f1(f1) {}

        double operator () (double v) {
            return std::cos(f1(v));
        }

        double e(double v) {
            return std::cos(f1(v));
        }

        double dx(double v) {
            return -std::sin(f1(v)) * f1.dx(v);
        }

    private:
        F1 f1;
};

class X {
    public:
        X() {}

        double operator()( double v ) {
            return v;
        }

        double dx( double v ) {
            return 1;
        }
};

class Cte {
    public:
        Cte( double c ): c(c) {}

        double operator()( double v ) {
            return c;
        }

        double dx( double v ) {
            return 0;
        }

    private:
        double c;
};

// Trigonométricas
template <typename F1>
cosseno<F1> cos(F1 f1) {
    return cosseno<F1>(f1);
}

template <typename F1>
seno<F1> sin(F1 f1) {
    return seno<F1>(f1);
}

// Multiplicação
template <typename F1, typename F2>
class Multiplica {
    public:
        Multiplica( F1 f1, F2 f2 ): f1(f1), f2(f2) {}

        double operator()( double v ) {
            return f1(v) * f2(v);
        }

        double e(double v) {
            return f1(v) * f2(v);
        }

        double dx( double v ) {
            return f1(v)*f2.dx(v) + f1.dx(v)*f2(v);
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

        double operator()( double v ) {
            return f1(v) + f2(v);
        }

        double e(double v) {
            return f1(v) + f2(v);
        }

        double dx( double v ) {
            return f1.dx(v) + f2.dx(v);
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

        double operator()( double v ) {
            return f1(v) - f2(v);
        }

        double e(double v) {
            return f1(v) - f2(v);
        }

        double dx( double v ) {
            return f1.dx(v) - f2.dx(v);
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

        double operator()( double v ) {
            return f1(v) / f2(v);
        }

        double e(double v) {
            return f1(v) / f2(v);
        }

        double dx( double v ) {
            return (f1.dx(v) * f2(v) - f1(v) * f2.dx(v)) / (f2(v) * f2(v));
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

        double operator()( double v ) {
            return std::pow(f1(v), n);
        }

        double e(double v) {
            return std::pow(f1(v), n);
        }

        double dx( double v ) {
            return std::pow(f1(v), n-1) * n * f1.dx(v);
        }

    private:
        F1 f1;
        int n;
};

// Exponencial
template <typename F1>
class Exp {
    public:
        Exp( F1 f1): f1(f1) {}

        double operator()( double v ) {
            return std::exp(f1(v));
        }

        double e(double v) {
            return std::exp(f1(v));
        }

        double dx( double v ) {
            return std::exp(f1(v)) * f1.dx(v);
        }

    private:
        F1 f1;
};

// Logaritmo
template <typename F1>
class Log {
    public:
        Log( F1 f1): f1(f1) {}

        double operator()( double v ) {
            return std::log(f1(v));
        }

        double e(double v) {
            return std::log(f1(v));
        }

        double dx( double v ) {
            return 1 / f1(v);
        }

    private:
        F1 f1;
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

// Potenciação
template <typename F1>
Pot<F1> operator ->* (F1 f1, int n) {
    return Pot<F1> (f1, n);
}

// Exponenciação
template <typename F1>
Exp<F1> exp(F1 f1) {
    return Exp<F1>(f1);
}

// Logaritmo
template <typename F1>
Log<F1> log(F1 f1) {
    return Log<F1>(f1);
}

// Erro
template <typename F1, typename F2>
Pot<F1> operator ->*(F1 f1, F2 f2) {
  static_assert(is_same<F2,int>::value, "Operador de potenciação definido apenas para inteiros");
  return Pot<F1>(f1, f2);
}


int main() {
    // TESTE 1
    // double v = 5.1;
    // auto f = 3.0 * x * x;
    // f(5.1)=78.03, f'(5.1)=30.6

    // cout << "f(" << v << ")=" << f.e( v ) << ", f'(" << v << ")=" << f.dx( v ) << endl;

    // TESTE 2
    // double v = 1.1;
    // auto f = x*x*( x + 8.0 ) + x;

    // // f(1.1)=12.111, f'(1.1)=22.23
    // cout << "f(" << v << ")=" << f.e( v ) << ", f'(" << v << ")=" << f.dx( v ) << endl;

    // TESTE 3
    // double v = 0.5;
    // auto f = sin( x * x * cos( 3.14 * x + 1.0 ) );

    // // f(0.5)=-0.208714, f'(0.5)=-1.23783
    // cout << "f(" << v << ")=" << f.e( v ) << ", f'(" << v << ")=" << f.dx( v ) << endl;

    // TESTE 4
    // double v = 3.14;
    // auto f = sin( x * x - cos( 3.14 * x + 1.0 ) );

    // // f(3.14)=-0.53995, f'(3.14)=-2.66732
    // cout << "f(" << v << ")=" << f.e( v ) << ", f'(" << v << ")=" << f.dx( v ) << endl;

    // TESTE 5
    // double v = 3.14159;
    // auto f = sin(x)/cos(x);

    // // f(3.14159)=-2.65359e-06, f'(3.14159)=1
    // cout << "f(" << v << ")=" << f.e( v ) << ", f'(" << v << ")=" << f.dx( v ) << endl;

    // TESTE 6
    // double v = -7.3;
    // auto f = x->*3 + x->*2;

    // // f(-7.3)=-335.727, f'(-7.3)=145.27
    // cout << "f(" << v << ")=" << f.e( v ) << ", f'(" << v << ")=" << f.dx( v ) << endl;

    // TESTE 7
    // double v = -0.5;
    // auto f = 1.0 / (sin(x)->*2 + cos(x)->*2)->*4;


    // // f(-0.5)=1, f'(-0.5)=0
    // cout << "f(" << v << ")=" << f.e( v ) << ", f'(" << v << ")=" << f.dx( v ) << endl;

    // TESTE 8
    // double v = 0.1;
    // auto f = 1 / (1 + exp( -2*( x - 1 )->*4 ) );

    // // f(0.1)=0.787881, f'(0.1)=0.97467
    // cout << "f(" << v << ")=" << f.e( v ) << ", f'(" << v << ")=" << f.dx( v ) << endl;

    // TESTE 9
    // double v = 3;
    // auto f = exp( x * log( x ) );

    // // f(3)=27, f'(3)=56.6625
    // cout << "f(" << v << ")=" << f.e( v ) << ", f'(" << v << ")=" << f.dx( v ) << endl;

    // TESTE 10
    double v = 3;
    auto f =  2 * x->*1.1;

    // Operador de potenciação definido apenas para inteiros
// Essa nota será dada manualmente - gere um erro estático. Exemplo:
// In file included from teste.gerado.cc:7:0:
// derivador.cc: In instantiation of ‘constexpr auto operator->*(F, G) [with F = <lambda(double)>; G = double]’:
// teste.gerado.cc:14:19:   required from here
// derivador.cc:68: error: static assertion failed: Operador de potenciação definido apenas para inteiros
//          static_assert( !is_same_v< double, G >, "Operador de potenciação definido apenas para inteiros" );
//          ^~~~~~~~~~~~~

}