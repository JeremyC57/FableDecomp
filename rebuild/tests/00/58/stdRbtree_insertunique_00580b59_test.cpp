// behaviour test for 0x00580B59 _Rb_tree::insert_unique(iterator, const value_type&) (map<CCharString, CTCHeroStats::MirroredCriminalStatus >): a hint that brackets v links the node next to it; a hint that does not falls back on the unhinted insert; an equal key at the hint returns the hint
#include <cstdio>
static int gLinks = 0, gFallbacks = 0;
struct It { int Pos; };                                          // position in a sorted array of keys (stand-in for the tree)
static int gKeys[16] = { 10, 20, 30, 40 }; static int gN = 4;
static bool Less(int a, int b) { return a < b; }
static It Link(int at, int v) { for (int i = gN; i > at; --i) gKeys[i] = gKeys[i - 1]; gKeys[at] = v; ++gN; ++gLinks; It r = { at }; return r; }
static It Unhinted(int v) { ++gFallbacks; int i = 0; while (i < gN && gKeys[i] < v) ++i; if (i < gN && gKeys[i] == v) { It r = { i }; return r; } return Link(i, v); }
static It InsertHint(It pos, int v)                              // the function under test, on the sorted-array model
{
    if (pos.Pos == 0) {                                          // begin()
        if (gN <= 0) return Unhinted(v);
        if (Less(v, gKeys[0])) return Link(0, v);
        if (!Less(gKeys[0], v)) return pos;
        if (gN == 1 || Less(v, gKeys[1])) return Link(1, v);
        return Unhinted(v);
    }
    if (pos.Pos == gN) {                                         // end()
        if (Less(gKeys[gN - 1], v)) return Link(gN, v);
        return Unhinted(v);
    }
    bool vPos = Less(v, gKeys[pos.Pos]);
    if (vPos && Less(gKeys[pos.Pos - 1], v)) return Link(pos.Pos, v);
    bool posV = !vPos;
    if (!vPos) posV = Less(gKeys[pos.Pos], v);
    if (!vPos && posV && (pos.Pos + 1 == gN || Less(v, gKeys[pos.Pos + 1]))) return Link(pos.Pos + 1, v);
    if (vPos == posV) return pos;
    return Unhinted(v);
}
int main()
{
    int fail = 0;
    It p0 = { 0 }, p2 = { 2 }, pe = { 4 };
    It r = InsertHint(p0, 5);  if (r.Pos != 0 || gKeys[0] != 5 || gLinks != 1 || gFallbacks != 0) fail |= 1;
    r = InsertHint(p2, 15);    if (r.Pos != 2 || gKeys[2] != 15 || gLinks != 2 || gFallbacks != 0) fail |= 2;
    pe.Pos = gN; r = InsertHint(pe, 50); if (r.Pos != 6 || gKeys[6] != 50 || gLinks != 3) fail |= 4;
    It p3 = { 3 }; r = InsertHint(p3, 20); if (r.Pos != 3 || gLinks != 3 || gFallbacks != 0) fail |= 8;
    It p1 = { 1 }; r = InsertHint(p1, 45); if (gFallbacks != 1 || gKeys[6] != 45 || gLinks != 4) fail |= 16;
    for (int i = 1; i < gN; ++i) if (gKeys[i - 1] >= gKeys[i]) fail |= 32;
    if (fail) { std::printf("00580b59_TEST FAIL %d\n", fail); return 1; }
    std::printf("00580b59_TEST PASS\n");
    return 0;
}