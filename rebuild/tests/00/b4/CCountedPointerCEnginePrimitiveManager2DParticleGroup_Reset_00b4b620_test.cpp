// Behaviour test for 0x00B4B620: links the row object's ?Reset@?$CCountedPointer@VCEnginePrimitiveManager2DParticleGroup@@@@QAEXPAVCEnginePrimitiveManager2DParticleGroup@@@Z.
// The shared engine/CCountedPointer.h with Reset declaration-only (FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL, so the call below is an
// external reference the row defines) and DeleteData declaration-only (defined here for T, the row stores its address).
// T stays incomplete: Reset only stores the pointer, the deleter receives it as void*.
#include <cstdio>
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CEnginePrimitiveManager2DParticleGroup;
static int g_deletes = 0;
static const void* g_deleted = 0;
template <> void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManager2DParticleGroup>::DeleteData(void* data) { ++g_deletes; g_deleted = data; }
int main()
{
    static int objA, objB;                               // stand-in storage: T is only declared
    CEnginePrimitiveManager2DParticleGroup* a = reinterpret_cast<CEnginePrimitiveManager2DParticleGroup*>(&objA);
    CEnginePrimitiveManager2DParticleGroup* b = reinterpret_cast<CEnginePrimitiveManager2DParticleGroup*>(&objB);
    CCountedPointer<CEnginePrimitiveManager2DParticleGroup> p; p.Data = 0; p.Info = 0;
    p.Reset(a);                                         // adopt a: fresh block, count 1, deleter stored
    if (p.Data != a || !p.Info || p.Info->RefCount != 1 || p.Info->Data != (void*)a || p.Info->DeleteFunc != &CCountedPointer<CEnginePrimitiveManager2DParticleGroup>::DeleteData) return 1;
    CCPPointerInfo* first = p.Info;
    first->RefCount = 2;                                // a second sharer: Reset only drops our reference
    p.Reset(b);
    if (first->RefCount != 1 || g_deletes != 0 || p.Data != b || !p.Info || p.Info == first) return 2;
    p.Reset(0);                                         // last reference on b: deleter runs on b, pointer empties
    if (g_deletes != 1 || g_deleted != (void*)b || p.Data != 0 || p.Info != 0) return 3;
    delete first;
    p.Data = 0; p.Info = 0;                             // the header has no destructor body to run here
    std::printf("00b4b620_TEST PASS\n");
    return 0;
}
