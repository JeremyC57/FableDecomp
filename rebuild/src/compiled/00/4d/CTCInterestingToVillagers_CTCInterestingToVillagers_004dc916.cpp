// CTCInterestingToVillagers::~CTCInterestingToVillagers() -- interesting-to-villagers component dtor (compiler-generated): release the def pointer, then ~CTCBase
// Leaf: ??1CTCInterestingToVillagers@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DC916, 32 bytes, thiscall; size-optimised TU (unaligned, `and [esi+0Ch],0`), ends `jmp ~CTCBase`.
// Identity (agentFAM06): the only caller is 0x004DC8FA, slot 0 (scalar deleting destructor) of the CTCInterestingToVillagers RTTI
//   vtable 0x012402F4; Ego_r.pdb / FableWin.pdb ??1CTCInterestingToVillagers@@UAE@XZ; Ego_r.pdb CTCInterestingToVillagers is 16
//   bytes with PDef (CDefPointer<const CInterestingToVillagersDef>) at +0x0C, the member released here; the tail jump goes to
//   ~CTCBase 0x00686830. The catalog label CIVCountedPointer_Release and the landed leaf ?dtor@@YIXPAVC3DAnimationBlendState@@@Z
//   are placeholders.
// Body: the shared engine/CTCInterestingToVillagers.h under the new opt-in FABLE_RETAIL_CTCINTERESTINGTOVILLAGERS_IMPLICIT_DTOR:
//   retail stores no vtable, which is the compiler-generated destructor (a user-declared one stores it first: graded 38 v 32).
//   The helper below only makes VC7.1 emit the implicit ??1 COMDAT; it is not a retail function.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINTERESTINGTOVILLAGERS_IMPLICIT_DTOR
#include "rebuild_abi.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInterestingToVillagers.h"

void EmitInterestingToVillagersDestructor(CTCInterestingToVillagers* p)   // emits the implicit ??1; not a retail function
{
    p->CTCInterestingToVillagers::~CTCInterestingToVillagers();
}
