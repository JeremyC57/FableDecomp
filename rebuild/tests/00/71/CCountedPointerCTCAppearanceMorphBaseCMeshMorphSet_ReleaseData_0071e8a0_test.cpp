// Behaviour test for 0x0071E8A0: links the row object's ?ReleaseData@?$CCountedPointer@VCMeshMorphSet@CTCAppearanceMorphBase@@@@QAEXXZ.
// The shared engine/CCountedPointer.h with the member declaration-only (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY), so the call is an external
// reference the row defines. T stays incomplete: the count block's deleter receives the data as void*.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace CTCAppearanceMorphBase { class CMeshMorphSet; }
static int g_deletes = 0;
static void* g_deleted = 0;
static void FABLE_FASTCALL TestDelete(void* data) { ++g_deletes; g_deleted = data; }
int main()
{
    static int objA, objB;
    CTCAppearanceMorphBase::CMeshMorphSet* a = reinterpret_cast<CTCAppearanceMorphBase::CMeshMorphSet*>(&objA);
    CTCAppearanceMorphBase::CMeshMorphSet* b = reinterpret_cast<CTCAppearanceMorphBase::CMeshMorphSet*>(&objB);
    static char buf[sizeof(CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>)];
    CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>* p = reinterpret_cast<CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>*>(buf);
    CCPPointerInfo* shared = new CCPPointerInfo(2, TestDelete, (void*)a);
    p->Data = a; p->Info = shared;
    p->ReleaseData();                                 // a second sharer remains: only the count drops
    if (shared->RefCount != 1 || g_deletes != 0 || p->Data != 0 || p->Info != 0) return 1;
    p->Data = a; p->Info = shared;
    p->ReleaseData();                                 // last reference: the deleter runs on Data, the block is freed
    if (g_deletes != 1 || g_deleted != (void*)a || p->Data != 0 || p->Info != 0) return 2;
    p->Data = 0; p->Info = 0;
    p->ReleaseData();                                 // empty pointer: nothing to release
    if (g_deletes != 1 || p->Data != 0 || p->Info != 0) return 3;
    std::printf("PASS_0071e8a0\n");
    return 0;
}
