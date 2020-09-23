#include <functional>
#include <vector>

using namespace std;

template<int L, int C>
class Matriz {
    public:
        vector<vector<double>> matrix;

        Matriz() {
            vector<vector<double>> m(L, vector<double>(C));
        }

        void insert(int i, int j, double v) {
            matrix[i][j] = v;
        }

        auto operator + (Matriz<L,C> m) {
            Matriz<L,C> result;
            for(int i = 0; i < L; i++) {
                for(int j = 0; j < C; j++)
                    result.insert(i,j,
                        matrix[i][j] + m[i][j]);
            }
            return result;
        }

};

template <int L, int CL, int C>
class Op {
public:
  Matriz<L,CL> a;
  Matriz<CL,C> b;

  Otimizador(Matriz<L,CL> a, Matriz<CL,C> b): a(a), b(b) {}

  int custo(int n, int m, int p) {
    return n*p*(2*m-1);
  }

  template <int Q>
  auto operator*(Matriz<P,Q> c) {
    // Temos a: N×M, b: M×P, c: P×Q
    if(custo(N,M,P)+custo(N,P,Q) // N×M * M×P (=N×P) + N×P * P×Q (=N×Q)
                   >
       custo(M,P,Q)+custo(N,M,Q))// M×P * P×Q (=M×Q) + N×M * M×Q (=N×Q)
    {
      return mult(a,mult(b,c));
    }
    return mult(mult(a,b),c);
  }
};


template <int L, int CL, int C>
auto operator * (const Matriz<L,CL>& a, const Matriz<CL,C>& b) {
    return Op<L, C, C2>(a,b);
}

// template <int L, int C>
// class Matriz;

// template <int L, int CL, int C>
// auto mult(const Matriz<L,CL>& m1, const Matriz<CL,C> m2) {
//   Matriz<L,C> res;
//   for(int i = 0; i < L; i++) {
//     for(int j = 0; j < C; j++) {
//       double ij = 0;
//       for(int k = 0; k < CL; k++) {
//         ij += m1.at(i,k) * m2.at(k,j);
//       }
//       res.set(i,j,ij);
//     }
//   }
//   return res;
// }

template <int N, int M, int P>
class Otimizador {
public:
  Matriz<N,M> a;
  Matriz<M,P> b;

  Otimizador(Matriz<N,M> a, Matriz<M,P> b): a(a), b(b) {}

  int custo(int n, int m, int p) {
    return n*p*(2*m-1);
  }

  template <int Q>
  auto operator*(Matriz<P,Q> c) {
    // Temos a: N×M, b: M×P, c: P×Q
    if(custo(N,M,P)+custo(N,P,Q) // N×M * M×P (=N×P) + N×P * P×Q (=N×Q)
                   >
       custo(M,P,Q)+custo(N,M,Q))// M×P * P×Q (=M×Q) + N×M * M×Q (=N×Q)
    {
      return mult(a,mult(b,c));
    }
    return mult(mult(a,b),c);
  }
};

template <int L, int C>
class Matriz {
public:
    vector<vector<double>> m;

    Matriz() {
        vector<vector<double>> aux(L);
        for(int i = 0; i < L; i++) {
            aux[i] = vector<double>(C); 
            for(int j = 0; j < C; j++) {
            aux[i][j] = 0;
            }
        }
        m = aux;
    }

    template <int N, int M, int P>
    Matriz(Otimizador<N,M,P> o) {
        Matriz<N,P> c = mult(o.a,o.b);
        vector<vector<double>> aux(L);
        for(int i = 0; i < L; i++) {
            aux[i] = vector<double>(C); 
            for(int j = 0; j < C; j++) {
                aux[i][j] = c.at(i,j);
            }
        }
        m = aux;
    }

    void set(int i, int j, double v) { m[i][j] = v; }
    double at(int i, int j) const { return m[i][j]; }

    Matriz<L,C> operator+(Matriz<L,C> m2) {
    Matriz<L,C> res;
    for(int i = 0; i < L; i++)
        for(int j = 0; j < C; j++)
        res.set(i,j,m[i][j] + m2.at(i,j));
    return res;
    }
    };

template <int L, int C, int C2>
auto operator*(const Matriz<L,C>& a, const Matriz<C,C2>& b) {
  return Otimizador<L, C, C2>(a,b);
}


template <typename F>
class Apply {
public:
    Apply(F f): f(f) {}

    template <int L, int C>
    Matriz<L,C> operator()(const Matriz<L,C>& m) const {
        if constexpr(is_same_v<void,invoke_result_t<F,double>>) {
            for(int i = 0; i < L; i++)
                for(int j = 0; j < C; j++)
                    invoke(f, m.at(i,j));
            return m;
        }
        Matriz<L,C> nm;
        for(int i = 0; i < L; i++)
            for(int j = 0; j < C; j++)
                nm.set(i,j,invoke(f, m.at(i,j)));
            return nm;
    }

    template <int N, int M, int P>
    Matriz<N,P> operator()(const Otimizador<N,M,P>& o) const {
    Matriz<N,P> m = o;
    if constexpr(is_same_v<void,invoke_result_t<F,double>>) {
        for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++)
            invoke(f, m.at(i,j));
        return m;
    }
    for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++)
        m.set(i,j,invoke(f, m.at(i,j)));
    return m;
}

private:
    F f;
};
