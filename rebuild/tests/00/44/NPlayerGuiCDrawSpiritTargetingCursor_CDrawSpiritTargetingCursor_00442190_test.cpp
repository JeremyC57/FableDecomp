// behaviour test for 0x00442190 NPlayerGui::CDrawSpiritTargetingCursor::~CDrawSpiritTargetingCursor() (implicit):
// destroys each targeted-thing pointer (virtual dtor) and frees the vector, then the CDrawBase handles likewise
#include <cstdio>
#include <cstdlib>
#include <new>
static int gPtrDtors = 0, gHandleDtors = 0, gFrees = 0;
static void Free(void* p) { if (p) { ++gFrees; std::free(p); } }
struct Handle { virtual ~Handle() { ++gHandleDtors; } void* data; };
struct IntelPtr { virtual ~IntelPtr() { ++gPtrDtors; } void* PData; };
template <class T> struct Vec { T* start; T* finish; T* eos;
    Vec() : start(0), finish(0), eos(0) { }
    void Fill(int n) { start = (T*)std::malloc(n * sizeof(T)); for (int i = 0; i < n; ++i) new (&start[i]) T(); finish = eos = start + n; }
    ~Vec() { for (T* p = start; p != finish; ++p) p->~T(); Free(start); } };
struct DrawBase { void* fields[6]; Vec<Handle> PrimitiveHandles; };
struct Cursor : DrawBase { Vec<IntelPtr> PTargetedThings; };
int main()
{
    int fails = 0;
    { Cursor c; c.PrimitiveHandles.Fill(1); c.PTargetedThings.Fill(3); }
    if (gPtrDtors != 3 || gHandleDtors != 1 || gFrees != 2) fails |= 1;
    { Cursor e; }
    if (gFrees != 2) fails |= 2;
    if (fails) { std::printf("00442190_TEST FAIL %d\n", fails); return 1; }
    std::printf("00442190_TEST PASS\n");
    return 0;
}