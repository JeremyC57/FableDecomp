#include <cstdio>
// Behaviour model: the parent is kept and the chain link cleared.
struct Sub { void* parent; Sub* next; Sub(void* p) : parent(p), next(0) { } };
int main()
{
    int prim = 0; Sub s(&prim);
    if (s.parent != &prim || s.next) { std::printf("00bace50_TEST FAIL\n"); return 1; }
    std::printf("00bace50_TEST PASS\n");
    return 0;
}