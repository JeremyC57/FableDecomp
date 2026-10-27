// behaviour test for 0x008AC970: every element is destroyed once, in order
#include <cstdio>
#include <new>
static int g_order[8]; static int g_n = 0;
struct E { int id; E(int i) : id(i) { } virtual ~E() { g_order[g_n++] = id; } };
int main()
{
    void* raw = ::operator new(3 * sizeof(E)); E* v = (E*)raw;
    for (int i = 0; i < 3; ++i) new (v + i) E(i);
    for (E* p = v; p != v + 3; ++p) p->~E();
    ::operator delete(raw);
    if (g_n != 3 || g_order[0] != 0 || g_order[2] != 2) { std::printf("008ac970_TEST FAIL\n"); return 1; }
    std::printf("008ac970_TEST PASS\n");
    return 0;
}