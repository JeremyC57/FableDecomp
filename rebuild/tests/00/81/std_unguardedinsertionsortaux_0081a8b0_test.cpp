// Behaviour test for 0x0081A8B0 __unguarded_insertion_sort_aux<pair<long, 20-byte POD>> (model copy).
#include <cstdio>
struct SRecordInfo { long m_words[5]; };
struct E {
    long first; SRecordInfo second;
    E() : first(0) { for (int i = 0; i < 5; ++i) second.m_words[i] = 0; }
    E(const E& o) : first(o.first), second(o.second) { }
};
struct P { bool operator()(const E& a, const E& b) const { return a.first < b.first; } };
static void setk(E& e, long k) { e.first = k; e.second.m_words[4] = (long)(k * 3); }
static int chk(const E& e, long k) { return e.first == k && e.second.m_words[4] == (long)(k * 3); }
typedef long K;
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
    static const K keys[9] = { -100, 5, 3, 3, 9, -7, 0, 12, 1 };
    static const K want[9] = { -100, -7, 0, 1, 3, 3, 5, 9, 12 };
    E v[9];
    int i;
    for (i = 0; i < 9; ++i) setk(v[i], keys[i]);
    { int m = 0; for (i = 1; i < 9; ++i) if (v[i].first < v[m].first) m = i; E t(v[0]); v[0] = v[m]; v[m] = t; }
    unguarded_insertion_sort_aux_model(v + 1, v + 9, (E*)0, P());
    int ok = 1;
    for (i = 0; i < 9; ++i) if (!chk(v[i], want[i])) ok = 0;
    if (!ok) { std::printf("0081a8b0_TEST FAIL\n"); return 1; }
    std::printf("0081a8b0_TEST PASS\n");
    return 0;
}