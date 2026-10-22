#include <cstdio>
// Behaviour test for 0080aa60: STLport __linear_insert on target points -- when val sorts before *first, the range
// [first,last) shifts up one and val lands at first; otherwise the unguarded linear insert walks back from last.
struct CThing { int id; };
struct Ptr { CThing* item; };
struct TP { Ptr PThing; long Index; float X, Y, Z; long FrameRejected; };
struct Comp { int calls; bool operator()(const TP& a, const TP& b) { ++calls; return a.Index < b.Index; } };
TP* CopyBackward(TP* first, TP* last, TP* result) { for (long n = last - first; n > 0; --n) *--result = *--last; return result; }
void UnguardedLinearInsert(TP* last, TP val, Comp comp)
{
    TP* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
void LinearInsert(TP* first, TP* last, TP val, Comp comp)
{
    if (comp(val, *first)) { CopyBackward(first, last, last + 1); *first = val; }
    else UnguardedLinearInsert(last, val, comp);
}
static TP Make(long i) { TP t; t.PThing.item = 0; t.Index = i; t.X = (float)i; t.Y = 0; t.Z = 0; t.FrameRejected = 0; return t; }
int main()
{
    TP v[5]; v[0] = Make(2); v[1] = Make(4); v[2] = Make(6); v[3] = Make(1); v[4] = Make(0);
    Comp c; c.calls = 0;
    LinearInsert(v, v + 3, v[3], c);      // 1 goes to the front
    int ok = (v[0].Index == 1 && v[1].Index == 2 && v[2].Index == 4 && v[3].Index == 6 && v[0].X == 1.0f);
    TP w[4]; w[0] = Make(1); w[1] = Make(3); w[2] = Make(5); w[3] = Make(4);
    LinearInsert(w, w + 3, w[3], c);      // 4 goes between 3 and 5
    ok = ok && w[0].Index == 1 && w[1].Index == 3 && w[2].Index == 4 && w[3].Index == 5;
    if (!ok) { std::printf("0080aa60_TEST FAIL\n"); return 1; }
    std::printf("0080aa60_TEST PASS\n");
    return 0;
}