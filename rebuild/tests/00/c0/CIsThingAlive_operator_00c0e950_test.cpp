#include <cstdio>
struct V { virtual bool Is(const void* const& t) { return t != 0; } };
struct P { unsigned char pad[0x74]; V v; bool operator()(void* t) { return v.Is(t); } };
int main()
{
    P p; int x; if (!p(&x) || p(0)) { std::printf("00c0e950_TEST FAIL\n"); return 1; }

    std::printf("00c0e950_TEST PASS\n");
    return 0;
}