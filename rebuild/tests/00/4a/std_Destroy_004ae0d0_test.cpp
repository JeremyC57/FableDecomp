// Behaviour test for 004ae0d0 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
#include <new>
static int g_Released = 0;
struct CCountedPointer { int* Data; ~CCountedPointer() { ++g_Released; } };
struct Pair { unsigned int first; CCountedPointer second; };
template <class It> void Destroy(It first, It last) { for (; first != last; ++first) first->~Pair(); }
int main() {
    unsigned char buf[sizeof(Pair) * 3]; Pair* p = (Pair*)(void*)buf;
    for (int i = 0; i < 3; ++i) { new (p + i) Pair(); p[i].first = i; }
    Destroy(p, p + 3);
    if (g_Released != 3) { std::printf("004ae0d0_TEST FAIL\n"); return 1; }
    Destroy(p, p);
    if (g_Released != 3) { std::printf("004ae0d0_TEST FAIL\n"); return 1; }
    { std::printf("004ae0d0_TEST PASS\n"); return 0; }
}