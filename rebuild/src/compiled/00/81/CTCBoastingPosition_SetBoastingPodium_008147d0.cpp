// CTCBoastingPosition::SetBoastingPodium(CThing& podium) -- remember the boasting podium: PBoastingPodium = &podium (a CIntelligentPointer<CThing> temporary of the podium assigned into the member)
// Leaf: ?SetBoastingPodium@CTCBoastingPosition@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008147D0, 71 bytes, thiscall `ret 4`.
// Names (agentFAM11, 2026-11-12; was _unattributed::AssignTarget_008147d0): FableWin.pdb ?SetBoastingPodium@CTCBoastingPosition@@QAEXAAVCThing@@@Z;
//   its only caller CTCBoastingPodium::OnInitialActivate 0x00814260 (call at 0x00814482, already bound to this spelling) calls it on the
//   TCI_BOASTING_POSITION component with GetParentThing(); Ego_r.pdb CTCBoastingPosition::PBoastingPodium (+0x0C) is CIntelligentPointer<CThing>.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10, SetPItem 0x00A01B90, GetPItem 0x00A01B50, dtor 0x00A01C10;
//   CIntelligentPointer<CThing> vtable 0x01238C6C.
// Shape: the converting temporary CIntelligentPointer<CThing>(&podium) (base ctor, vptr, SetPItem) assigned through the inline
//   operator= (`SetPItem(other.GetPItem())`, FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN), then the temporary's destructor.
class CThing;
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#define FABLE_RETAIL_CTCBOASTINGPOSITION_SETBOASTINGPODIUM
#define FABLE_RETAIL_CTCBOASTINGPOSITION_PBOASTINGPODIUM
#include "engine/CTCBoastingPosition.h"

void CTCBoastingPosition::SetBoastingPodium(CThing& podium)
{
    PBoastingPodium = &podium;
}
