// CThingFilter_IsValidGossipee::operator()(const CThingAICreature& thing) const -- a gossip partner other than the gossiper itself, of the required faction (or factionless when none is required) and village (or village-less), and, outside the village's sociable hours, sharing the gossiper's home building and standing inside it (or both homeless)
// Grade: RELOCATION_MATCH -- w24A: retail keeps TWO `return false` blocks and pairs them crosswise: the faction mismatch (falls in at 0x0090C946) shares with the village-less arm's far backward jump (0x0090C9D0), and the factionless `HasTC(TCI_ENEMY)` test (jne at 0x0090C952) shares the village mismatch's block 0x0090C994. Four separate `return false;` statements always send the factionless test to the first copy (Q16's ~30 restructurings); the pairing is two exits to one statement, written here as a `goto` into the village mismatch's return (a structural stand-in for whatever shared exit the original had -- macro or early-out; no evidence which).
// Leaf: ??RCThingFilter_IsValidGossipee@@QBE_NABVCThingAICreature@@@Z
// Retail: Fable.exe 0x0090C900, 345 bytes, thiscall `ret 4` (Ego_r.pdb name above).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (the expanded PeekTC<CTCEnemy> / <CTCVillageMember>),
//   CTCVillageMember::GetPVillage 0x007EE540, CTCVillage::IsSociableTime 0x00658D5D (Ego_r.pdb ?IsSociableTime@CTCVillage@@QBE_NXZ, landed; Q15 called it IsCurfewTime, which is 0x00658DB3), CBaseIntelligentPointer::GetPItem 0x00A01B50
//   (inside the inline GetHomeBuilding), CThing::GetPos const 0x004C73D0, CThingBuilding::IsPointInside 0x0082D000.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_CURFEW
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CThingFilter_IsValidGossipee.h"
// FableWin.pdb ?GetHomeBuilding@CThingAICreature@@QBEPAVCThingBuilding@@XZ, inline in retail (declared in engine/CThingAICreature.h).
inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }

bool CThingFilter_IsValidGossipee::operator()(const CThingAICreature& thing) const
{
    const CTCVillageMember* member;
    if (PFaction) {
        const CTCEnemy* enemy;
        if (!thing.PeekTC(&enemy) || PFaction != enemy->GetPFaction())
            return false;
    } else if (thing.HasTC(TCI_ENEMY))
        goto fail;
    if (PTCVillage) {
        if (!thing.PeekTC(&member) || PTCVillage != member->GetPVillage()) {
fail:
            return false;
        }
    } else if (thing.PeekTC(&member) && member->GetPVillage())
        return false;
    if (PTCVillage && !PTCVillage->IsSociableTime()) {
        CThingBuilding* home = thing.GetHomeBuilding();
        if (home)
            return &thing != Thing && Thing->GetHomeBuilding() == home && home->IsPointInside(thing.GetPos());
        return &thing != Thing && !Thing->GetHomeBuilding();
    }
    return &thing != Thing;
}