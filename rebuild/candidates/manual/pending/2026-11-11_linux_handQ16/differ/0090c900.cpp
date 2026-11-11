// CThingFilter_IsValidGossipee::operator()(const CThingAICreature& thing) const -- a gossip partner other than the gossiper itself, of the required faction (or factionless when none is required) and village (or village-less), and, during the village's curfew, sharing the gossiper's home building and standing inside it (or both homeless)
// Grade: DIFFER(345v345) -- one branch target left after the port: the factionless `else if (thing.HasTC(TCI_ENEMY)) return false;` (jne at 0x0090C952) reaches the first return-false copy 0x0090C946, retail the second copy 0x0090C994. Q16 probed ~30 restructurings (braced else, split faction/village ifs, ?: faction test, empty-then/negated HasTC, per-arm member locals, de Morgan village test, separate !PFaction test, HasTC forceinline/bool-ref spellings): each leaves this target or moves the village targets with it.
// Leaf: ??RCThingFilter_IsValidGossipee@@QBE_NABVCThingAICreature@@@Z
// Retail: Fable.exe 0x0090C900, 345 bytes, thiscall `ret 4` (Ego_r.pdb name above).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (the expanded PeekTC<CTCEnemy> / <CTCVillageMember>),
//   CTCVillageMember::GetPVillage 0x007EE540, CTCVillage::IsCurfewTime 0x00658D5D, CBaseIntelligentPointer::GetPItem 0x00A01B50
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
    if (PFaction) {
        const CTCEnemy* enemy;
        if (!thing.PeekTC(&enemy) || PFaction != enemy->GetPFaction())
            return false;
    } else if (thing.HasTC(TCI_ENEMY))
        return false;
    const CTCVillageMember* member;
    if (PTCVillage) {
        if (!thing.PeekTC(&member) || PTCVillage != member->GetPVillage())
            return false;
    } else if (thing.PeekTC(&member) && member->GetPVillage())
        return false;
    if (PTCVillage && !PTCVillage->IsCurfewTime()) {
        CThingBuilding* home = thing.GetHomeBuilding();
        if (home)
            return &thing != Thing && Thing->GetHomeBuilding() == home && home->IsPointInside(thing.GetPos());
        return &thing != Thing && !Thing->GetHomeBuilding();
    }
    return &thing != Thing;
}
