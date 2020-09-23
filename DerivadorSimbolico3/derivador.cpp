#include <iostream>
#include <cmath>
#include <sstream>
#include <map>

// #define x X()

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
            if(f1.dx_str() != "1")
                s << "cos(" << f1.str() << ")*" << f1.dx_str();
            else
                s << "cos(" << f1.str() << ")";
            return s.str();
        }

    private:
        F1 f1;
    public:
        unsigned priority = 0;
        string signal = "sin";
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
            if(f1.dx_str() != "1")
                s << "-sin(" << f1.str() << ")*" << f1.dx_str();
            else
                s << "-sin(" << f1.str() << ")";
            return s.str();
        }

    private:
        F1 f1;
    public:
        string signal = "cos";
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
            return "x";
        }

        string dx_str() {
            return "1";
        }

        string signal = "x";
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
            strs<< c;
            return strs.str();
        }

        string dx_str() {
            return "0";
        }

    private:
        double c;
    public:
        string signal = "k";
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
            stringstream s1;
            stringstream s2;
            stringstream s;
            if(f1.signal == "+" || f1.signal == "-")
                s1 << "(" << f1.str() << ")";
            else
                s1 << f1.str();
            if(f2.signal == "+" || f2.signal == "-")
                s2 << "(" << f2.str() << ")";
            else
                s2 << f2.str();
            s << s1.str() << "*" << s2.str();
            return s.str();
        }

        string dx_str() {
            stringstream s;
            stringstream s1;
            stringstream s2;
            if(f1.dx_str() == "1" && f2.dx_str() == "1") {
                s << "(" << f2.str() << ")+(" << f1.str() << ")";
                return s.str();
            }
            if(f1.dx_str() == "1")
                s1 << f2.str();
            else if(f1.dx_str() != "0")
                s1 << f1.dx_str() << "*" << f2.str();

            if(f2.dx_str() == "1")
                s2 << f1.str();
            else if(f2.dx_str() != "0")
                s2 << f1.str() << "*" << f2.dx_str();

            if(s1.str() == "")
                s << s2.str();
            else if(s2.str() == "")
                s << s1.str();
            else
                s << s1.str() << "+" << s2.str();
            return s.str();
        }

        // string dx_str() {
        //     stringstream s;
        //     if(f1.dx_str() != "1" && f2.dx_str() != "1")
        //         s << f1.dx_str() << "*" << f2.str() << "+" << f1.str() << "*" << f2.dx_str();

        //     else if (f1.dx_str() == "1" && f2.dx_str() != "1")
        //         s << f2.str() << "+" << f1.str() << "*" << f2.dx_str();

        //     else if (f1.dx_str() != "1" && f2.dx_str() == "1")
        //         s << f1.dx_str() << "*" << f2.str() << "+" << f1.str();

        //     else
        //         s << "(" << f2.str() << ")+(" << f1.str() << ")";
        //     return s.str();
        // }

    private:
        F1 f1;
        F2 f2;
    public:
        string signal = "*";
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
            s << f1.str() << "+" << f2.str();
            return s.str();
        }

        string dx_str() {
            stringstream s;
            stringstream s1;
            stringstream s2;
            if(f1.dx_str() != "0" && f2.dx_str() != "0")
                s << f1.dx_str() << "+" << f2.dx_str();
            else if(f1.dx_str() != "0")
                s << f1.dx_str();
            else if(f2.dx_str() != "0")
                s << f2.dx_str();
            return s.str();
        }

    private:
        F1 f1;
        F2 f2;
    public:
        string signal = "+";
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
            if(f2.signal == "-")
                s << f1.str() << signal << "(" << f2.str() << ")";
            else
                s << f1.str() << signal << f2.str();
            return s.str();
        }

        string dx_str() {
            stringstream s;
            if(f1.dx_str() != "0" && f2.dx_str() != "0")
                s << f1.dx_str() << "-" << f2.dx_str();
            else if(f1.dx_str() != "0")
                s << f1.dx_str();
            else if(f2.dx_str() != "0")
                s << f2.dx_str();
            return s.str();
        }

    private:
        F1 f1;
        F2 f2;
    public:
        string signal = "-";
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
            s << f1.str() << "/" << f2.str();
            return s.str();
        }

        string dx_str() {
            stringstream s;
            stringstream s1;
            stringstream s2;
            // s << "-" << f1.str() << "*" << f2.dx_str() << "/" << f2.str() << "^2";
            if(f1.dx_str() == "1")
                s1 << f2.str();
            else if(f1.dx_str() != "0")
                s1 << f1.dx_str() << "*" << f2.str();

            if(f2.dx_str() == "1")
                s2 << f1.str();
            else if(f2.dx_str() != "0") {
                if(f1.str() == "1")
                    s2 << f2.dx_str();
                else
                    s2 << f1.str() << "*" << f2.dx_str();
            } else {
                if(f1.signal == "+" || f1.signal == "+"
                || f1.signal == "/" || f1.signal == "*")
                    s1 << "(" << s1.str() << ")";

                s << s1.str() << "/" << f2.str() << "^2";
                return s.str();
            }

            if(f2.signal == "+" || f2.signal == "+"
            || f2.signal == "/" || f2.signal == "*"
            || f2.signal == "->*") {
                s << s1.str() << "-(" << s2.str() << ")/(" << f2.str() << ")^2";
                return s.str();
            }

            s << s1.str() << "-" << s2.str() << "/" << f2.str() << "^2";
            return s.str();
        }

    private:
        F1 f1;
        F2 f2;
    public:
        string signal = "/";
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
            if(f1.signal == "+" || f1.signal == "-"
            || f1.signal == "/" || f1.signal == "*")
                s << "(" << f1.str() << ")" << "^" << to_string(n);
            else
                s << f1.str() << "^" << to_string(n);
            return s.str();
        }

        string dx_str() {
            stringstream s;
            if(f1.dx_str() != "1") {
                if(f1.signal == "+" || f1.signal == "-"
                || f1.signal == "*" || f1.signal == "/"
                || f1.signal == "->*") {
                    s << to_string(n) << "*(" << f1.dx_str() << ")*(" << f1.str() << ")";
                }
                else
                    s << to_string(n) << "*" << f1.dx_str() << "*" << f1.str();
            }
            else
                s << to_string(n) << "*" << f1.str();
            if(n > 2)
                s << "^" << (n-1);
            return s.str();
        }

    private:
        F1 f1;
        int n;
    public:
        string signal = "->*";
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
            if(f1.signal == "+" || f1.signal == "-"
            || f1.signal == "/" || f1.signal == "*") {
                s << "exp(" << f1.str() << ")*(" << f1.dx_str() << ")";
            }
            else
                s << "exp(" << f1.str() << ")*" << f1.dx_str();
            return s.str();
        }

    private:
        F1 f1;
    public:
        string signal = "exp";
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
            s << "log(" << f1.str() << ")";
            return s.str();
        }

        string dx_str() {
            stringstream s;
            if(f1.dx_str() == "1")
                s << "1/(" << f1.str() << ")";
            else
                s << "1/(" << f1.str() << ")*" << f1.dx_str();
            return s.str();
        }

    private:
        F1 f1;
    public:
        string signal = "log";
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

template <typename F1>
Adicao<F1,Cte> operator + (F1 f1, int n) {
    return Adicao<F1,Cte> (f1,double(n));
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

X x;

int main() {
    // auto f = x*x;
    // f(x) = x*x, f'(x) = x+x

    // auto f = x-(x-5);
    // f(x) = x-(x-5), f'(x) = 1-1

    // auto f = x->*3;
    // f(x) = x^3
    // f'(x) = 3*x^2

    // auto f = (7.0*x+5.0)->*3;
    // f(x) = (7*x+5)^3
    // f'(x) = 3*7*(7*x+5)^2

    // auto f = sin( x + 5 );
    // f(x) = sin(x+5)
    // f'(x) = cos(x+5)

    // auto f = 1.0 / (sin(x)->*2 + cos(x)->*2)->*4;
    // 1/(sin(x)^2+cos(x)^2)^4
    // f'(x) = -(4*(2*cos(x)*sin(x)+2*-sin(x)*cos(x))*(sin(x)^2+cos(x)^2)^3)/((sin(x)^2+cos(x)^2)^4)^2
            //    -(4*2*cos(x)*sin(x)+2*-sin(x)*cos(x)*(sin(x)^2+cos(x)^2)^3*1)/((sin(x)^2+cos(x)^2)^4)^2


    // auto f = exp( x * log( x - 8.0 ) + 1.0 );
    // f(x) = exp(x*log(x-8)+1)
    // f'(x) = exp(x*log(x-8)+1)*(log(x-8)+x*1/(x-8))

    // auto f = exp( (x + 1.0)*(x - 1.0) )->*2;
    // f(x) = exp((x+1)*(x-1))^2
    // f'(x) = 2*exp((x+1)*(x-1))*((x-1)+(x+1))*exp((x+1)*(x-1))

    // auto f = log( x->* 4 + 8.0 ) * sin( cos( x / 3.14 ) );
    // f(x) = log(x^4+8)*sin(cos(x/3.14))
    // f'(x) = 1/(x^4+8)*4*x^3*sin(cos(x/3.14))+log(x^4+8)*cos(cos(x/3.14))*-sin(x/3.14)*3.14/3.14^2

    // auto f = log( (8.1/(x+1.0)->*3 - 9.2 ) * (x + 3) *sin( cos( x / 3.14 ) ) + x);
    // f(x) = log((8.1/(x+1)^3-9.2)*(x+3)*sin(cos(x/3.14))+x)
    // f'(x) = 1/((8.1/(x+1)^3-9.2)*(x+3)*sin(cos(x/3.14))+x)*((-(8.1*3*(x+1)^2)/((x+1)^3)^2*(x+3)+8.1/(x+1)^3-9.2)*sin(cos(x/3.14))+(8.1/(x+1)^3-9.2)*(x+3)*cos(cos(x/3.14))*-sin(x/3.14)*3.14/3.14^2+1)

    auto f = (8.1/(x+1.0)->*3 - 9.2 ) * (x + 3) *sin( cos( x / 3.14 ) ) + x;
    // ((-(8.1*3*(x+1)^2)/((x+1)^3)^2*(x+3)+8.1/(x+1)^3-9.2)*sin(cos(x/3.14))+(8.1/(x+1)^3-9.2)*(x+3)*cos(cos(x/3.14))*-sin(x/3.14)*3.14/3.14^2+1)

    cout << f.str() << endl;
    cout << f.dx_str() << endl;

    return 0;
}
