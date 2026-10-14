#include <cstdio>
#include <new>
static int g_qi = 0, g_rel = 0, g_acc = 0, g_sync = 0, g_queue = 0; static long g_accepts = 0; static long g_syncr = 0; static long g_queuer = 7; static void* g_req = 0; static bool g_have = true;
struct Gd { unsigned long a; unsigned short b, c; unsigned char d[8]; }; static Gd g_iid;
struct Iu { virtual long QI(const Gd& i, void** o) = 0; virtual unsigned long AddRef() = 0; virtual unsigned long Release() = 0; };
struct Is : Iu { virtual long Accepts(void* r) = 0; }; struct Ie : Iu { virtual long Queue(void* s, long a, void* r, long b, void* c, long k) = 0; };
struct St : Is { long QI(const Gd& i, void** o) { return 0; } unsigned long AddRef() { return 1; } unsigned long Release() { ++g_rel; return 0; } long Accepts(void* r) { ++g_acc; return g_accepts; } }; static St g_st;
struct Src : Iu { long QI(const Gd& i, void** o) { ++g_qi; *o = g_have ? (void*)&g_st : 0; return 0; } unsigned long AddRef() { return 1; } unsigned long Release() { return 0; } };
struct En : Ie { long QI(const Gd& i, void** o) { return 0; } unsigned long AddRef() { return 1; } unsigned long Release() { return 0; } long Queue(void* s, long a, void* r, long b, void* c, long k) { ++g_queue; g_req = r; return g_queuer; } };
struct Sp { Is* p; Sp() : p(0) {} ~Sp() { if (p) p->Release(); } };
struct Bk { unsigned char pad[0xc]; unsigned char slot[0xc]; Iu* src; unsigned char pad2[0xb4]; void* ctx; Ie* eng; long LoadSync(void* r) { ++g_sync; return g_syncr; } long LoadAsync(void* r) { if (!src) return (long)0x80040209; Sp s; src->QI(g_iid, (void**)&s.p); if (s.p) { if (!s.p->Accepts(r)) { long res = LoadSync(r); if (res < 0) return res; return 0; } } long res; if (eng && ctx) res = eng->Queue(slot, 0, r, 0, ctx, 2); else res = (long)0x80004005; return res; } };
static bool Check() { Bk* b = new Bk; Src src; En en; int r, c; b->src = 0; if (b->LoadAsync(&r) != (long)0x80040209) return false; b->src = &src; b->eng = &en; b->ctx = &c; if (b->LoadAsync(&r) != 0 || g_sync != 1 || g_rel != 1 || g_acc != 1) return false; g_syncr = -3; if (b->LoadAsync(&r) != -3 || g_rel != 2) return false; g_accepts = 1; if (b->LoadAsync(&r) != 7 || g_queue != 1 || g_req != &r || g_rel != 3) return false; b->eng = 0; if (b->LoadAsync(&r) != (long)0x80004005) return false; g_have = false; b->eng = &en; bool ok = b->LoadAsync(&r) == 7 && g_rel == 4 && g_queue == 2; delete b; return ok; }

int main()
{
    if (!Check()) { std::printf("00cab990_TEST FAIL\n"); return 1; }
    std::printf("00cab990_TEST PASS\n");
    return 0;
}