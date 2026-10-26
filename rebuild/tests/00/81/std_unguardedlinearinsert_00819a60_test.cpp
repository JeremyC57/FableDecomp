// Behaviour test for 0x00819A60 __unguarded_linear_insert<pair<unsigned long, CSoundPair>> (model copy).
#include <cstdio>
class CSoundPair { public: int FirstSound; int LastSound; };
struct E {
    unsigned int first; CSoundPair second;
    E() : first(0) { second.FirstSound = 0; second.LastSound = 0; }
    E(const E& o) : first(o.first), second(o.second) { }
};
struct P { bool operator()(const E& a, const E& b) const { return a.first < b.first; } };
static void setk(E& e, unsigned int k) { e.first = k; e.second.FirstSound = (int)k; e.second.LastSound = (int)(k ^ 0x55u); }
static int chk(const E& e, unsigned int k) { return e.first == k && e.second.FirstSound == (int)k && e.second.LastSound == (int)(k ^ 0x55u); }
typedef unsigned int K;
template <class E, class P>
static void unguarded_linear_insert_model(E* last, E val, P comp)
{
    E* next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
template <class E, class P>
static void unguarded_insertion_sort_aux_model(E* first, E* last, E*, P comp)
{
    for (E* i = first; i != last; ++i) unguarded_linear_insert_model(i, E(*i), comp);
}
int main()
{
    static const K keys[9] = { 0u, 5u, 3u, 0xFFFFFFF0u, 3u, 9u, 0x80000000u, 1u, 12u };
    static const K want[9] = { 0u, 1u, 3u, 3u, 5u, 9u, 12u, 0x80000000u, 0xFFFFFFF0u };
    E v[9];
    int i;
    for (i = 0; i < 9; ++i) setk(v[i], keys[i]);
    // put the minimum first as the unguarded sentinel; [1] is the first element to sort
    { int m = 0; for (i = 1; i < 9; ++i) if (v[i].first < v[m].first) m = i; E t(v[0]); v[0] = v[m]; v[m] = t; }
    // sentinel-guarded insertion sort driven through the sink step (v[0] is the minimum)
    for (i = 2; i < 9; ++i) unguarded_linear_insert_model(v + i, E(v[i]), P());
    int ok = 1;
    for (i = 0; i < 9; ++i) if (!chk(v[i], want[i])) ok = 0;
    if (!ok) { std::printf("00819a60_TEST FAIL\n"); return 1; }
    std::printf("00819a60_TEST PASS\n");
    return 0;
}