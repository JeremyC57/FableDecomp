#include <cstdio>
#ifndef _MSC_VER
// Host tests use pointer-sized members; VC7.1 uses the shared retail layouts.
#define FABLE_ENGINE_CAISTATEGROUPBASE_H
#define FABLE_ENGINE_CAIBRAIN_H
#define FABLE_ENGINE_CTHINGAICREATURE_H
class CThingAICreature { public:};
class CAIBrain { public: CThingAICreature* Creature; };
class CAIStateGroupBase { public: void* __vftable; CAIBrain* PBrain; };
#endif
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

static int getCalls, sets, constructed, destroyed, seats, placements, replacements;
static int errors, sequence[8], sequenceCount;
static bool seatResult, placementResult;
static CGameMasterMethods* expectedState;
static CCreatureWorkView* replacementCreature;
static C3DVector expectedPosition = {2.0f,-3.0f,4.0f};
float GameMasterPlacementRadius = 7.5f;
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerThing::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { ++constructed; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++destroyed; }
CThing* CBaseIntelligentPointer::GetPItem() const { ++getCalls; return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* p) { ++sets; m_pItem=p; }
void CSeatVirtualView::v00() {} void CSeatVirtualView::v01() {}
void CSeatVirtualView::v02() {} void CSeatVirtualView::v03() {}
void CSeatVirtualView::v04() {} void CSeatVirtualView::v05() {}
void CSeatVirtualView::v06() {} void CSeatVirtualView::v07() {}
void CSeatVirtualView::v08() {} void CSeatVirtualView::v09() {}
void CSeatVirtualView::v11() {} void CSeatVirtualView::v12() {}
void CSeatVirtualView::v13() {}
bool CSeatVirtualView::GetSeat() {
    ++seats; sequence[sequenceCount++]=1;
    if ((void*)this!=(void*)expectedState || destroyed!=1) ++errors;
    expectedState->SeatPosition=expectedPosition;
    expectedState->PBrain->Creature=(CThingAICreature*)replacementCreature;
    return seatResult;
}
void CSeatVirtualView::OnReplace() {
    ++replacements; sequence[sequenceCount++]=3;
    if ((void*)this!=(void*)expectedState || placements!=1) ++errors;
}
bool CCreaturePlacementMethods::AutoPlaceNear(const C3DVector& p,float radius) {
    ++placements; sequence[sequenceCount++]=2;
    if ((void*)this!=(void*)replacementCreature || &p!=&expectedState->SeatPosition ||
        p.X!=2.0f || p.Y!=-3.0f || p.Z!=4.0f || radius!=7.5f || seats!=1) ++errors;
    return placementResult;
}
static int run(bool hasBuilding,bool dead,bool seat,bool placed) {
    CCreatureWorkView firstCreature, secondCreature;
    CGameMasterMethods state;
    CSeatVirtualView vtableProvider;
    CAIBrain brain;
    CThing building = {};
    building.Dead=dead;
    firstCreature.WorkBuilding.m_pItem=hasBuilding?&building:0;
    brain.Creature=(CThingAICreature*)&firstCreature;
    state.PBrain=&brain;
    state.__vftable=*(void**)&vtableProvider;
    state.SeatPosition.X=99; state.SeatPosition.Y=98; state.SeatPosition.Z=97;
    expectedState=&state; replacementCreature=&secondCreature;
    seatResult=seat; placementResult=placed;
    getCalls=sets=constructed=destroyed=seats=placements=replacements=errors=sequenceCount=0;
    bool result=state.InitCreaturePosition();
    bool attempted=hasBuilding && !dead && seat;
    if (result!=(attempted && placed) || constructed!=1 || destroyed!=1 || sets!=2 ||
        state.Building.m_pItem!=(hasBuilding?&building:0) ||
        seats!=(hasBuilding && !dead ? 1:0) || placements!=(attempted?1:0) ||
        replacements!=(attempted?1:0) || getCalls!=(hasBuilding?4:3)) ++errors;
    if (attempted && (sequenceCount!=3 || sequence[0]!=1 || sequence[1]!=2 || sequence[2]!=3)) ++errors;
    return errors;
}
int main() {
    if (run(false,false,true,true) || run(true,true,true,true) ||
        run(true,false,false,true) || run(true,false,true,false) || run(true,false,true,true)) {
        std::printf("0094c850_TEST FAIL\n"); return 1;
    }
    std::printf("0094c850_TEST PASS\n"); return 0;
}