#include <cstdio>
#include <cstring>
#include <new>
static long g_type = 0; static unsigned long g_flags = 0;
class CIVCountedPointeeBase { public: virtual void v00() { } unsigned long IVRefCount; };
struct CIVCountedPointer { CIVCountedPointeeBase* Object; CIVCountedPointer() : Object(0) { } };
struct CBase { virtual void v00() { } unsigned char _pad04[0x44]; CBase(long type, unsigned long flags) { g_type = type; g_flags = flags; } };
struct COccBase : CBase { CIVCountedPointer OcclusionInfo[2]; COccBase(long t, unsigned long f) : CBase(t, f) { } };
struct CGroup : COccBase {
    void* GridCell; void* Grid; unsigned long GridX, GridY, FramesRendered;
    CGroup(void* cell, void* grid, unsigned long x, unsigned long y) : COccBase(0x29, 8), GridCell(cell), Grid(grid), GridX(x), GridY(y), FramesRendered(0) { }
};
int main()
{
    unsigned char raw[sizeof(CGroup)]; std::memset(raw, 0xee, sizeof(raw));
    int cell, grid;
    CGroup* g = new (raw) CGroup(&cell, &grid, 5, 9);
    if (g_type != 0x29 || g_flags != 8 || g->GridCell != &cell || g->Grid != &grid || g->GridX != 5 || g->GridY != 9 || g->FramesRendered != 0 ||
        g->OcclusionInfo[0].Object != 0 || g->OcclusionInfo[1].Object != 0) { std::printf("00bacc00_TEST FAIL\n"); return 1; }
    std::printf("00bacc00_TEST PASS\n"); return 0;
}