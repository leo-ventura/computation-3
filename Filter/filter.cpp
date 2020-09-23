#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

template <typename F>
class MidString {
    public:
        MidString(vector<string> iterable, F fn) {
            for(auto  i : iterable) {
                    tabOriginal.push_back(i);
                    tabResult.push_back((i.*fn)());
            }
        }
        vector<string> tabOriginal;
        vector<int> tabResult;

};

// MidString
template <typename F>
MidString<F> operator | (vector<string> iterable, F fn) {
    return MidString<F>(iterable, fn);
}

template <typename F1, typename F2>
void operator | (MidString<F1> mid, F2 fn) {
    for(unsigned i = 0; i < mid.tabOriginal.size(); i++) {
        if(mid.tabResult[i])
            fn(mid.tabResult[i]);
    }
}

// Map template
template <typename A1, typename A2, typename F>
auto operator | (map<A1,A2> iterable, F fn) {
    if constexpr (std::is_same<decltype(fn(*iterable.begin())), void>::value) {
        for(auto i : iterable)
            fn(i);
    } else if constexpr (std::is_same_v<decltype(fn(*iterable.begin())), bool>::value) {
        vector<decltype(iterable.begin())> v;
        for(auto i : iterable) {
            if(fn(i))
                v.push_back(i);
        }
        return v;
    } else {
        vector<decltype(fn(*iterable.begin()))> v;
        for(auto i : iterable) {
            v.push_back(fn(i));
        }
        return v;
    }
}

// Vector template
template <typename I, typename F>
auto operator | (vector<I> iterable, F fn) {
    if constexpr (std::is_same<decltype(fn(iterable.at(0))), bool>::value) {
        vector<I> v;
        for(auto i : iterable) {
            if(fn(i))
                v.push_back(i);
        }
        return v;
    } else {
        for(auto i : iterable)
            fn(i);
    }
}

// Literal array template
template <int N, typename I, typename F>
auto operator | (I (& iterable) [N], F fn) {
    if constexpr (std::is_same<decltype(fn(iterable[0])), bool>::value) {
        vector<I> v;
        for(int i = 0; i < N; i++) {
            if(fn(iterable[i]))
                v.push_back(iterable[i]);
        }
        return v;
    } else {
        for(int i = 0; i < N; i++) {
            fn(iterable[i]);
        }
    }
}

int main() {
    // std::vector<std::string> v1 = { "janeiro", "fevereiro", "março", "abril", "maio" };
    // v1 | &std::string::length | [] ( int x ) { std::cout << x << " "; };

    // vector<int> v1 = { 2, 9, 8, 8, 7, 4 };
    // auto result = v1 | []( int x ) { return x % 2 == 0; };
    // for( auto x : result )
    //     cout << x << " ";

    // vector<int> v{ 2 , 6, 8, 10, 11, 13, 14, 15, 16};
    // v | []( int x ) { return x % 2 == 0; } | [] ( int x ) { cout << x << endl; };

    // int v1[] = { 2, 9, 8, 8, 7, 4 };
    // auto result = v1 | []( int x ) { return x % 2 == 0; };
    // for( auto x : result )
    //     cout << x << " ";

    // int v1[] = { 2, 9, 8, 8, 7, 4 };
    // v1 | []( int x ) { return x % 2 == 0; } | [] ( int x ) { cout << x << " "; };

    // map<string,string> v = { { "a", "1" }, { "b", "2" }, { "c", "3" }, { "d", "4" }, { "e", "5" } };
    // v | []( auto x ){ return pair{ x.first, stod( x.second ) }; } | []( auto p ) { cout << p.second + 1.1 << " "; };

    // 2.1 3.1 4.1 5.1 6.1

    map<string,string> v = { { "a", "1" }, { "b", "2" }, { "c", "3" }, { "d", "4" }, { "e", "5" }, { "f", "6" } };
    auto par = []( auto x ){ return stoi( x.second ) % 2 == 0; };
    auto upper_first = []( auto x ){ string st = x.first; transform(st.begin(), st.end(), st.begin(), ::toupper); return st; };
    v | par | upper_first | []( auto st ) { cout << st + "! "; };

    // B! D! F!

    // map<string,string> v = { { "a", "1" }, { "b", "2" }, { "c", "3" }, { "d", "4" }, { "e", "5" }, { "F", "6" } };
    // auto par = []( auto x ){ return stoi( x.second ) % 2 == 0; };
    // auto first = []( pair<string,string> p ) { return p.first; };
    // auto upper = []( string st ){ transform(st.begin(), st.end(), st.begin(), ::toupper); return st; };
    // vector<string> result = v | par | first | upper;
    // result | []( auto st ) { cout << st + "! "; };

    // F! B! D!

    // vector<string> v1 = { "janeiro", "fevereiro", "março", "abril", "maio" };
    // v1 | &string::length | [] ( int x ) { cout << x << " "; };

    // 7 9 6 5 4

}