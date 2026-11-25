// Behaviour test for 0x005105F0: links the row object's ?ReleaseData@?$CCountedPointer@VCOldFileFormatMapHeader3@@@@QAEXXZ.
// The shared engine/CCountedPointer.h with the member declaration-only (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY), so the call is an external
// reference the row defines. T stays incomplete: the count block's deleter receives the data as void*.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class COldFileFormatMapHeader3;
static int g_deletes = 0;
static void* g_deleted = 0;
static void FABLE_FASTCALL TestDelete(void* data) { ++g_deletes; g_deleted = data; }
int main()
{
    static int objA, objB;
    COldFileFormatMapHeader3* a = reinterpret_cast<COldFileFormatMapHeader3*>(&objA);
    COldFileFormatMapHeader3* b = reinterpret_cast<COldFileFormatMapHeader3*>(&objB);
    static char buf[sizeof(CCountedPointer<COldFileFormatMapHeader3>)];
    CCountedPointer<COldFileFormatMapHeader3>* p = reinterpret_cast<CCountedPointer<COldFileFormatMapHeader3>*>(buf);
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
    std::printf("PASS_005105f0\n");
    return 0;
}
