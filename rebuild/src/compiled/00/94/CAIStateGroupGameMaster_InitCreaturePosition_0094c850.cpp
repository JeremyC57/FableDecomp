// CAIStateGroup_GameMaster::InitCreaturePosition() -- acquire work building and seat, place the creature, then run replacement cleanup.
// Grade: RELOCATION_MATCH -- explicit temporary lifetime and placement receiver reproduce retail ordering.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
class C3DVector { public: float X,Y,Z; };
class CThing { public: unsigned char _beforeDead[0x91]; unsigned char Dead:1; };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item=0) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
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
struct CGameMasterMethods : CAIStateGroupBase {
 CIntelligentPointerThing Building;
 unsigned char _beforeSeat[0x40-0x1c]; C3DVector SeatPosition;
 bool InitCreaturePosition();
};
bool CGameMasterMethods::InitCreaturePosition() {
 CCreatureWorkView* creature=(CCreatureWorkView*)PBrain->Creature;
 {
 CIntelligentPointerThing work(creature->GetWorkBuilding().GetPItem());
 CThing* item=work.GetPItem();
 Building.SetPItem(item);
 }
 if(Building.GetPItem() && !Building.GetPItem()->Dead && ((CSeatVirtualView*)this)->GetSeat()) {
  CAIBrain* brain = PBrain;
  CCreaturePlacementMethods* placement=(CCreaturePlacementMethods*)brain->Creature;
  bool placed=placement->AutoPlaceNear(SeatPosition,GameMasterPlacementRadius);
  ((CSeatVirtualView*)this)->OnReplace();
  return placed;
 }
 return false;
}