#include <cstdio>
// Behaviour model of NInventory::CCategory::operator= (0x006255B0): ref-counted def pointer reassignment + memberwise copy.
struct Def { long refs; int deleted; };
struct DefPtr {
    Def* p;
    DefPtr& operator=(const DefPtr& o)
    {
        if (p != o.p) {
            Def* q = p;
            if (q) { if (--q->refs == 0) q->deleted = 1; p = 0; }
            p = o.p;
            if (p) ++p->refs;
        }
        return *this;
    }
};
struct Cat { void* thing; DefPtr def; long a, b, c; bool s, d;
    Cat& operator=(const Cat& o) { thing = o.thing; def = o.def; a = o.a; b = o.b; c = o.c; s = o.s; d = o.d; return *this; } };
int main()
{
    Def d1; d1.refs = 1; d1.deleted = 0; Def d2; d2.refs = 1; d2.deleted = 0;
    Cat x; x.thing = 0; x.def.p = &d1; x.a = x.b = x.c = 0; x.s = x.d = false;
    Cat y; y.thing = &x; y.def.p = &d2; y.a = 1; y.b = 2; y.c = 3; y.s = true; y.d = true;
    Cat& r = (x = y);
    if (&r != &x || !d1.deleted || d2.refs != 2 || x.def.p != &d2 || x.a != 1 || x.c != 3 || !x.s || !x.d || x.thing != &x) { std::printf("006255b0_TEST FAIL\n"); return 1; }
    x = y;
    if (d2.refs != 2) { std::printf("006255b0_TEST FAIL\n"); return 1; }
    std::printf("006255b0_TEST PASS\n");
    return 0;
}
