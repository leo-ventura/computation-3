#include <iostream>
#include <vector>
#include <map>
#include <initializer_list>

using namespace std;

template <typename I, typename F>
auto apply( I iterable, F fn ) {
    // vector<decltype(fn)> _vector;
    // vector<int> _vector;
    typename I::const_iterator _i = iterable.begin();

    vector<decltype(fn(*_i))> _vector;

    while(_i != iterable.end()) {
        _vector.push_back(fn(*_i++));
    }

    return move(_vector);
}

template <typename F>
auto apply( initializer_list<int> iterable, F fn ) {
    initializer_list<int>::const_iterator _i = iterable.begin();

    vector<decltype(fn(*_i))> _vector;

    while(_i != iterable.end()) {
        _vector.push_back(fn(*_i++));
    }

    return move(_vector);
}

string roman( int n ) {
    map<int,string> rom = { { 1, "I" }, { 2, "II" }, { 3, "III" }, { 4, "IV" }, { 5, "V" }, { 6, "VI" }, { 7, "VII" }, { 8, "VIII" } } ;

    return rom[n]; 
}

int soma2(int x) {
    return x+2;
}

double soma2v1(int x) {
    return x + 2.1;
}

template<typename T>
void print(vector<T> r) {
    for(auto _ : r)
        cout << _ << " ";
    cout << endl << endl;
}

int main( int argc, char *argv[] ) {
    vector<int> v = {1, 2, 3};
    // vector<int> v

    auto v2 = { 1, 2, 3, 4, 5 };
    // cout << ::apply( v2, []( auto n ) -> double { return n*n + 0.1; } ) << endl;

    // results

    auto r1 = apply(v, soma2);
    auto r2 = apply(v, soma2v1);
    auto r3 = apply(v2, []( auto n ) -> double { return n*n + 0.1; });
    auto r4 = apply(v2, roman);

    cout << "r1:" << endl;
    print(r1);

    cout << "r2:" << endl;
    print(r2);

    cout << "r3:" << endl;
    print(r3);

    cout << "r4:" << endl;
    print(r4);

    return 0;
}