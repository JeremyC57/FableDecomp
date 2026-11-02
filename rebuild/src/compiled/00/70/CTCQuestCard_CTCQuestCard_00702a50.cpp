// CTCQuestCard::~CTCQuestCard() -- implicit destructor: the five CCharString members in reverse order, the PDef def-pointer release (count down, Delete at zero, clear), then ~CTCBase
// Leaf: ??1CTCQuestCard@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00702A50, 75 bytes (+ int3 padding), thiscall; tail `jmp ~CTCBase` 0x00686830.
// Identity (proven, two sources): the only caller 0x00702A33 is the scalar deleting destructor 0x00702A30, slot 0 of vtable
//   0x01264C44 (RTTI .?AVCTCQuestCard@@ : CTCBase), stored by the ctor 0x007026A0; FableWin.pdb ??1CTCQuestCard@@UAE@XZ
//   (0x025534C0, right after ??_G 0x02553480; Ego_r inlines it). It stores no vtable of its own: the compiler-generated
//   destructor. Ego_r.pdb members (no STL container, so the offsets are retail's and the ctor 0x007026A0 confirms them): PDef
//   (CDefPointer<const CQuestCardDef>) +0x0C, Finished +0x10, Replayable +0x11, FeatIndex +0x14, QuestName +0x18,
//   ObjectiveName +0x1C, RegionName +0x20, TeleporterRegionName +0x24, ScriptQuestName +0x28, GoldReward +0x2C, RenownReward
//   +0x30; sizeof 0x34. The catalog label "CTCDExplosion::~CTCDExplosion" is propagated and wrong.
// Callees: ??1CCharString@@QAE@XZ 0x0099EAE0 (x5), the def's virtual slot 1 Delete() (CDefPointeeBase, through the vtable),
//   ??1CTCBase@@UAE@XZ 0x00686830 (tail).
// Shape: engine/CDefPointer.h's inline ~CDefPointer = Release(): `if (Object) { if (--count == 0) Delete(); Object = 0; }` -- the
//   `dec dword ptr [ecx+4] ; jne` form with the clearing store inside the non-null arm (as chunk 710's 0x007E0B20). The PDB
//   constructor ??0CTCQuestCard@@QAE@AAVCThing@@@Z is defined (body not reconstructed) so the TU emits the vtable, ??_G and this
//   ??1, as retail's TU does.
// Layout: engine/CTCQuestCard.h retail opt-in FABLE_RETAIL_CTCQUESTCARD (hdr/ overlay, chunk 713: : CTCBase, real CDefPointer and
//   CCharString members); engine/CTCBase.h (FABLE_RETAIL_CTCBASE) and engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCQUESTCARD
#include "engine/CTCQuestCard.h"

CTCQuestCard::CTCQuestCard(CThing& thing)                  // 0x007026A0 (emission only: body not reconstructed here)
    : CTCBase(thing)
{
}