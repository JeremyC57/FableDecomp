#include <cstdio>
// Behaviour model of CTCHero::GetTitleGuiText (0x006CA0A0): text of the title tag, else empty; the entry is released.
struct Entry { long refs; int text; int destroyed; };
struct Def { long tag; };
static Entry g_entry;
static Entry* Lookup(long tag) { if (tag == 7) { ++g_entry.refs; return &g_entry; } return 0; }
static int GetTitle(const Def* def)
{
    if (def && def->tag > 0) {
        Entry* e = Lookup(def->tag);
        if (e) { int t = e->text; if (--e->refs == 0) e->destroyed = 1; return t; }
    }
    return 0;
}
int main()
{
    g_entry.refs = 1; g_entry.text = 42; g_entry.destroyed = 0;
    Def d; d.tag = 7;
    if (GetTitle(&d) != 42 || g_entry.refs != 1) { std::printf("006ca0a0_TEST FAIL\n"); return 1; }
    d.tag = 0; if (GetTitle(&d) != 0) { std::printf("006ca0a0_TEST FAIL\n"); return 1; }
    d.tag = 3; if (GetTitle(&d) != 0 || GetTitle(0) != 0) { std::printf("006ca0a0_TEST FAIL\n"); return 1; }
    std::printf("006ca0a0_TEST PASS\n");
    return 0;
}
