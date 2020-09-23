#include <iostream>
#include <vector>

using namespace std;

template <int N, typename T = double>
class Vetor {
    public:

    class MeioDaOperacaoAtr {
        private:
            MeioDaOperacaoAtr( Vetor* v ): vetor( v ) {}

        // Declaro que a classe Vetor pode enxergar campos e métodos privados
        friend class Vetor;

        public:
            MeioDaOperacaoAtr operator , ( T n ) {
                *vetor = n;

                return *this;
            }

        private:
            Vetor *vetor;
    }; // fim da classe MeioDaOperacaoAtr

    class MeioDoProdVetorial {
        private:
            MeioDoProdVetorial( Vetor* v ): vetor( v ) {}

        // Declaro que a classe Vetor pode enxergar campos e métodos privados
        friend class Vetor;

        private:
            Vetor *vetor;
    }; // fim da classe MeioDoProdVetorial

    /* OPERADORES BASICOS DE VETORES */
    Vetor operator + (const Vetor& v) {
        Vetor<N,T> r;
        for(int i = 0; i < N; i++)
            r.tab.push_back(tab[i] + v.tab[i]);
        return r;
    }

    Vetor operator - (const Vetor& v) {
        Vetor<N,T> r;
        for(int i = 0; i < N; i++)
            r.tab.push_back(tab[i] - v.tab[i]);
        return r;
    }

    Vetor operator * (T n) {
        Vetor<N,T> r;
        for(int i = 0; i < N; i++)
            r.tab.push_back(tab[i] * n);
        return r;
    }

    T operator * (const Vetor& v) {
        T r = 0;
        for(int i = 0; i < N; i++)
            r += tab[i] * v.tab[i];
        return r;
    }

    MeioDoProdVetorial operator * () {
        return MeioDoProdVetorial(this);
    }

    Vetor operator *(const MeioDoProdVetorial& m) {
        auto v = m.vetor;
        Vetor<N,T> r;
        switch(N) {
            case 2:
                r.tab.push_back(0); // coordenada i
                r.tab.push_back(0); // coordenada j
                r.tab.push_back(tab[0]*v->tab[1] - tab[1]*v->tab[0]); // coordenada k
                break;
            case 3:
                r.tab.push_back(tab[1]*v->tab[2] - tab[2]*v->tab[1]); // coordenada i
                r.tab.push_back(tab[2]*v->tab[0] - tab[0]*v->tab[2]); // coordenada j
                r.tab.push_back(tab[0]*v->tab[1] - tab[1]*v->tab[0]); // coordenada k
                break;
        }
        return r;
    }

    Vetor operator / (T n) {
        Vetor<N,T> r;
        for(int i = 0; i < N; i++)
            r.tab.push_back(tab[i] / n);
        return r;
    }

    // redefinindo operador de atribuição pela preferencia em relacao à ,
    MeioDaOperacaoAtr operator = ( T n ) {
        tab.push_back( n );
        return MeioDaOperacaoAtr( this );
    }

    void imprime( ostream& o ) const {
        for( auto x : tab ) 
            o << x << " ";
    }

    public:
        vector<T> tab;

};

template <int N, typename T = double>
ostream& operator << ( ostream& o, const Vetor<N,T>& v) {
    v.imprime( o );
    return o;
}

template <int N, typename T = double>
Vetor<N,T> operator * (double n, const Vetor<N,T>& v) {
    Vetor<N,T> r;
    for(int i = 0; i < N; i++)
        r.tab.push_back(v.tab[i] * n);
    return r;

}

int main( int argc, char *argv[] ) {
    Vetor<2,double> v1;
    v1 = 4.5, 6.1;
    v1.imprime();

    Vetor<2,double> v2;
    v2 = 6.7, 7.2;
    v2.imprime();

    Vetor<2,double> r;
    r = v1 + v2;
    r.imprime();

    r = v1 - v2;
    r.imprime();

    r = v2 * 2;
    r.imprime();

    r = 2 * v2;
    r.imprime();

    r = v2 / 2;
    r.imprime();

    r = v1 ** v2;
    r.imprime();

    double p;
    p = v1 * v2;
    cout << p << endl;

    cout << "Testando para vetor de 3 dimensoes" << endl;

    Vetor<3, double> v3;
    v3 = 4.1, 5.2, 6.3;
    v3.imprime();

    Vetor<3, double> v4;
    v4 = 1.1, 2.1, 1.1;
    v4.imprime();

    Vetor<3, double> v5;
    v5 = 0.5, 0.1, 0.2;
    v5.imprime();

    Vetor<3, double> s;
    s = v3 + v4 + v5;
    s.imprime();

    s = v3 - v4 - v5;
    s.imprime();

    s = v5 * 2;
    s.imprime();

    s = v5 / 2;
    s.imprime();

    return 0;
}