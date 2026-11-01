#include <cstdio>
#include <cstring>
struct Rec { int worldKey; int entryKey; };
struct Group { Rec* rec; int entry; Group(Rec* r, int e) : rec(r), entry(e) { } };
static int Lookup(int w) { return w > 0 ? w * 10 : 0; }
static Group* Init(Rec* r) { int e = Lookup(r->worldKey); if (e) return new Group(r, e); return 0; }
static bool Run() { Rec a = { 2, 0 }, b = { 0, 0 }; Group* g = Init(&a); bool ok = g && g->entry == 20 && g->rec == &a && Init(&b) == 0; delete g; return ok; }
int main()
{
    if (!Run()) { std::printf("00c096e0_TEST FAIL\n"); return 1; }
    std::printf("00c096e0_TEST PASS\n");
    return 0;
}