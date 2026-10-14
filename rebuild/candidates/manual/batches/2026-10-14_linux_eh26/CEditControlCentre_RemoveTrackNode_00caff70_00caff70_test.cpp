#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_rel = 0, g_ref = 0; struct Nd { Nd* prev; Nd* next; void* data; }; struct Rf; typedef void (*PFR)(Rf** s); static void Rel(Rf** s) { ++g_rel; } struct Rf { void* a; void* b; PFR rel; }; struct Tn { unsigned char p[0xc]; Rf* ref; unsigned char q[0x20]; long sec; };
struct Lst { Nd* head; Nd* tail; long n; unsigned char p[0xc]; void* Head() { return head; } void* At(void* pos) { return pos ? ((Nd*)pos)->data : 0; } void* Next(void*& pos) { Nd* x = (Nd*)pos; pos = x->next; return x->data; } void Remove(void* pos) { Nd* x = (Nd*)pos; if (x->prev) x->prev->next = x->next; else head = x->next; if (x->next) x->next->prev = x->prev; else tail = x->prev; --n; }
  void* Find(void* d, void*& pos) { for (pos = Head(); pos; Next(pos)) if (At(pos) == d) return pos; return 0; } };
struct X { unsigned char p[8]; unsigned char sec[0x18]; Lst a; Lst b; long Refresh() { return ++g_ref; } long Remove(Tn* t) { L l(sec); long r = 0; Lst* ls = t->sec ? &b : &a; void* pos; if (!ls->Find(t, pos)) r = 0x80040216; else { ls->Remove(pos); t->ref->rel(&t->ref); Refresh(); } return r; } };
static bool Check() { X x; Tn t1, t2; Rf rf; rf.rel = Rel; t1.ref = &rf; t2.ref = &rf; t1.sec = 0; t2.sec = 1; Nd n1, n2; n1.prev = 0; n1.next = &n2; n1.data = &t2; n2.prev = &n1; n2.next = 0; n2.data = &t1; x.a.head = &n1; x.a.tail = &n2; x.a.n = 2; x.b.head = 0; x.b.tail = 0; x.b.n = 0; if (x.Remove(&t2) != (long)0x80040216 || g_rel) return false; if (x.Remove(&t1) != 0 || g_rel != 1 || g_ref != 1 || x.a.n != 1 || x.a.head != &n1 || x.a.tail != &n1) return false; return x.Remove(&t1) == (long)0x80040216 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00caff70_TEST FAIL\n"); return 1; }
    std::printf("00caff70_TEST PASS\n");
    return 0;
}
