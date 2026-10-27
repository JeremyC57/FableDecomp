// CAIStateGroup_WorkBase<CAIStateGroupBase>::IsInSchoolHours() const -- the creature's CTCVillageMember (interface 0x23) has a village and the village's IsSchoolTime holds
// Leaf: ?IsInSchoolHours@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00916790, 93 bytes, thiscall, `ret`.
// Identity (proven): Ego_r.pdb ?IsInSchoolHours@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ (bound in reloc_bindings under the stand-in owner CAIStateGroup_StayNearSchool); callers CAIStateGroup_StayNearSchool::IsRunnable 0x009167F0 and Update 0x00916910 (RTTI base CAIStateGroup_WorkBase<CAIStateGroupBase>); it tests the village's school time.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020 (the inlined GetInterface on PBrain->Creature, CAIBrain +0x18),
//   CTCVillageMember::GetPVillage() const 0x007EE540 (twice), CTCVillage::IsSchoolTime() const 0x00658A35.
// CTCVillage's three time tests compare the clock with Ego_r.pdb CVillageDef SchoolStartTimes/SchoolDuration (0x00658A35), WorkStartTimes/WorkDuration (0x00658B20) and EatStartTimes/EatDuration (0x00658BB9, retail def +0x84/+0x90); IsSchoolTime / IsWorkTime are Ego_r.pdb names, IsEatTime is descriptive.
// Header debt: engine/CAIStateGroup_WorkBase.h / CAIStateGroupBase.h / CAIBrain.h are flat layouts; modelled locally.
enum ETCInterfaceType { ETCInterfaceType_FIRST };
class CTCBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                     // +0x00
    value_type* Last;                                      // +0x04
    value_type* EndOfStorage;                              // +0x08
    TCompare    Compare;                                   // +0x0C
    bool        Dirty;                                     // +0x0D
    value_type* end() { return Last; }
    value_type* LowerBound(const K& key);                  // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x1C];
    unsigned long PresentInterfaces[9];                    // +0x20
    CInterfaceMap Interfaces;                              // +0x44
    template <class T> __forceinline bool GetInterface(T** out, ETCInterfaceType type)
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = type;
            CInterfaceMap* map = &Interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
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
    if (creature->GetInterface(&member, (ETCInterfaceType)0x23) && member->GetPVillage() && member->GetPVillage()->IsSchoolTime())
        return true;
    return false;
}
template class CAIStateGroup_WorkBase<CAIStateGroupBase>;