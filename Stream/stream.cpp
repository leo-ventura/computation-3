#include <iostream>
#include <vector>
#include <map>

using namespace std;

template <typename I, typename F>
auto operator | (I iterable, F fn) {
    for(auto _i : iterable)
        fn(_i);
}

template <int N, typename I, typename F>
void operator | (I (& iterable) [N], F fn) {
    for(int i = 0; i < N; i++)
        fn(iterable[i]);
}

void print( int x ) { cout << x << " "; }

int main() {
    // int tab[10] = { 1, 2, 3, 2, 3, 4, 6, 0, 1, 8 };
    // vector<int> v{ 2 , 6, 8 };

    // tab | []( int x ) { cout << x*x << endl; };

    // tab | [ &v ]( int x ) { v.push_back( x ); };

    // v | []( int x ) { cout << x*x << endl; };

    // v | &print;

    // std::vector<std::string> v1 = { "janeiro", "fevereiro", "março", "abril", "maio" };
    // v1 | &std::string::length; //| [] ( int x ) { std::cout << x << " "; };

    map<string,string> v = { { "a", "1" }, { "b", "2" }, { "c", "3" }, { "d", "4" }, { "e", "5" } };
    v | []( auto x ){ return pair{ x.first, stod( x.second ) }; };

}