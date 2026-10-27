#include <cstdio>
#include <new>
// Behaviour model of the range erase over polymorphic elements: assign the tail down, destroy the vacated ones.
static int g_destroyed;
struct E { int v; virtual ~E() { ++g_destroyed; } E& operator=(const E& o) { v = o.v; return *this; } };
static E* erase(E*& finish, E* first, E* last)
{
    E* r = first;
    for (int n = (int)(finish - last); n > 0; --n) { *r = *last; ++r; ++last; }
    for (E* p = r; p != finish; ++p) p->~E();
    finish = r;
    return first;
}
int main()
{
    E a[5]; for (int i = 0; i < 5; ++i) a[i].v = i;
    E* finish = a + 5;
    E* r = erase(finish, a + 1, a + 3);
    if (r != a + 1 || finish != a + 3 || a[0].v != 0 || a[1].v != 3 || a[2].v != 4 || g_destroyed != 2) { std::printf("00f26500_TEST FAIL\n"); return 1; }
    for (int i = 3; i < 5; ++i) new (&a[i]) E();                // re-arm the destroyed slots for the automatic destructors
    std::printf("00f26500_TEST PASS\n");
    return 0;
}