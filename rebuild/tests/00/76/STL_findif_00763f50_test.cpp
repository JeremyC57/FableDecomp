// Behaviour test for 0x00763F50 STLport __find_if over C3DVector with the circulation predicate: returns the first
// accepted point (in the unrolled body and in each remainder case) or last, calling the predicate in order and never
// past the hit.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };

struct CPred {                                     // stand-in predicate: accepts points with X == target
    float target;
    int* calls;
    bool operator()(const C3DVector& p) const { ++*calls; return p.X == target; }
};

struct random_access_iterator_tag { };

template <class It, class Pred>
It find_if_model(It first, It last, Pred pred, const random_access_iterator_tag&)
{
    int trip = (last - first) >> 2;
    for ( ; trip > 0; --trip) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
        ++first;
    case 0:
    default:
        return last;
    }
}

int main()
{
    C3DVector v[11];
    for (int i = 0; i < 11; ++i) { v[i].X = (float)i; v[i].Y = 0.0f; v[i].Z = 0.0f; }
    int ok = 1;
    for (int n = 0; n <= 11; ++n) {                 // every length 0..11: each hit position and the miss
        for (int t = 0; t <= n; ++t) {
            int calls = 0;
            CPred p = { (float)t, &calls };
            C3DVector* r = find_if_model(v, v + n, p, random_access_iterator_tag());
            if (t < n) { if (r != v + t || calls != t + 1) ok = 0; }
            else { if (r != v + n || calls != n) ok = 0; }
        }
    }
    if (!ok) { std::printf("00763f50_TEST FAIL\n"); return 1; }
    std::printf("00763f50_TEST PASS\n");
    return 0;
}