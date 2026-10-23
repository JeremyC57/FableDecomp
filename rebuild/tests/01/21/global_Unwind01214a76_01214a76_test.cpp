#include <cstdio>
#include <new>
#include <cstdlib>
static int g_q = 0, g_cur = 0, g_stop = 0, g_fin = 0, g_det = 0, g_unreg = 0, g_take = 0, g_dsink = 0, g_d2c = 0, g_d60 = 0, g_d94 = 0, g_dcc = 0, g_dreg = 0, g_d138 = 0, g_dbase = 0; static bool g_qr = false, g_qf = false; static void* g_detp = 0; static void* g_unregp = 0; static void* g_taken = 0; static long g_qa = -1;
struct It { long m0, m4, m8; }; static It g_item;
struct Sk2 { virtual void Take(void* i) { ++g_take; g_taken = i; } virtual void s4() {} virtual void s8() {} virtual void sc() {} virtual void s10() {} virtual void s14() {} virtual ~Sk2() { ++g_dsink; } };
struct P2c { unsigned char b[0x34]; ~P2c() { ++g_d2c; } }; struct P60 { unsigned char b[0x34]; ~P60() { ++g_d60; } }; struct P94 { unsigned char b[0x38]; ~P94() { ++g_d94; } }; struct Pcc { unsigned char b[0xc]; ~Pcc() { ++g_dcc; } };
struct Rg { unsigned char b[0x3c]; void Detach(void* e) { ++g_det; g_detp = e; } void Unregister(void* e) { ++g_unreg; g_unregp = e; } ~Rg() { ++g_dreg; } }; struct P138 { unsigned char b[0x34]; ~P138() { ++g_d138; } };
struct Bs { unsigned char z[0xc]; unsigned char entry[0x20]; ~Bs() { ++g_dbase; } };
struct Rc : Bs { P2c m2c; P60 m60; P94 m94; Pcc mcc; unsigned char d8[0x20]; long state; Rg reg; P138 m138; Sk2* sink; bool Query(bool* f, long a) { ++g_q; g_qa = a; *f = g_qf; return g_qr; } It* Current() { ++g_cur; g_item.m8 = 5; return &g_item; } void Stop() { ++g_stop; } void Final() { ++g_fin; }
    ~Rc() { if (state == 1) { bool flag; if (Query(&flag, 0) || flag) { reg.Detach(d8); Stop(); } state = 0; } if (state == 3) { It* item = Current(); item->m8 = 0; sink->Take(item); state = 2; } if (state == 2) { reg.Detach(d8); state = 4; } if (state == 4) { Stop(); if (sink) delete sink; sink = 0; state = 0; } if (state == 0) { reg.Unregister(entry); state = 5; } if (state == 5) { Final(); state = 6; } } };
static Rc* Mk(long st, bool withSink) { Rc* r = (Rc*)std::calloc(1, sizeof(Rc)); new (r) Rc; r->state = st; r->sink = withSink ? new Sk2 : 0; return r; }
static bool Check() { Rc* r = Mk(1, false); r->~Rc(); std::free(r); if (g_q != 1 || g_qa != 0 || g_det || g_stop || g_unreg != 1 || g_unregp != (void*)r->entry || g_fin != 1 || g_cur || g_take || g_d2c != 1 || g_d60 != 1 || g_d94 != 1 || g_dcc != 1 || g_dreg != 1 || g_d138 != 1 || g_dbase != 1) return false;
    g_qf = true; r = Mk(1, false); r->~Rc(); std::free(r); if (g_q != 2 || g_det != 1 || g_detp != (void*)r->d8 || g_stop != 1 || g_unreg != 2 || g_fin != 2) return false;
    g_qf = false; g_qr = true; r = Mk(1, false); r->~Rc(); std::free(r); if (g_q != 3 || g_det != 2 || g_stop != 2 || g_unreg != 3 || g_fin != 3) return false;
    r = Mk(3, true); r->~Rc(); std::free(r); if (g_cur != 1 || g_item.m8 != 0 || g_take != 1 || g_taken != &g_item || g_det != 3 || g_stop != 3 || g_dsink != 1 || g_unreg != 4 || g_fin != 4 || g_q != 3) return false;
    r = Mk(2, false); r->~Rc(); std::free(r); if (g_cur != 1 || g_det != 4 || g_stop != 4 || g_dsink != 1 || g_unreg != 5 || g_fin != 5) return false;
    r = Mk(4, true); r->~Rc(); std::free(r); if (g_det != 4 || g_stop != 5 || g_dsink != 2 || g_unreg != 6 || g_fin != 6) return false;
    r = Mk(5, true); r->~Rc(); if (g_stop != 5 || g_unreg != 6 || g_fin != 7 || g_dsink != 2) return false; delete r->sink; std::free(r);
    r = Mk(6, false); r->~Rc(); std::free(r); return g_q == 3 && g_stop == 5 && g_unreg == 6 && g_fin == 7 && g_d2c == 8 && g_d60 == 8 && g_d94 == 8 && g_dcc == 8 && g_dreg == 8 && g_d138 == 8 && g_dbase == 8; }

int main()
{
    if (!Check()) { std::printf("00c38b20_TEST FAIL\n"); return 1; }
    std::printf("01214a76_TEST PASS\n");
    return 0;
}