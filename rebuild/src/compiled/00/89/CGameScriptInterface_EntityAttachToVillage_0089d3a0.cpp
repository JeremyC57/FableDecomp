// CGameScriptInterface::EntityAttachToVillage(const CScriptThing& thing, const CScriptThing& village) const -- add a villager-capable thing (kind 0x23) to the village thing's village component (kind 0x22) when both script things are valid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089D3A0, length 124 bytes.
// Callees: the script things' slots 75 (+0x12c) and 11 (+0x2c), the component-map LowerBound @ 0x0040F020, CTCVillage::AddThingToVillage @ 0x0065B854.
// Leaf: ?EntityAttachToVillage@CGameScriptInterface@@UBEXABVCScriptThing@@0@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming (NF168, 2026-11-20): moved onto the shared headers (engine/CThing.h, engine/CTCVillage.h). The component call is
// CTCVillage::AddThingToVillage(CThing&) 0x0065B854 (Ego_r.pdb / FableWin.pdb ?AddThingToVillage@CTCVillage@@QAEXAAVCThing@@@Z), which
// replaces the CTCVillage_Retail / CThing_Retail stand-ins; interface kinds 0x23 / 0x22 are TCI_VILLAGE_MEMBER / TCI_VILLAGE.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#include "fable_compiler.h"
#include "engine/CThing.h"
#include "engine/CTCVillage.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntityAttachToVillage(const CScriptThing& thing, const CScriptThing& village) const
{
    if (thing.IsAlive() && village.IsAlive() && thing.GetPThing()->HasTC(TCI_VILLAGE_MEMBER)) {
        CTCVillage* tc;
        if (village.GetPThing()->GetTC(TCI_VILLAGE, &tc))
            tc->AddThingToVillage(*thing.GetPThing());
    }
}
