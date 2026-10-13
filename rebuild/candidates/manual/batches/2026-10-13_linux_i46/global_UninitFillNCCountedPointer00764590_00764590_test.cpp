#include <cstdio>
#include <new>
struct B { long r; }; struct P { void* o; B* c; P(const P& x) : o(x.o), c(x.c) { if (c) ++c->r; } }; struct A {};
static P* __fastcall UF(P* f, unsigned long n, const P& v, A) { for (; 0 < n; --n, ++f) new (f) P(v); return f; }
int main()
{
    B b; b.r = 1; unsigned char vb[8]; P* v = (P*)vb; v->o = 0; v->c = &b; unsigned char buf[24]; P* e = UF((P*)buf, 3, *v, A()); if (e != (P*)(buf + 24) || b.r != 4 || ((P*)buf)[2].c != &b) { std::printf("00764590_TEST FAIL\n"); return 1; }

    std::printf("00764590_TEST PASS\n");
    return 0;
}
