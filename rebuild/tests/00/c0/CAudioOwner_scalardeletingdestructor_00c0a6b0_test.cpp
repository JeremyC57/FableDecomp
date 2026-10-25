#include <cstdio>
#include <cstdlib>
#include <new>
static int g_dtor = 0, g_freed = 0;
static void* g_lastFreed = 0;
void operator delete(void* p) { ++g_freed; g_lastFreed = p; std::free(p); }
void* operator new(std::size_t n) { return std::malloc(n); }
struct CAudioOwner {
    virtual void v00() {}
    virtual ~CAudioOwner() { ++g_dtor; }
};
int main()
{
    CAudioOwner* p = new CAudioOwner;
    void* raw = p;
    delete p;                                    // virtual: through the scalar deleting destructor, flags = 1
    { CAudioOwner local; }                       // plain destructor, no free
    if (g_dtor != 2 || g_freed != 1 || g_lastFreed != raw) { std::printf("00c0a6b0_TEST FAIL\n"); return 1; }
    std::printf("00c0a6b0_TEST PASS\n");
    return 0;
}
