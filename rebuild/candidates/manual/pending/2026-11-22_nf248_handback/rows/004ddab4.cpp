// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90). Spellings unified: 00a01c10: ??1CBaseIntelligentPointer@@QAE@XZ -> ??1CBaseIntelligentPointer@@UAE@XZ
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?SetPTargetedThing@CTCTargetingBase@@UAEXPAVCThing@@@Z
// CTCTargetingBase::SetPTargetedThing(CThing*) -- wraps the thing in a temporary CIntelligentPointer<CThing> (catalog / table spelling of 0x004A96F0),
// copies its item into PTargetedThing through SetPItem, then destroys the temporary.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DDAB4, length 49 bytes; frame-pointer size TU (`push ebp ; mov ebp,esp ;
// push ecx ; push ecx` for the 8-byte temporary, `leave`): `optimize("y",off)` + `("s",on)`.
// Callees: CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, CBaseIntelligentPointer::SetPItem(CBaseObject*) @ 0x00A01B90, ~CBaseIntelligentPointer @ 0x00A01C10.
// PDB CTCTargetingBase (sizeof 0x38): +0x25 CIntelligentPointer<CThing> PTargetedThing (the odd offset follows the
// bool at +0x24 of the packed PDB layout and the retail body addresses `this+25h`). The body sits among the
// CTCTargetingAI methods (vtable 0x012411FC slot 34); the manifest label "CCategory::SetCategoryThingAssociation" is
// propagated and rejected.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#include "engine/CTCBase.h"
#include "engine/CTCTargetingBase.h"

// Name: FableWin + Ego_r S_PUB32 ?SetPTargetedThing@CTCTargetingBase@@UAEXPAVCThing@@@Z (public virtual); the binder 0x00802E00
// (CTCTargetingPlayer::SetPTargetedThing) already calls it by that name. The header keeps PTargetedThing (+0x25,
// CIntelligentPointer<CThing>) opaque: it is viewed as its CBaseIntelligentPointer.
void CTCTargetingBase::SetPTargetedThing(CThing* thing)
{
    CIntelligentPointer<CThing> pointer(thing);
    reinterpret_cast<CBaseIntelligentPointer*>(PTargetedThing)->SetPItem(pointer.GetPItem());
}
