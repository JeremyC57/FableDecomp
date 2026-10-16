#include <cstdio>
struct CComp { void* blocking; };
struct CThing { bool dead; bool hasComp; CComp* comp; unsigned char flags1c9; };
struct CTCWife {
    CThing* parent; CThing* hero; long attitude; bool blocked;
    bool Get() const {
        if (!blocked && attitude != 4 && (parent->flags1c9 & 8)) { CThing* h = hero; if (h && !h->dead) { if (h->hasComp) { if (!h->comp->blocking) return true; } } }
        return false;
    }
};
int main()
{
    CComp free_ = { 0 }, busy = { &free_ };
    CThing p = { false, false, 0, 8 }, p0 = { false, false, 0, 0 }, h = { false, true, &free_, 0 }, hb = { false, true, &busy, 0 }, hd = { true, true, &free_, 0 };
    CTCWife a = { &p, &h, 0, false }, b = { &p, &hb, 0, false }, c = { &p, &hd, 0, false }, d = { &p, &h, 4, false }, e = { &p, &h, 0, true }, f = { &p0, &h, 0, false }, g = { &p, 0, 0, false };
    if (!a.Get() || b.Get() || c.Get() || d.Get() || e.Get() || f.Get() || g.Get()) { std::printf("006cf810_TEST FAIL\n"); return 1; }
    std::printf("006cf810_TEST PASS\n");
    return 0;
}