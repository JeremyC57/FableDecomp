// CAIStateGroup_WanderAroundShops::InitCreaturePosition() -- during work hours and with the script-def chance (percent), pick a random shop; when one is found, place the creature next to it (beside the building for a building, else near its position, radius 5)
// Leaf: ?InitCreaturePosition@CAIStateGroup_WanderAroundShops
// NF131 (2026-11-20, q4946): the row-local CAIStateGroupBase model gives way to the retail engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE,
//   _MEMBERS) and engine/CAIStateGroup_WorkBase.h; the class is re-based on CAIStateGroup_WorkBase<CAIStateGroupBase> (RTTI ghidra_out/rtti_bases.tsv),
//   so the call to 0x0091A6F0 binds ?IsInWorkHours@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ (Ego_r.pdb 0x0093E1A0 + FableWin.pdb
//   0x02BA9910: protected const member of the template), was a stand-in spelling.
//   The local base's Random(range) helper (the seed step on GetPWorldSeed()) is the shared inline GFRandom (engine/GFRandom.h, Ego_r.pdb
//   ?GFRandom@@YIKKPAK@Z, as the landed 0x00938280) on the retail header's protected GetPWorldSeed() 0x008FCFD0.
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009473E0, length 266 bytes (plain `ret`, virtual, bool).
// Callees: CAIStateGroup_WorkBase::IsInWorkHours @ 0x0091A6F0, GetPWorldSeed @ 0x008FCFD0 + GFROR13 @ 0x00497890 (the
//          landed CAIStateGroupBase::Random step), the shop picker @ 0x00946F90 (manifest
//          "CAIStateGroup_WanderAroundShops_SelectRandomEnemy"), CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem
//          0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CThing::GetPos 0x004C73D0,
//          CThingCreatureBase::AutoPlaceNearBuilding 0x00666EF0 / AutoPlaceNear 0x00665DF0; data: the chance 0x0138CE34.
// Layout: engine/CAIStateGroup_WanderAroundShops.h (Shop +0x14, PrettyThing +0x1c, NextFrameBuy +0x24) over the landed
//          CAIStateGroupBase model; the shop's +0x6c bit 6 marks a building.
// Shape: one `&&` chain whose last term assigns the picked shop through an IMPLICIT CThing* -> CIntelligentPointer
//          conversion temporary (`(Shop = SelectRandomShop()).GetPItem() != 0`): VC keeps the temporary's construction
//          flag in ebx (set after the converting ctor; an explicit functional cast sets it before the picker call) and the
//          condition in a frame byte; each placement arm reads the creature into a named local before its call.
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
class CThing;
#include "engine/C3DVector.h"
class CThingBuilding;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
class CThingPhysical : public CThing { public:
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"   // CBaseIntelligentPointer 0x00A01B10 / ~ 0x00A01C10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (Ego_r.pdb)
// CIntelligentPointerThing = the shared CIntelligentPointer<CThing> (engine/CIntelligentPointer.h).
class CThingCreatureBase { public:
    bool AutoPlaceNearBuilding(CThingBuilding* building, float radius);   // 0x00666EF0
    bool AutoPlaceNear(const C3DVector& position, float radius);          // 0x00665DF0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
#include "engine/GFRandom.h"                 // GFROR13 0x00497890, the inline GFRandom (Ego_r.pdb ?GFRandom@@YIKKPAK@Z)
extern unsigned long g_WanderAroundShopsChance;          // 0x0138CE34 (percent)

#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_WorkBase.h"
struct CAIStateGroup_WanderAroundShops : CAIStateGroup_WorkBase<CAIStateGroupBase> {
private:
    CThing* FindASimplyDelightfulShop();   // 0x00946F90 (Ego_r.pdb ?FindASimplyDelightfulShop@CAIStateGroup_WanderAroundShops@@AAEPAVCThing@@XZ)
public:
    CIntelligentPointer<CThing> Shop;              // +0x14
    CIntelligentPointer<CThing> PrettyThing;       // +0x1c
    long NextFrameBuy;                          // +0x24
    bool InitCreaturePosition();
};

bool CAIStateGroup_WanderAroundShops::InitCreaturePosition()
{
    bool ok = IsInWorkHours() && GFRandom(100, GetPWorldSeed()) < g_WanderAroundShopsChance
              && reinterpret_cast<CThingPhysical*>((Shop = ((CAIStateGroup_WanderAroundShops*)this)->FindASimplyDelightfulShop()).GetPItem()) != 0;
    if (ok) {
        if (reinterpret_cast<CThingPhysical*>(Shop.GetPItem())->GetTypeFlags() & 0x40) {
            CThingCreatureBase* creature = PBrain->PCreature;
            return creature->AutoPlaceNearBuilding((CThingBuilding*)reinterpret_cast<CThingPhysical*>(Shop.GetPItem()), 5.0f);
        }
        CThingCreatureBase* creature = PBrain->PCreature;
        return creature->AutoPlaceNear(reinterpret_cast<CThingPhysical*>(Shop.GetPItem())->GetPos(), 5.0f);
    }
    return false;
}