#include <iostream>
#include <cmath>
#include <sstream>

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

        string str() {
            stringstream s;
            s << "sin(" << f1.str() << ")";
            return s.str();
        }

        string dx_str() {
            stringstream s;
            s << "(cos(" << f1.str() << ")*" << f1.dx_str() << ")";
            return s.str();
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

        string str() {
            stringstream s;
            s << "cos(" << f1.str() << ")";
            return s.str();
        }

        string dx_str() {
            stringstream s;
            s << "(-sin(" << f1.str() << ")*" << f1.dx_str() << ")";
            return s.str();
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

        string str() {
            return "(x)";
        }

        string dx_str() {
            return "(1)";
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

        string str() {
            ostringstream strs;
            strs << "(" << c << ")";
            return strs.str();
        }

        string dx_str() {
            return "(0)";
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

        string str() {
            stringstream s;
            s << "(" << f1.str() << "*" << f2.str() << ")";
            return s.str();
        }

        string dx_str() {
            stringstream s;
            s << "(" << f1.dx_str() << "*" << f2.str() << "+" << f1.str() << "*" << f2.dx_str() << ")";
            return s.str();
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

        string str() {
            stringstream s;
            s << "(" << f1.str() << "+" << f2.str() << ")";
            return s.str();
        }

        string dx_str() {
            stringstream s;
            s << "(" << f1.dx_str() << "+" << f2.dx_str() << ")";
            return s.str();
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

        string str() {
            stringstream s;
            s << "(" << f1.str() << "-" << f2.str() << ")";
            return s.str();
        }

        string dx_str() {
            stringstream s;
            s << "(" << f1.dx_str() << "-" << f2.dx_str() << ")";
            return s.str();
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

        string str() {
            stringstream s;
            s << "(" << f1.str() << "/" << f2.str() << ")";
            return s.str();
        }

        string dx_str() {
            stringstream s;
            s << "(" << f1.dx_str() << "*" << f2.str() << "-" << f1.str() << "*" << f2.dx_str() << ")/(" << f2.str() << "^2)";
            return s.str();
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

        string str() {
            stringstream s;
            s << f1.str() << "^" << to_string(n);
            return s.str();
        }

        string dx_str() {
            stringstream s;
            s << "(" << to_string(n) << "*" << f1.dx_str() << "*" << f1.str() << "^" << (n-1) << ")";
            return s.str();
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

        string str() {
            stringstream s;
            s << "exp(" << f1.str() << ")";
            return s.str();
        }

        string dx_str() {
            stringstream s;
            s << "(exp(" << f1.str() << ")*" << f1.dx_str() << ")";
            return s.str();
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

        string str() {
            stringstream s;
            s << "(log(" << f1.str() << "))";
            return s.str();
        }

        string dx_str() {
            stringstream s;
            s << "((1/" << f1.str() << ")*" << f1.dx_str() << ")";
            return s.str();
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
    // TESTE 2
    // f(x) = x*x
    auto f = x*x;

    cout << f.str() << endl;

    // [+]Test 2: f(x) = x*x 
    // [+]Test 4: f(x) = (x+7.0)*(x-7.0) 
    // [+]Test 5: f(x) = x->*3 
    // [+]Test 6: f(x) = (7.0*x+5.0)->*3 
    // [+]Test 7: f(x) = sin( x + 5 ) 
    // [+]Test 8: f(x) = 1.0 / (sin(x)->*2 + cos(x)->*2)->*4 
    // [+]Test 9: f(x) = exp( x * log( x - 8.0 ) + 1.0 ) 
    // [+]Test 10: f(x) = exp( (x + 1.0)*(x - 1.0) )->*2 
    // [+]Test 11: f(x) = log( x->* 4 + 8.0 ) * sin( cos( x / 3.14 ) ) 
    // [+]Test 12: f(x) = log( (8.1/(x+1.0)x->*3 - 9.2 ) * (x + 3) *sin( cos( x / 3.14 ) ) 

    return 0;
}
