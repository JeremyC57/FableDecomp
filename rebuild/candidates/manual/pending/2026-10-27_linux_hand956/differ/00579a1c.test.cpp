// behaviour test for 0x00579A1C: the list copy constructor copies every element in order into a fresh list
#include <cstdio>
struct N { N* next; N* prev; long v; };
struct L { N* node; L() { node = new N; node->next = node->prev = node; }
    void push_back(long v) { N* n = new N; n->v = v; n->next = node; n->prev = node->prev; node->prev->next = n; node->prev = n; }
    L(const L& x) { node = new N; node->next = node->prev = node; for (N* p = x.node->next; p != x.node; p = p->next) push_back(p->v); } };
int main()
{
    L a; a.push_back(3); a.push_back(1); a.push_back(4);
    L b(a); long want[3] = { 3, 1, 4 }; int i = 0, ok = b.node != a.node;
    for (N* p = b.node->next; p != b.node; p = p->next, ++i) ok = ok && i < 3 && p->v == want[i];
    L e; L f(e); ok = ok && i == 3 && f.node->next == f.node;
    if (!ok) { std::printf("00579a1c_TEST FAIL\n"); return 1; }
    std::printf("00579a1c_TEST PASS\n");
    return 0;
}
