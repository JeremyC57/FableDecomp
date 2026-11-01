// behaviour test for 0x004427D0 deque<...>::~deque (model: elements destroyed, then the blocks freed)
#include <cstdio>
#include <deque>
static int g_dtors = 0;
struct E { int v; E(int x) : v(x) { } ~E() { ++g_dtors; } };
int main()
{
    { std::deque<E> d; d.push_back(E(1)); d.push_back(E(2)); g_dtors = 0; }
    int ok = g_dtors == 2;
    if (!ok) { std::printf("004427d0_TEST FAIL\n"); return 1; }
    std::printf("004427d0_TEST PASS\n");
    return 0;
}