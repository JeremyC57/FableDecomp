#include <cstdio>
// CBaseOutputPin::InitAllocator -> CreateMemoryAllocator -> CoCreateInstance(CLSID_MemoryAllocator, 0, CLSCTX_INPROC_SERVER, IID_IMemAllocator, ppAlloc)
static const int CLSID_MemoryAllocator = 1, IID_IMemAllocator = 2;
static const int* gClsid; static const int* gIid; static void* gOuter; static unsigned gCtx; static void** gPpv; static int gAlloc;
int CoCreateInstance(const int& clsid, void* outer, unsigned ctx, const int& iid, void** ppv)
{ gClsid = &clsid; gOuter = outer; gCtx = ctx; gIid = &iid; gPpv = ppv; *ppv = &gAlloc; return 0; }
inline int CreateMemoryAllocator(void** pp) { return CoCreateInstance(CLSID_MemoryAllocator, 0, 1, IID_IMemAllocator, pp); }
struct OutPin { int InitAllocator(void** ppAlloc) { return CreateMemoryAllocator(ppAlloc); } };
int main()
{
    OutPin p; void* a = 0;
    if (p.InitAllocator(&a) != 0 || a != &gAlloc || gClsid != &CLSID_MemoryAllocator || gIid != &IID_IMemAllocator || gOuter != 0 || gCtx != 1 || gPpv != &a) {
        std::printf("00ca86b0_TEST FAIL\n"); return 1;
    }
    std::printf("00ca86b0_TEST PASS\n");
    return 0;
}