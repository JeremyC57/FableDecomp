#include <cstdio>
#include <cstring>
static int g_base = 0, g_added = 0; static char g_name[64];
struct Str { const char* p; Str(const char* t, long l) : p(t) {} ~Str() {} };
struct Hit { unsigned char pad[0x18]; unsigned char a, b; Hit() : a(0), b(0) {} };
static Hit g_hit; static Hit* g_result = &g_hit;
struct Thing { unsigned char pad[0x20]; unsigned long bits[9];
    bool HasInterface(long t) { return (bits[t >> 5] & (1UL << (t & 31))) != 0; }
    Hit* AddTC(const Str& n, bool act, void* p) { ++g_added; std::strcpy(g_name, n.p); return g_result; } };
struct Ptr { virtual ~Ptr() {} Thing* p; Thing* GetPItem() const { return p; } };
struct Base { virtual ~Base() {} virtual void Initialise() { ++g_base; } Ptr PCreature; };
struct Act : Base { void Initialise() {
    if (!PCreature.GetPItem()->HasInterface(200)) { Hit* h; { Str n("CTCHitOnCollision", -1); h = PCreature.GetPItem()->AddTC(n, false, 0); } if (h) { h->a = 1; h->b = 1; } }
    Base::Initialise(); } };
int main()
{
    Thing t; for (int i = 0; i < 9; ++i) t.bits[i] = 0; Act a; a.PCreature.p = &t;
    a.Initialise();
    if (g_added != 1 || g_hit.a != 1 || g_hit.b != 1 || std::strcmp(g_name, "CTCHitOnCollision") != 0) { std::printf("008c4250_TEST FAIL\n"); return 1; }
    g_result = 0; a.Initialise();            // null result tolerated
    t.bits[6] = 0x100; a.Initialise();       // already present: no add
    if (g_added != 2 || g_base != 3) { std::printf("008c4250_TEST FAIL\n"); return 1; }
    std::printf("008c4250_TEST PASS\n");
    return 0;
}