#include <cstdio>
#include <new>
static int g_lk = 0; static unsigned char g_sec[8]; struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } };
static int g_open = 0, g_apply = 0, g_bld = 0; static unsigned long g_special = 0, g_specialv = 0; static void* g_oscript = 0; static void* g_octx = 0; static unsigned long g_ostart = 0, g_oend = 0, g_olimit = 0, g_otag = 0; static unsigned short g_okind = 0; static void* g_handle = 0; static const void* g_bptr = 0; static long g_ret = 77;
struct Pm { unsigned short kind; unsigned long limit; void* ctx; long tag; unsigned long off, len; };
struct Rg { unsigned long limit, start, end; unsigned short kind, e; long tag; unsigned long Convert(unsigned long o) { return o == g_special ? g_specialv : o; } };
struct Rc { long m0, m4, m8, mc, m10, m14, m18; unsigned short m1c, m1e; long m20; unsigned char m24; long m28; };
struct Bd { long v[12]; Bd() { ++g_bld; for (int i = 0; i < 12; ++i) v[i] = -1; } void Set0(long x) { v[0] = x; } void Set4(long x) { v[1] = x; } void Set8(long x) { v[2] = x; } void SetC(long x) { v[3] = x; } void Set10(long x) { v[4] = x; } void Set14(long x) { v[5] = x; } void Set18(long x) { v[6] = x; } void Set1C(unsigned short x) { v[7] = x; } void Set1E(unsigned short x) { v[8] = x; } void Set20(long x) { v[9] = x; } void Set24(unsigned char x) { v[10] = x; } void Set28(long x) { v[11] = x; } };
struct En { void* Open(const Rg* r, void* s, void* c) { ++g_open; g_ostart = r->start; g_oend = r->end; g_olimit = r->limit; g_okind = r->kind; g_otag = r->tag; g_oscript = s; g_octx = c; if (r->e || g_lk != 1) return 0; return (void*)0x77; } long Apply(void* h, const Bd* b) { ++g_apply; g_handle = h; g_bptr = b; if (g_lk != 1) return -1; for (int i = 0; i < 12; ++i) if (b->v[i] != i + 1) return -2; return g_ret; } };
struct Ld { long m0, m4; En* eng;
    long LoadScript(void* script, const Pm* p, const Rc* r) { Pl lock(g_sec); Rg range; range.limit = p->limit; range.start = 0; range.end = 0; range.kind = p->kind; range.e = 0; range.tag = p->tag; range.start = range.Convert(p->off); range.end = range.Convert(p->len + p->off); if (!(range.start < range.limit && range.end < range.limit && range.start <= range.end)) { range.end = 0; range.start = 0; } void* h = eng->Open(&range, script, p->ctx); Bd b; b.Set0(r->m0); b.Set4(r->m4); b.Set8(r->m8); b.SetC(r->mc); b.Set10(r->m10); b.Set14(r->m14); b.Set18(r->m18); b.Set1C(r->m1c); b.Set1E(r->m1e); b.Set20(r->m20); b.Set24(r->m24); b.Set28(r->m28); return eng->Apply(h, &b); } };
static bool Check() { En en; Ld l; l.eng = &en; int s, c; Pm p; p.kind = 9; p.limit = 100; p.ctx = &c; p.tag = 0x55; p.off = 10; p.len = 20; Rc r; r.m0 = 1; r.m4 = 2; r.m8 = 3; r.mc = 4; r.m10 = 5; r.m14 = 6; r.m18 = 7; r.m1c = 8; r.m1e = 9; r.m20 = 10; r.m24 = 11; r.m28 = 12;
    if (l.LoadScript(&s, &p, &r) != 77 || g_open != 1 || g_apply != 1 || g_bld != 1 || g_lk != 0 || g_ostart != 10 || g_oend != 30 || g_olimit != 100 || g_okind != 9 || g_otag != 0x55 || g_oscript != &s || g_octx != &c || g_handle != (void*)0x77) return false;
    p.limit = 30; if (l.LoadScript(&s, &p, &r) != 77 || g_ostart != 0 || g_oend != 0 || g_olimit != 30) return false;
    p.limit = 100; g_special = 30; g_specialv = 5; if (l.LoadScript(&s, &p, &r) != 77 || g_ostart != 0 || g_oend != 0) return false;
    g_special = 10; g_specialv = 99; p.off = 10; if (l.LoadScript(&s, &p, &r) != 77 || g_ostart != 0 || g_oend != 0) return false;
    g_special = 0; g_ret = -9; r.m28 = 13; bool ok = l.LoadScript(&s, &p, &r) == -2 && g_ostart == 10 && g_oend == 30 && g_open == 5 && g_apply == 5 && g_bld == 5 && g_lk == 0; r.m28 = 12; return ok && l.LoadScript(&s, &p, &r) == -9; }

int main()
{
    if (!Check()) { std::printf("00c16310_TEST FAIL\n"); return 1; }
    std::printf("01211cc0_TEST PASS\n");
    return 0;
}