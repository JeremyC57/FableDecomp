#include <cstdio>
#include <cstdlib>
#include <new>
struct E { virtual ~E() {} long pri; long owner; unsigned char active; };
struct R { long r, g, b, a; };
struct D : E { R col; };
struct N { N* next; N* prev; D value; };
struct L { N* BuyNode(const D& v) { N* n = (N*)std::malloc(sizeof(N)); new (&n->value) D(v); return n; } };
int main()
{
    D d; d.pri = 3; d.owner = 4; d.active = 1; d.col.r = 10; d.col.a = 40;
    L l; N* n = l.BuyNode(d);
    if (!n || n->value.pri != 3 || n->value.owner != 4 || n->value.active != 1 || n->value.col.r != 10 || n->value.col.a != 40) { std::printf("00883f10_TEST FAIL\n"); return 1; }
    std::free(n);
    std::printf("00883f10_TEST PASS\n");
    return 0;
}
