// Behaviour test for 0x008C32B0: links the row object's ??1?$CCountedPointer@VCModeJackOfBladesFlash@NCreatureMode@@@@QAE@XZ.
// The shared engine/CCountedPointer.h with the member declaration-only (FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY), so the call is an external
// reference the row defines. T stays incomplete: the count block's deleter receives the data as void*.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NCreatureMode { class CModeJackOfBladesFlash; }
static int g_deletes = 0;
static void* g_deleted = 0;
static void FABLE_FASTCALL TestDelete(void* data) { ++g_deletes; g_deleted = data; }
int main()
{
    static int objA, objB;
    NCreatureMode::CModeJackOfBladesFlash* a = reinterpret_cast<NCreatureMode::CModeJackOfBladesFlash*>(&objA);
    NCreatureMode::CModeJackOfBladesFlash* b = reinterpret_cast<NCreatureMode::CModeJackOfBladesFlash*>(&objB);
    static char buf[sizeof(CCountedPointer<NCreatureMode::CModeJackOfBladesFlash>)];
    CCountedPointer<NCreatureMode::CModeJackOfBladesFlash>* p = reinterpret_cast<CCountedPointer<NCreatureMode::CModeJackOfBladesFlash>*>(buf);
    CCPPointerInfo* shared = new CCPPointerInfo(2, TestDelete, (void*)a);
    p->Data = a; p->Info = shared;
    p->CCountedPointer<NCreatureMode::CModeJackOfBladesFlash>::~CCountedPointer();                                 // a second sharer remains: only the count drops
    if (shared->RefCount != 1 || g_deletes != 0 || p->Data != 0 || p->Info != 0) return 1;
    p->Data = a; p->Info = shared;
    p->CCountedPointer<NCreatureMode::CModeJackOfBladesFlash>::~CCountedPointer();                                 // last reference: the deleter runs on Data, the block is freed
    if (g_deletes != 1 || g_deleted != (void*)a || p->Data != 0 || p->Info != 0) return 2;
    p->Data = 0; p->Info = 0;
    p->CCountedPointer<NCreatureMode::CModeJackOfBladesFlash>::~CCountedPointer();                                 // empty pointer: nothing to release
    if (g_deletes != 1 || p->Data != 0 || p->Info != 0) return 3;
    std::printf("PASS_008c32b0\n");
    return 0;
}
