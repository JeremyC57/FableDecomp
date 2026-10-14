#include <cstdio>
#include <cstdlib>
#include <new>
struct S { const char* p; S(const char* t) : p(t) {} S(const S& o) : p(o.p) {} };
struct V { S a; S b; unsigned char f; long n; V(const char* x, const char* y, unsigned char ff, long nn) : a(x), b(y), f(ff), n(nn) {} V(const V& o) : a(o.a), b(o.b), f(o.f), n(o.n) {} };
struct N { N* next; N* prev; V value; };
struct It { N* node; It(N* n) : node(n) {} };
struct L { N* head;
    It Insert(It where, const V& v) { N* n = (N*)std::malloc(sizeof(N)); new (&n->value) V(v); n->next = where.node; n->prev = where.node->prev; where.node->prev->next = n; where.node->prev = n; return It(n); } };
int main()
{
    N& sentinel = *(N*)std::malloc(sizeof(N)); sentinel.next = &sentinel; sentinel.prev = &sentinel;
    L l; l.head = &sentinel; V v("k", "v", 7, 99);
    It it = l.Insert(It(&sentinel), v);
    N* n = it.node;
    if (n->next != &sentinel || n->prev != &sentinel || sentinel.next != n || sentinel.prev != n || n->value.f != 7 || n->value.n != 99 || n->value.a.p != v.a.p) { std::printf("004b99e0_TEST FAIL\n"); return 1; }
    It it2 = l.Insert(It(&sentinel), v);    // appends after n
    if (it2.node->prev != n || n->next != it2.node || sentinel.prev != it2.node) { std::printf("004b99e0_TEST FAIL\n"); return 1; }
    std::free(n); std::free(it2.node); std::free(&sentinel);
    std::printf("004b99e0_TEST PASS\n");
    return 0;
}
