#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_released = 0; static void Release(void* b) { g_released = b; }
struct CAlloc { virtual void v00() { } unsigned char _pad[0xc30 - sizeof(void*)]; virtual ~CAlloc() { } static void operator delete(void* b) { Release(b); } static void* operator new(size_t n) { return std::malloc(n); } };

int main() {
    CAlloc* a = new CAlloc; delete a; if (g_released != a) { std::printf("00a66600_TEST FAIL\n"); return 1; }
    std::printf("00a66600_TEST PASS\n"); return 0;
}
