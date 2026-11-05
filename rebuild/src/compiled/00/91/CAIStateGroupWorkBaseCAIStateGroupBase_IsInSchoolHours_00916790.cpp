// CAIStateGroup_WorkBase<CAIStateGroupBase>::IsInSchoolHours() const -- the creature's CTCVillageMember (interface 0x23) has a village and the village's IsSchoolTime holds
// Leaf: ?IsInSchoolHours@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x00916790, 93 bytes, thiscall, `ret`.
// Identity (proven): Ego_r.pdb ?IsInSchoolHours@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ (bound in reloc_bindings under the stand-in owner CAIStateGroup_StayNearSchool); callers CAIStateGroup_StayNearSchool::IsRunnable 0x009167F0 and Update 0x00916910 (RTTI base CAIStateGroup_WorkBase<CAIStateGroupBase>); it tests the village's school time.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020 (the inlined GetInterface on PBrain->Creature, CAIBrain +0x18),
//   CTCVillageMember::GetPVillage() const 0x007EE540 (twice), CTCVillage::IsSchoolTime() const 0x00658A35.
// CTCVillage's three time tests compare the clock with Ego_r.pdb CVillageDef SchoolStartTimes/SchoolDuration (0x00658A35), WorkStartTimes/WorkDuration (0x00658B20) and EatStartTimes/EatDuration (0x00658BB9, retail def +0x84/+0x90); IsSchoolTime / IsWorkTime are Ego_r.pdb names, IsEatTime is descriptive.
// Header debt: engine/CAIStateGroup_WorkBase.h / CAIStateGroupBase.h / CAIBrain.h are flat layouts; modelled locally.
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature : public CThing { };
class CTCVillage {
public:
    bool IsSchoolTime() const;                                    // 0x00658A35
};
class CTCVillageMember {
public:
    CTCVillage* GetPVillage() const;                       // 0x007EE540
};
class CAIBrain {
public:
    unsigned char     _pad00[0x18];
    CThingAICreature* Creature;                            // +0x18
};
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                      // +0x04
};
template <class TBase> class CAIStateGroup_WorkBase : public TBase {
protected:
    bool IsInSchoolHours() const;
};
template <class TBase> bool CAIStateGroup_WorkBase<TBase>::IsInSchoolHours() const
{
    CThingAICreature* creature = this->PBrain->Creature;
    CTCVillageMember* member;
    if (creature->GetTC(TCI_VILLAGE_MEMBER, &member) && member->GetPVillage() && member->GetPVillage()->IsSchoolTime())
        return true;
    return false;
}
template class CAIStateGroup_WorkBase<CAIStateGroupBase>;