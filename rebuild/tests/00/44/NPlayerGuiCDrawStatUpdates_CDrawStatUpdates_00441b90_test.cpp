// behaviour test for 0x00441b90 NPlayerGui::CDrawStatUpdates::~CDrawStatUpdates() (implicit): frees PreviousStats, then
// the CDrawBase part destroys each primitive handle (virtual dtor) and frees their storage; no vtable is stored
#include <cstdio>
#include <cstdlib>
#include <new>
static int gHandleDtors = 0, gFrees = 0;
static void Free(void* p) { if (p) { ++gFrees; std::free(p); } }
struct Handle { virtual ~Handle() { ++gHandleDtors; } void* data; };
template <class T> struct Vec { T* start; T* finish; T* eos;
    Vec() : start(0), finish(0), eos(0) { }
    ~Vec() { for (T* p = start; p != finish; ++p) p->~T(); Free(start); } };
struct DrawBase { void* fields[6]; Vec<Handle> PrimitiveHandles; };
struct DrawStatUpdates : DrawBase { long LastRenown, LastMorality; bool Initialised; Vec<long> PreviousStats; };
int main()
{
    int fails = 0;
    {
        DrawStatUpdates d;
        d.PrimitiveHandles.start = (Handle*)std::malloc(2 * sizeof(Handle));
        new (&d.PrimitiveHandles.start[0]) Handle(); new (&d.PrimitiveHandles.start[1]) Handle();
        d.PrimitiveHandles.finish = d.PrimitiveHandles.eos = d.PrimitiveHandles.start + 2;
        d.PreviousStats.start = (long*)std::malloc(3 * sizeof(long));
        d.PreviousStats.finish = d.PreviousStats.eos = d.PreviousStats.start + 3;
    }
    if (gHandleDtors != 2 || gFrees != 2) fails |= 1;
    { DrawStatUpdates e; }
    if (gFrees != 2) fails |= 2;
    if (fails) { std::printf("00441b90_TEST FAIL %d\n", fails); return 1; }
    std::printf("00441b90_TEST PASS\n");
    return 0;
}