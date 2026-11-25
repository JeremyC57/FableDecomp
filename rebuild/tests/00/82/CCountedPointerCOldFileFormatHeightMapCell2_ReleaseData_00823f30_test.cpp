// Behaviour test for 0x00823F30: links the row object's ?ReleaseData@?$CCountedPointer@VCOldFileFormatHeightMapCell2@@@@QAEXXZ.
// The shared engine/CCountedPointer.h with the member declaration-only (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY), so the call is an external
// reference the row defines. T stays incomplete: the count block's deleter receives the data as void*.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class COldFileFormatHeightMapCell2;
static int g_deletes = 0;
static void* g_deleted = 0;
static void FABLE_FASTCALL TestDelete(void* data) { ++g_deletes; g_deleted = data; }
int main()
{
    static int objA, objB;
    COldFileFormatHeightMapCell2* a = reinterpret_cast<COldFileFormatHeightMapCell2*>(&objA);
    COldFileFormatHeightMapCell2* b = reinterpret_cast<COldFileFormatHeightMapCell2*>(&objB);
    static char buf[sizeof(CCountedPointer<COldFileFormatHeightMapCell2>)];
    CCountedPointer<COldFileFormatHeightMapCell2>* p = reinterpret_cast<CCountedPointer<COldFileFormatHeightMapCell2>*>(buf);
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
    std::printf("PASS_00823f30\n");
    return 0;
}
