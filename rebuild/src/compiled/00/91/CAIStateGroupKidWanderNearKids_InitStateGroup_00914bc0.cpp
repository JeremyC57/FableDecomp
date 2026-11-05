// CAIStateGroup_KidWanderNearKids::InitStateGroup(const CWorld& world, CThingAICreature& creature) -- register this state group as idle group 11 with the creature's CTCIdleScheduler, when it has one
// Leaf: ?InitStateGroup@CAIStateGroup_KidWanderNearKids@@UAEXABVCWorld@@AAVCThingAICreature@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x00914bc0, 75 bytes, thiscall, `ret 8`.
// Identity (proven): vtable 0x01289798 (RTTI .?AVCAIStateGroup_KidWanderNearKids@@) slot 13 (the InitStateGroup slot: CAIStateGroup_VillageTaskPickCrops' slot 13 is its Ego_r.pdb InitStateGroup); Ego_r.pdb ?InitStateGroup@CAIStateGroup_KidWanderNearKids@@UAEXABVCWorld@@AAVCThingAICreature@@@Z; idle group 11; CTCIdleScheduler::GetInterfaceType() is 211 (0xD3, landed).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020 (the inlined GetInterface: presence bit 19 of bitset
//   word 6), CTCIdleScheduler::RegisterIdleStateGroup(CAIStateGroupBase*, EIdleStateGroup) 0x00801920 (Ego_r.pdb).
// Header debt: engine/CAIStateGroup_KidWanderNearKids.h / CAIStateGroupBase.h / CThing are flat layouts; modelled locally.
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature : public CThing { };
class CWorld;
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
};
enum EIdleStateGroup { EIdleStateGroup_FIRST };
class CTCIdleScheduler {
public:
    void RegisterIdleStateGroup(CAIStateGroupBase* group, EIdleStateGroup type);   // 0x00801920
};
class CAIStateGroup_KidWanderNearKids : public CAIStateGroupBase {
public:
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};
void CAIStateGroup_KidWanderNearKids::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    CTCIdleScheduler* scheduler;
    if (creature.GetTC(TCI_IDLE_SCHEDULER, &scheduler))
        scheduler->RegisterIdleStateGroup(this, (EIdleStateGroup)11);
}