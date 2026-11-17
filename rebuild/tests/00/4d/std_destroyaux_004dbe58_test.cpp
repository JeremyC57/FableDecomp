// Behaviour test for 0x004DBE58 std::__destroy_aux<NPlayerTargeting::CTargetPoint*>(first, last, const __false_type&):
// destroy every 28-byte CTargetPoint in [first, last) through its destructor (the intelligent pointer's ~CBaseIntelligentPointer
// 0x00A01C10 is the only non-trivial part), in order; an empty range does nothing.
#include <cstdio>

static int g_destroyed = 0;
static int g_order[8];

struct CBaseIntelligentPointer {                 // 8 bytes: vptr + data in retail; modelled as two words
    int   Vtable;
    int   PData;
    ~CBaseIntelligentPointer() { if (g_destroyed < 8) g_order[g_destroyed] = PData; ++g_destroyed; }
};
struct C3DVector { float X, Y, Z; };
namespace NPlayerTargeting {
struct CTargetPoint {                            // Ego_r.pdb: PThing +0, Index +8, TargetingPosition +0xC, FrameRejected +0x18
    CBaseIntelligentPointer PThing;
    int Index;
    C3DVector TargetingPosition;
    int FrameRejected;
};
}
struct __false_type { };

static void __destroy_aux(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, const __false_type&)
{
    for (; first != last; ++first) first->~CTargetPoint();
}

int main()
{
    static unsigned char raw[4 * sizeof(NPlayerTargeting::CTargetPoint)];
    NPlayerTargeting::CTargetPoint* arr = (NPlayerTargeting::CTargetPoint*)raw;
    for (int i = 0; i < 4; ++i) { arr[i].PThing.Vtable = 0; arr[i].PThing.PData = 10 + i; }
    __destroy_aux(arr + 1, arr + 3, __false_type());
    int mid = g_destroyed; bool order = g_order[0] == 11 && g_order[1] == 12;
    g_destroyed = 0;
    __destroy_aux(arr, arr, __false_type());
    int none = g_destroyed;
    bool ok = mid == 2 && order && none == 0 && sizeof(NPlayerTargeting::CTargetPoint) == 28;
    std::printf(ok ? "DESTROY_RANGE_OK\n" : "004dbe58_TEST FAIL\n");   // the catalogue PassPattern
    return ok ? 0 : 1;
}
