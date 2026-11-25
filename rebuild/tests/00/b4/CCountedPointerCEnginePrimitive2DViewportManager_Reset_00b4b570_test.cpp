// Behaviour test for 0x00B4B570: links the row object's ?Reset@?$CCountedPointer@VCEnginePrimitive2DViewportManager@@@@QAEXPAVCEnginePrimitive2DViewportManager@@@Z.
// The shared engine/CCountedPointer.h with Reset declaration-only (FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL, so the call below is an
// external reference the row defines) and DeleteData declaration-only (defined here for T, the row stores its address).
// T stays incomplete: Reset only stores the pointer, the deleter receives it as void*.
#include <cstdio>
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CEnginePrimitive2DViewportManager;
static int g_deletes = 0;
static const void* g_deleted = 0;
template <> void FABLE_FASTCALL CCountedPointer<CEnginePrimitive2DViewportManager>::DeleteData(void* data) { ++g_deletes; g_deleted = data; }
int main()
{
    static int objA, objB;                               // stand-in storage: T is only declared
    CEnginePrimitive2DViewportManager* a = reinterpret_cast<CEnginePrimitive2DViewportManager*>(&objA);
    CEnginePrimitive2DViewportManager* b = reinterpret_cast<CEnginePrimitive2DViewportManager*>(&objB);
    CCountedPointer<CEnginePrimitive2DViewportManager> p; p.Data = 0; p.Info = 0;
    p.Reset(a);                                         // adopt a: fresh block, count 1, deleter stored
    if (p.Data != a || !p.Info || p.Info->RefCount != 1 || p.Info->Data != (void*)a || p.Info->DeleteFunc != &CCountedPointer<CEnginePrimitive2DViewportManager>::DeleteData) return 1;
    CCPPointerInfo* first = p.Info;
    first->RefCount = 2;                                // a second sharer: Reset only drops our reference
    p.Reset(b);
    if (first->RefCount != 1 || g_deletes != 0 || p.Data != b || !p.Info || p.Info == first) return 2;
    p.Reset(0);                                         // last reference on b: deleter runs on b, pointer empties
    if (g_deletes != 1 || g_deleted != (void*)b || p.Data != 0 || p.Info != 0) return 3;
    delete first;
    p.Data = 0; p.Info = 0;                             // the header has no destructor body to run here
    std::printf("00b4b570_TEST PASS\n");
    return 0;
}
