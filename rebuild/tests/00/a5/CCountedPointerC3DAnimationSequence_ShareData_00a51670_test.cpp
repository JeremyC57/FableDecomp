// Behaviour test for 0x00A51670: links the row object's ?ShareData@?$CCountedPointer@VC3DAnimationSequence@@@@QAEXPAVC3DAnimationSequence@@PAVCCPPointerInfo@@@Z.
// The shared engine/CCountedPointer.h with the member declaration-only (FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY), so the call is an external
// reference the row defines. T stays incomplete: the count block's deleter receives the data as void*.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class C3DAnimationSequence;
static int g_deletes = 0;
static void* g_deleted = 0;
static void FABLE_FASTCALL TestDelete(void* data) { ++g_deletes; g_deleted = data; }
int main()
{
    static int objA, objB;
    C3DAnimationSequence* a = reinterpret_cast<C3DAnimationSequence*>(&objA);
    C3DAnimationSequence* b = reinterpret_cast<C3DAnimationSequence*>(&objB);
    static char buf[sizeof(CCountedPointer<C3DAnimationSequence>)];
    CCountedPointer<C3DAnimationSequence>* p = reinterpret_cast<CCountedPointer<C3DAnimationSequence>*>(buf);
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
    std::printf("PARITY_OK PASS\n");
    return 0;
}
