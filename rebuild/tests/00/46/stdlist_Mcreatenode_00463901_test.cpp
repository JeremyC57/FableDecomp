// behaviour test for 0x00463901: the node holds a copy of x at +8
#include <cstdio>
#include <cstdlib>
#include <new>
struct E { unsigned int a, b; };
struct N { N* next; N* prev; E data; };
static N* Create(const E& x) { N* p = (N*)std::malloc(sizeof(N)); new (&p->data) E(x); return p; }
int main()
{
    E x = { 7, 9 }; N* n = Create(x);
    if ((char*)&n->data - (char*)n != 2 * sizeof(void*) || n->data.a != 7 || n->data.b != 9) { std::printf("00463901_TEST FAIL\n"); return 1; }
    std::free(n);
    std::printf("00463901_TEST PASS\n");
    return 0;
}