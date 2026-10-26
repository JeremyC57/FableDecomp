// behaviour test for 0x008268F0 std::vector<CMap::CScriptedMapBrushUpdateArea >::push_back: with room, x is copied to the end (first word, then the
// string's copy) and finish advances by one 8-byte element; at capacity the overflow path gets (finish, x, 1, true)
#include <cstdio>
#include <new>
static int gStrCopies = 0; static int gOverflow = 0; static unsigned int gFill = 0; static bool gAtEnd = false;
struct Str { int Id; Str() : Id(0) { } Str(const Str& o) : Id(o.Id) { ++gStrCopies; } };
struct Elem { unsigned int Key; Str Text; };
struct Vec { Elem* Start; Elem* Finish; Elem* EndOfStorage; };
static void Overflow(Vec& v, Elem* pos, const Elem& x, unsigned int fill, bool atEnd) { ++gOverflow; gFill = fill; gAtEnd = atEnd; (void)v; (void)pos; (void)x; }
static void PushBack(Vec& v, const Elem& x)
{
    if (v.Finish != v.EndOfStorage) { new (v.Finish) Elem(x); ++v.Finish; }
    else Overflow(v, v.Finish, x, 1u, true);
}
int main()
{
    int fail = 0;
    static Elem storage[2];
    Vec v; v.Start = storage; v.Finish = storage; v.EndOfStorage = storage + 2;
    Elem a; a.Key = 5; a.Text.Id = 9;
    PushBack(v, a);
    if (v.Finish != storage + 1 || storage[0].Key != 5 || storage[0].Text.Id != 9 || gStrCopies != 1) fail |= 1;
    PushBack(v, a);
    PushBack(v, a);
    if (v.Finish != storage + 2 || gOverflow != 1 || gFill != 1 || !gAtEnd || gStrCopies != 2) fail |= 2;
    if (fail) { std::printf("008268f0_TEST FAIL %d\n", fail); return 1; }
    std::printf("008268f0_TEST PASS\n");
    return 0;
}