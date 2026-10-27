// CAIStateGroup_SitForMeal::IsMealTime() const -- the creature's CTCVillageMember (interface 0x23) has a village and the village's IsEatTime holds
// Leaf: ?IsMealTime@CAIStateGroup_SitForMeal@@ABE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009455b0, 93 bytes, thiscall, `ret`.
// Identity (proven): Ego_r.pdb ?IsMealTime@CAIStateGroup_SitForMeal@@ABE_NXZ; callers CAIStateGroup_SitForMeal::IsStateGroupStillValid 0x00945610 and IsRunnable 0x00945B90 (landed); the creature is read through the const interface lookup 0x004365B0.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x004365B0 (const) (the inlined GetInterface on PBrain->Creature, CAIBrain +0x18),
//   CTCVillageMember::GetPVillage() const 0x007EE540 (twice), CTCVillage::IsEatTime() const 0x00658BB9.
// CTCVillage's three time tests compare the clock with Ego_r.pdb CVillageDef SchoolStartTimes/SchoolDuration (0x00658A35), WorkStartTimes/WorkDuration (0x00658B20) and EatStartTimes/EatDuration (0x00658BB9, retail def +0x84/+0x90); IsSchoolTime / IsWorkTime are Ego_r.pdb names, IsEatTime is descriptive.
// Header debt: engine/CAIStateGroup_SitForMeal.h / CAIStateGroupBase.h / CAIBrain.h are flat layouts; modelled locally.
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
    const value_type* end() const { return Last; }
    const value_type* LowerBound(const K& key) const;      // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x1C];
    unsigned long PresentInterfaces[9];                    // +0x20
    CInterfaceMap Interfaces;                              // +0x44
    template <class T> __forceinline bool GetInterface(const T** out, ETCInterfaceType type) const
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = type;
            const CInterfaceMap* map = &Interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (const T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingAICreature : public CThing { };
class CTCVillage {
public:
    bool IsEatTime() const;                                    // 0x00658BB9
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
class CAIStateGroup_SitForMeal : public CAIStateGroupBase {
private:
    bool IsMealTime() const;
};
bool CAIStateGroup_SitForMeal::IsMealTime() const
{
    const CThingAICreature* creature = PBrain->Creature;
    const CTCVillageMember* member;
    if (creature->GetInterface(&member, (ETCInterfaceType)0x23) && member->GetPVillage() && member->GetPVillage()->IsEatTime())
        return true;
    return false;
}