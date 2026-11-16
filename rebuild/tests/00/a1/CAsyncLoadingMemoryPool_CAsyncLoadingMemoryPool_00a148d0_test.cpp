// Behaviour model of CAsyncLoadingMemoryPool::CAsyncLoadingMemoryPool 0x00A148D0: the CHandleBasedMemoryPool base is constructed once,
// then the object is a CAsyncLoadingMemoryPool (its own vtable: virtual calls through the base reach the derived class).
#include <cstdio>
static int g_base_ctors = 0;
struct HandleBasedMemoryPool {
    HandleBasedMemoryPool() { ++g_base_ctors; }
    virtual ~HandleBasedMemoryPool() { }
    virtual int Kind() const { return 1; }
};
struct AsyncLoadingMemoryPool : HandleBasedMemoryPool {
    AsyncLoadingMemoryPool() : HandleBasedMemoryPool() { }
    virtual int Kind() const { return 2; }
};
int main()
{
    AsyncLoadingMemoryPool pool;
    const HandleBasedMemoryPool& base = pool;
    if (g_base_ctors != 1) { std::printf("FAIL base\n"); return 1; }
    if (base.Kind() != 2) { std::printf("FAIL vtable\n"); return 1; }
    std::printf("00a148d0_TEST PASS\n");
    return 0;
}
