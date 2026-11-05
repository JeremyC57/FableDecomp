// CTCDummyVillager::CTCDummyVillager(CThing*) -- CTCBase(thing), no def type, the home/work building and creature intelligent pointers (UIDs zero), never-recreate day/frame, respawnable, not a guard
// Leaf: ??0CTCDummyVillager
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
//
// Retail 0x0072F0F0, 118 bytes (`ret 4`). Callees: CTCBase::CTCBase(CThing*) @ 0x00686800,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (three times; then the
// CIntelligentPointer<CThingBuilding> vtable 0x01244344 twice and CIntelligentPointer<CThingAICreature>
// 0x0123E7E0). The vtable store 0x01266CDC is CTCDummyVillager (rtti-vtables.tsv): the catalog label
// "CTCDPhysicalObstruction::CTCDPhysicalObstruction" is propagated.
// Layout = Ego_r.pdb CTCDummyVillager (0x50 bytes): DefType +0xc, HomeBuilding +0x10, HomeBuildingUID
// +0x18, WorkBuilding +0x20, WorkBuildingUID +0x28, Creature +0x30, CreatureUID +0x38,
// DayToBeRecreated +0x40, FrameToBeRecreated +0x44, Respawnable +0x48, Guard +0x49.
// Shape: plain initialisers in declaration order; one zero register (ebx), each pointer member
// addressed once in edi for its base ctor + vtable store.
class CThing;
class CThingBuilding;
class CThingAICreature;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* thing);                                 // 0x00686800
};

class CTCDummyVillager : public CTCBase { public:                         // vtable 0x01266CDC
    unsigned long                         DefType;          // +0x0c
    CIntelligentPointer<CThingBuilding>   HomeBuilding;     // +0x10
    unsigned __int64                      HomeBuildingUID;  // +0x18
    CIntelligentPointer<CThingBuilding>   WorkBuilding;     // +0x20
    unsigned __int64                      WorkBuildingUID;  // +0x28
    CIntelligentPointer<CThingAICreature> Creature;         // +0x30
    unsigned __int64                      CreatureUID;      // +0x38
    long                                  DayToBeRecreated; // +0x40
    long                                  FrameToBeRecreated; // +0x44
    bool                                  Respawnable;      // +0x48
    bool                                  Guard;            // +0x49
    virtual void v00();
    CTCDummyVillager(CThing* thing);
};

CTCDummyVillager::CTCDummyVillager(CThing* thing)
    : CTCBase(thing)
    , DefType(0)
    , HomeBuilding()
    , HomeBuildingUID(0)
    , WorkBuilding()
    , WorkBuildingUID(0)
    , Creature()
    , CreatureUID(0)
    , DayToBeRecreated(0x7fffffff)
    , FrameToBeRecreated(0x7fffffff)
    , Respawnable(true)
    , Guard(false)
{
}