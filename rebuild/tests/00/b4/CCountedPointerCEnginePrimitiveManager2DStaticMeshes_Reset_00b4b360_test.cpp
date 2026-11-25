// Behaviour test for 0x00B4B360: links the row object's ?Reset@?$CCountedPointer@VCEnginePrimitiveManager2DStaticMeshes@@@@QAEXPAVCEnginePrimitiveManager2DStaticMeshes@@@Z.
// The shared engine/CCountedPointer.h with Reset declaration-only (FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL, so the call below is an
// external reference the row defines) and DeleteData declaration-only (defined here for T, the row stores its address).
// T stays incomplete: Reset only stores the pointer, the deleter receives it as void*.
#include <cstdio>
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CEnginePrimitiveManager2DStaticMeshes;
static int g_deletes = 0;
static const void* g_deleted = 0;
template <> void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManager2DStaticMeshes>::DeleteData(void* data) { ++g_deletes; g_deleted = data; }
int main()
{
    static int objA, objB;                               // stand-in storage: T is only declared
    CEnginePrimitiveManager2DStaticMeshes* a = reinterpret_cast<CEnginePrimitiveManager2DStaticMeshes*>(&objA);
    CEnginePrimitiveManager2DStaticMeshes* b = reinterpret_cast<CEnginePrimitiveManager2DStaticMeshes*>(&objB);
    CCountedPointer<CEnginePrimitiveManager2DStaticMeshes> p; p.Data = 0; p.Info = 0;
    p.Reset(a);                                         // adopt a: fresh block, count 1, deleter stored
    if (p.Data != a || !p.Info || p.Info->RefCount != 1 || p.Info->Data != (void*)a || p.Info->DeleteFunc != &CCountedPointer<CEnginePrimitiveManager2DStaticMeshes>::DeleteData) return 1;
    CCPPointerInfo* first = p.Info;
    first->RefCount = 2;                                // a second sharer: Reset only drops our reference
    p.Reset(b);
    if (first->RefCount != 1 || g_deletes != 0 || p.Data != b || !p.Info || p.Info == first) return 2;
    p.Reset(0);                                         // last reference on b: deleter runs on b, pointer empties
    if (g_deletes != 1 || g_deleted != (void*)b || p.Data != 0 || p.Info != 0) return 3;
    delete first;
    p.Data = 0; p.Info = 0;                             // the header has no destructor body to run here
    std::printf("00b4b360_TEST PASS\n");
    return 0;
}
