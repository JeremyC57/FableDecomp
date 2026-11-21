// Behaviour test for 0x0051F0E0 CThingManager::CopyHeroToHero (links the REAL row object). Both things are raw retail-layout objects:
// the interface presence bitset<274> at +0x20 (bit t of word t/32) and the CThing interface CVectorMap at +0x44 (CArray begin / end /
// end-of-storage of sorted pair<ETCInterfaceType, CTCBase*>), searched by the row object's own CVectorMap::LowerBound copies. The
// components are fake objects; slot 12 (+0x30) is the virtual Copy. CThing::AddAndCopyTC and CTCHeroMorph::KillParticles are stubs under
// the row's mangled names. Every subset of the seven copied interface types (3 appearance morph, 4 hero stats, 0x5E attachable appearance
// modifiers, 0x5C basic graphic appearance, 0x68 hero experience, 0x6F hero abilities, 0x14 inventory stats) is tried on the loaded hero,
// among unrelated components; the call log is checked against an independent model of the copy order.
#include <stdio.h>
#include <string.h>
namespace std {
template<class T> class allocator {};
template<class T, class A = allocator<T> > class vector { public: T* s; T* f; T* e; };
}
class CTCBase;
class CThing { public:
    void AddAndCopyTC(const CTCBase& tc, bool flag);
};
class CTCHeroMorph { public:
    void KillParticles();
};
struct Runner;
class CThingManager {
    friend struct Runner;
    void CopyHeroToHero(CThing& loaded_hero, CThing& hero, std::vector<CThing*>& loaded_things);
};
static int g_fail = 0;
static char g_log[4096]; static int g_len = 0;
static void Log(const char* s) { int n = (int)strlen(s); if (g_len + n + 2 < (int)sizeof(g_log)) { memcpy(g_log + g_len, s, n); g_len += n; g_log[g_len++] = ' '; g_log[g_len] = 0; } }
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d\n", line); } }
struct FakeTC;
static FakeTC* g_tcs[2][256];                     // [0] loaded hero's components, [1] live hero's, by interface type
static unsigned char g_src[0x100], g_dst[0x100];  // the two things
static int Owner(const void* p) { return p == g_src ? 0 : p == g_dst ? 1 : -1; }
struct FakeTC {
    virtual void s0() {} virtual void s1() {} virtual void s2() {} virtual void s3() {} virtual void s4() {} virtual void s5() {}
    virtual void s6() {} virtual void s7() {} virtual void s8() {} virtual void s9() {} virtual void s10() {} virtual void s11() {}
    virtual void Copy(const FakeTC& from)           // slot 12
    {
        char b[48]; sprintf(b, "copy:%d.%d<-%d.%d", Who, Type, from.Who, from.Type); Log(b);
    }
    int Who, Type;
};
static const char* Name(const void* tc)
{
    static char b[16];
    for (int w = 0; w != 2; ++w) for (int t = 0; t != 256; ++t) if (g_tcs[w][t] == tc) { sprintf(b, "%d.%d", w, t); return b; }
    return "?";
}
void CThing::AddAndCopyTC(const CTCBase& tc, bool flag)
{
    char b[48]; sprintf(b, "add:%d:%s:%d", Owner(this), Name(&tc), flag ? 1 : 0); Log(b);
}
void CTCHeroMorph::KillParticles() { char b[32]; sprintf(b, "kill:%s", Name(this)); Log(b); }
struct Entry { int type; FakeTC* tc; };
static FakeTC g_store[2][16];
static Entry g_maps[2][16];
static void Build(unsigned char* thing, int who, const int* types, int n)
{
    memset(thing, 0xCC, 0x100);
    memset(thing + 0x20, 0, 9 * 4);
    for (int i = 0; i != n; ++i) {                 // types[] is sorted
        int t = types[i];
        thing[0x20 + t / 8] |= (unsigned char)(1 << (t % 8));
        g_store[who][i].Who = who; g_store[who][i].Type = t;
        g_tcs[who][t] = &g_store[who][i];
        g_maps[who][i].type = t; g_maps[who][i].tc = &g_store[who][i];
    }
    Entry* b = g_maps[who]; Entry* e = g_maps[who] + n;
    memcpy(thing + 0x44, &b, 4); memcpy(thing + 0x48, &e, 4); memcpy(thing + 0x4C, &e, 4);
}
static const int kTypes[7] = { 3, 4, 0x5E, 0x5C, 0x68, 0x6F, 0x14 };
static void Model(int mask, char* out)
{
    char b[48]; out[0] = 0;
    if (mask & 1) { strcat(out, "kill:1.3 add:1:0.3:1 "); }
    for (int i = 1; i != 5; ++i) if (mask & (1 << i)) { sprintf(b, "add:1:0.%d:1 ", kTypes[i]); strcat(out, b); }
    if (mask & 32) strcat(out, "copy:1.111<-0.111 ");
    if (mask & 64) strcat(out, "copy:1.20<-0.20 ");
}
struct Runner {
    static void Run(int mask, int line)
    {
        memset(g_tcs, 0, sizeof(g_tcs));
        int st[16], sn = 0;                        // loaded hero: the masked types among unrelated ones (1, 50, 200)
        int cand[10] = { 1, 3, 4, 0x14, 50, 0x5C, 0x5E, 0x68, 0x6F, 200 };
        for (int i = 0; i != 10; ++i) {
            int t = cand[i]; bool keep = (t == 1 || t == 50 || t == 200);
            for (int k = 0; k != 7; ++k) if (kTypes[k] == t && (mask & (1 << k))) keep = true;
            if (keep) st[sn++] = t;
        }
        Build(g_src, 0, st, sn);
        int dt[6] = { 2, 3, 0x14, 0x40, 0x6F, 0x70 };  // live hero: the three components it is asked for, among others
        Build(g_dst, 1, dt, 6);
        unsigned char mgr[0x100]; memset(mgr, 0, sizeof(mgr));
        CThing* things[1] = { (CThing*)g_src };
        std::vector<CThing*> v; v.s = things; v.f = things + 1; v.e = things + 1;
        g_len = 0; g_log[0] = 0;
        ((CThingManager*)mgr)->CopyHeroToHero(*(CThing*)g_src, *(CThing*)g_dst, v);
        char want[1024]; Model(mask, want);
        Check(!strcmp(g_log, want), line);
        if (strcmp(g_log, want)) printf("  mask %d\n  got  %s\n  want %s\n", mask, g_log, want);
    }
};
int main()
{
    for (int mask = 0; mask != 128; ++mask) Runner::Run(mask, __LINE__);
    if (!g_fail) printf("0051f0e0_TEST PASS\n");
    return g_fail;
}
