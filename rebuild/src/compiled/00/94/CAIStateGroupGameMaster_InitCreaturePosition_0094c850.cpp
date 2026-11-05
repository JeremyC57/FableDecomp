// CAIStateGroup_GameMaster::InitCreaturePosition() -- acquire work building and seat, place the creature, then run replacement cleanup.
// Leaf: ?InitCreaturePosition@CAIStateGroup_GameMaster@@QAE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Grade: RELOCATION_MATCH -- explicit temporary lifetime and placement receiver reproduce retail ordering.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/C3DVector.h"
class CThing { public: unsigned char _beforeDead[0x91]; unsigned char Dead:1; };
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item=0) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};


struct CCreatureWorkView {
 unsigned char _beforeWork[0x170]; CBaseIntelligentPointer WorkBuilding;
 CBaseIntelligentPointer& GetWorkBuilding() { return WorkBuilding; }
}; // donor CThingAICreature::WorkBuilding is +0x198; retail is +0x170.
struct CCreaturePlacementMethods { bool AutoPlaceNear(const C3DVector&,float); }; // 00665DF0
struct CSeatVirtualView {
 virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
 virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
 virtual void v08(); virtual void v09(); virtual void OnReplace(); virtual void v11();
 virtual void v12(); virtual void v13(); virtual bool GetSeat();
};
extern float GameMasterPlacementRadius; // 0138CEC8
struct CAIStateGroup_GameMaster : CAIStateGroupBase {
 CIntelligentPointerThing Building;
 unsigned char _beforeSeat[0x40-0x1c]; C3DVector SeatPosition;
 bool InitCreaturePosition();
};
bool CAIStateGroup_GameMaster::InitCreaturePosition() {
 CCreatureWorkView* creature=(CCreatureWorkView*)PBrain->Creature;
 {
 CIntelligentPointerThing work(((CThing*)creature->GetWorkBuilding().GetPItem()));
 CThing* item=((CThing*)work.GetPItem());
 Building.SetPItem((CBaseObject*)item);
 }
 if(((CThing*)Building.GetPItem()) && !((CThing*)Building.GetPItem())->Dead && ((CSeatVirtualView*)this)->GetSeat()) {
  CAIBrain* brain = PBrain;
  CCreaturePlacementMethods* placement=(CCreaturePlacementMethods*)brain->Creature;
  bool placed=placement->AutoPlaceNear(SeatPosition,GameMasterPlacementRadius);
  ((CSeatVirtualView*)this)->OnReplace();
  return placed;
 }
 return false;
}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitCreaturePosition@CAIStateGroup_GameMaster@@MAE_NXZ); evidence: RTTI vtable slot + PDB name set.
