// Behaviour test for 0050b550 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
#include <cstdlib>
template <class T> class Alloc { public: T* allocate(unsigned int n, const void* = 0) { return n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0; } };
int main() {
    Alloc<unsigned short> a;
    if (a.allocate(0) != 0) { std::printf("0050b550_TEST FAIL\n"); return 1; }
    unsigned short* p = a.allocate(8); if (!p) { std::printf("0050b550_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i) p[i] = (unsigned short)(i * 3);
    if (p[7] != 21) { std::printf("0050b550_TEST FAIL\n"); return 1; }
    std::free(p);
    { std::printf("0050b550_TEST PASS\n"); return 0; }
}