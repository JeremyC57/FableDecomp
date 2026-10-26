// Behaviour test for 0x00808120 STLport __unguarded_insertion_sort_aux over pair<C3DVector, long> (score = compare value, larger first).
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct SPair { C3DVector first; long second; };
struct CCompare {                                        // stand-in score: the X coordinate
    float Score(const C3DVector& v) const { return v.X; }
    bool operator()(const SPair& a, const SPair& b) const { return Score(a.first) > Score(b.first); }
};
static void UnguardedLinearInsert(SPair* last, SPair val, CCompare comp)
{
    SPair* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
static void UnguardedInsertionSortAux(SPair* first, SPair* last, SPair*, CCompare comp)
{
    for (SPair* i = first; i != last; ++i)
        UnguardedLinearInsert(i, SPair(*i), comp);
}
int main()
{
    SPair v[8];
    const float xs[8] = { 100.0f, 3.0f, 9.0f, 1.0f, 7.0f, 9.0f, -2.0f, 5.0f };   // v[0] is the sentinel (best score)
    for (int i = 0; i < 8; ++i) { v[i].first.X = xs[i]; v[i].first.Y = 0.0f; v[i].first.Z = 0.0f; v[i].second = i; }
    CCompare comp;
    UnguardedInsertionSortAux(v + 1, v + 8, (SPair*)0, comp);
    bool ok = v[0].second == 0;
    for (int i = 1; ok && i < 7; ++i)
        ok = !(v[i + 1].first.X > v[i].first.X);          // non-increasing scores
    ok = ok && v[1].first.X == 9.0f && v[1].second == 2 && v[2].second == 5;   // stable for equal scores (strict compare)
    ok = ok && v[7].first.X == -2.0f && v[7].second == 6;
    if (!ok) { std::printf("00808120_TEST FAIL\n"); return 1; }
    std::printf("00808120_TEST PASS\n");
    return 0;
}