// Behaviour test for 0x0050EA20: links the row object's ?ShareData@?$CCountedPointer@VCMap@@@@QAEXPAVCMap@@PAVCCPPointerInfo@@@Z.
// The shared engine/CCountedPointer.h with the member declaration-only (FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY), so the call is an external
// reference the row defines. T stays incomplete: the count block's deleter receives the data as void*.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CMap;
static int g_deletes = 0;
static void* g_deleted = 0;
static void FABLE_FASTCALL TestDelete(void* data) { ++g_deletes; g_deleted = data; }
int main()
{
    static int objA, objB;
    CMap* a = reinterpret_cast<CMap*>(&objA);
    CMap* b = reinterpret_cast<CMap*>(&objB);
    static char buf[sizeof(CCountedPointer<CMap>)];
    CCountedPointer<CMap>* p = reinterpret_cast<CCountedPointer<CMap>*>(buf);
    CCPPointerInfo* first = new CCPPointerInfo(1, TestDelete, (void*)a);
    CCPPointerInfo* second = new CCPPointerInfo(1, TestDelete, (void*)b);
    p->Data = a; p->Info = first;
    p->ShareData(a, first);                       // same block: nothing changes
    if (first->RefCount != 1 || p->Data != a || p->Info != first) return 1;
    p->ShareData(b, second);                      // last reference on the old block: deleter on a; adopt b, count 2
    if (g_deletes != 1 || g_deleted != (void*)a || p->Data != b || p->Info != second || second->RefCount != 2) return 2;
    p->ShareData(0, 0);                           // old block keeps a reference: no delete; pointer empties
    if (g_deletes != 1 || second->RefCount != 1 || p->Data != 0 || p->Info != 0) return 3;
    delete second;
    std::printf("PASS_0050ea20\n");
    return 0;
}
