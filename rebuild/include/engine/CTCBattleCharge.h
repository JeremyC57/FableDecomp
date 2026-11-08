#ifndef FABLE_ENGINE_CTCBATTLECHARGE_H
#define FABLE_ENGINE_CTCBATTLECHARGE_H
// CTCBattleCharge (new, chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): the hero's battle-charge
// component (TCI_BATTLE_CHARGE 168). Ego_r.pdb (struct_layouts_egor.tsv, 152 bytes there): PTarget CIntelligentPointer<CThing> +0x0C
// (12 bytes there, 8 in retail: every later member -4), PDef +0x14, PositionStartedFrom +0x18, ..., IsZTargetting (bool, 140 there;
// retail +0x88: the row tests `[eax+88h]` then compares `PTarget.GetPItem()` at +0x0C with the targeted thing). Only those two are
// modelled; the rest is opaque. Retail sizeof 0x94 (152 - 4 - 16: the one pointer and the PeopleHit vector, 16 -> 12).
// Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_CINTELLIGENTPOINTER.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ETCInterfaceType.h"
class CThing;
#pragma pack(push, 1)
class CTCBattleCharge : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_BATTLE_CHARGE; }
    // PLACEHOLDER accessor names (no PDB on disk lists a getter; the row reads the members as inline tests):
    const CIntelligentPointer<CThing>& GetPTarget() const { return PTarget; }
    bool GetIsZTargetting() const { return IsZTargetting; }
private:
    CIntelligentPointer<CThing> PTarget;                                 // +0x0c
    unsigned char _unmodelled_0x14[0x88 - 0x14];                         // +0x14 PDef .. PeopleHit (not modelled)
    bool IsZTargetting;                                                  // +0x88
    unsigned char _unmodelled_0x89[0x94 - 0x89];                         // +0x89 .. +0x93
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CTCBattleCharge) == 0x94);
// IsZTargetting at +0x88: 0x0C + 8 (PTarget) + (0x88 - 0x14) (opaque run); private, so no offsetof assert (VC7.1 refuses it).
#endif
