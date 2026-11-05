// CAIStateGroup_VillageTaskPatrol::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, EScriptAIPriority priority, const CCharString& name) -- the base initialisation, then cache the creature's CTCVillageMember component
// Leaf: ?Initialise@CAIStateGroup_VillageTaskPatrol@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093eed0, 96 bytes, thiscall, `ret 14h`.
// Identity (proven): vtable 0x0128BA64 (RTTI .?AVCAIStateGroup_VillageTaskPatrol@@) slot 1 (Initialise); Ego_r.pdb ?Initialise@CAIStateGroup_VillageTaskPatrol@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z; it fills Ego_r.pdb PTCVillageMember (+0x14).
// Callees: CAIStateGroupBase::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&) 0x008FCEC0
//   (Ego_r.pdb, qualified base call), CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020 (the inlined GetInterface
//   on PBrain->GetCreature(), its out pointer the member itself, the key homed in the dead name-argument slot).
// Header debt: engine/CAIStateGroup_VillageTaskPatrol.h / CAIStateGroupBase.h are flat layouts; modelled locally.
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature : public CThing { };
class CCharString;
class CTCVillageMember;
enum EScriptAIPriority { EScriptAIPriority_FIRST };
class CAIBrain {
public:
    unsigned char     _pad00[0x18];
    CThingAICreature* Creature;                            // +0x18
    CThingAICreature& GetCreature() const { return *Creature; }   // Ego_r.pdb ?GetCreature@CAIBrain@@QBEAAVCThingAICreature@@XZ (inline)
};
class CAIStateGroupBase {
public:
    virtual void v000();
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, EScriptAIPriority priority, const CCharString& name);   // 0x008FCEC0
    CAIBrain*     PBrain;                                  // +0x04
    unsigned char _pad08[0x0C];
};
class CAIStateGroup_VillageTaskPatrol : public CAIStateGroupBase {
public:
    CTCVillageMember* PTCVillageMember;                    // +0x14
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, EScriptAIPriority priority, const CCharString& name);
};
void CAIStateGroup_VillageTaskPatrol::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, EScriptAIPriority priority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, id, priority, name);
    PBrain->GetCreature().GetTC(TCI_VILLAGE_MEMBER, &PTCVillageMember);
}