// Behaviour test for 0x00CDD4F0: links the row object's ??1?$CCountedPointer@$$CBVCPredicateBase@NScript@@@@QAE@XZ.
// The shared engine/CCountedPointer.h with the member declaration-only (FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY), so the call is an external
// reference the row defines. T stays incomplete: the count block's deleter receives the data as void*.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NScript { class CPredicateBase; }
static int g_deletes = 0;
static void* g_deleted = 0;
static void FABLE_FASTCALL TestDelete(void* data) { ++g_deletes; g_deleted = data; }
int main()
{
    static int objA, objB;
    const NScript::CPredicateBase* a = reinterpret_cast<const NScript::CPredicateBase*>(&objA);
    const NScript::CPredicateBase* b = reinterpret_cast<const NScript::CPredicateBase*>(&objB);
    static char buf[sizeof(CCountedPointer<const NScript::CPredicateBase>)];
    CCountedPointer<const NScript::CPredicateBase>* p = reinterpret_cast<CCountedPointer<const NScript::CPredicateBase>*>(buf);
    CCPPointerInfo* shared = new CCPPointerInfo(2, TestDelete, (void*)a);
    p->Data = a; p->Info = shared;
    p->CCountedPointer<const NScript::CPredicateBase>::~CCountedPointer();                                 // a second sharer remains: only the count drops
    if (shared->RefCount != 1 || g_deletes != 0 || p->Data != 0 || p->Info != 0) return 1;
    p->Data = a; p->Info = shared;
    p->CCountedPointer<const NScript::CPredicateBase>::~CCountedPointer();                                 // last reference: the deleter runs on Data, the block is freed
    if (g_deletes != 1 || g_deleted != (void*)a || p->Data != 0 || p->Info != 0) return 2;
    p->Data = 0; p->Info = 0;
    p->CCountedPointer<const NScript::CPredicateBase>::~CCountedPointer();                                 // empty pointer: nothing to release
    if (g_deletes != 1 || p->Data != 0 || p->Info != 0) return 3;
    std::printf("PASS_00cdd4f0\n");
    return 0;
}
