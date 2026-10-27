// behaviour test for 0x00CBA3B0: the placement copy shares the string and add-refs the counted block
#include <cstdio>
#include <new>
struct Info { long RefCount; };
struct Rec { const char* name; void* data; Info* info;
    Rec(const Rec& o) : name(o.name), data(o.data), info(o.info) { if (info) ++info->RefCount; }
    Rec(const char* n, void* d, Info* i) : name(n), data(d), info(i) { } };
static void Construct(Rec* p, const Rec& v) { new (p) Rec(v); }
int main()
{
    Info info = { 1 }; int payload; Rec src("bank", &payload, &info);
    union { double align; char raw[sizeof(Rec)]; } slot;
    Rec* p = (Rec*)slot.raw; Construct(p, src);
    Rec bare("x", 0, 0); union { double align; char raw[sizeof(Rec)]; } slot2; Construct((Rec*)slot2.raw, bare);
    if (info.RefCount != 2 || p->data != &payload || p->info != &info || ((Rec*)slot2.raw)->info != 0) { std::printf("00cba3b0_TEST FAIL\n"); return 1; }
    std::printf("00cba3b0_TEST PASS\n");
    return 0;
}