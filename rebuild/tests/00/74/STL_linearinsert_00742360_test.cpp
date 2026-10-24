// Behaviour test for 0x00742360 _STL::__linear_insert over the mode-registry pairs (unsigned key, value by value)
#include <cstdio>
struct CRegisteredModeInfo { int tag; };

template <class A, class B> struct pair {
    A first;
    B second;
    pair() {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<unsigned int, CRegisteredModeInfo> TModePair;   // unsigned int: the 32-bit unsigned key

struct TModeLess {
    bool operator()(const TModePair& a, const TModePair& b) const { return a.first < b.first; }
};

static void Fill(TModePair* p, const unsigned int* keys, int n)
{
    for (int i = 0; i < n; ++i) { p[i].first = keys[i]; p[i].second.tag = i; }
}

struct random_access_iterator_tag {};

template <class RandomAccessIter, class BidirectionalIter, class Distance>
inline BidirectionalIter __copy_backward(RandomAccessIter first, RandomAccessIter last, BidirectionalIter result,
                                         const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

inline TModePair* copy_backward(TModePair* first, TModePair* last, TModePair* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

static int g_unguarded = 0;
void __unguarded_linear_insert(TModePair* last, TModePair val, TModeLess comp)    // stand-in for 0x0073CFC0
{
    ++g_unguarded;
    TModePair* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

void __linear_insert(TModePair* first, TModePair* last, TModePair val, TModeLess comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

int main()
{
    int ok = 1;
    // a key below the first: the whole run shifts up and the value lands at the front
    TModePair p[5];
    unsigned int keys[5] = { 4u, 6u, 0x80000000u, 9u, 2u };
    Fill(p, keys, 5);
    __linear_insert(p, p + 4, TModePair(p[4]), TModeLess());
    unsigned int want[5] = { 2u, 4u, 6u, 0x80000000u, 9u };
    for (int i = 0; i < 5; ++i) if (p[i].first != want[i]) ok = 0;
    if (p[0].second.tag != 4 || p[4].second.tag != 3 || g_unguarded != 0) ok = 0;
    // a key not below the first (unsigned: 0x80000001 is large) goes through the unguarded insert
    TModePair q[4];
    unsigned int k2[4] = { 1u, 5u, 0x90000000u, 0x80000001u };
    Fill(q, k2, 4);
    __linear_insert(q, q + 3, TModePair(q[3]), TModeLess());
    unsigned int w2[4] = { 1u, 5u, 0x80000001u, 0x90000000u };
    for (int i = 0; i < 4; ++i) if (q[i].first != w2[i]) ok = 0;
    if (g_unguarded != 1 || q[2].second.tag != 3) ok = 0;
    if (!ok) { std::printf("00742360_TEST FAIL\n"); return 1; }
    std::printf("00742360_TEST PASS\n");
    return 0;
}