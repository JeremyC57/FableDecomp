// CAIStateGroup_MinionSitAtHome::InitStateGroup(const CWorld& world, CThingAICreature& creature) -- register this state group as idle group 3 with the creature's CTCIdleScheduler, when it has one
// Leaf: ?InitStateGroup@CAIStateGroup_MinionSitAtHome@@MAEXABVCWorld@@AAVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00939130, 75 bytes, thiscall, `ret 8`.
// Identity (proven): vtable 0x0128B6C4 (RTTI .?AVCAIStateGroup_MinionSitAtHome@@) slot 13 (the InitStateGroup slot: CAIStateGroup_VillageTaskPickCrops' slot 13 is its Ego_r.pdb InitStateGroup); Ego_r.pdb ?InitStateGroup@CAIStateGroup_MinionSitAtHome@@MAEXABVCWorld@@AAVCThingAICreature@@@Z; idle group 3; CTCIdleScheduler::GetInterfaceType() is 211 (0xD3, landed).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020 (the inlined GetInterface: presence bit 19 of bitset
//   word 6), CTCIdleScheduler::RegisterIdleStateGroup(CAIStateGroupBase*, EIdleStateGroup) 0x00801920 (Ego_r.pdb).
// Header debt: engine/CAIStateGroup_MinionSitAtHome.h / CAIStateGroupBase.h / CThing are flat layouts; modelled locally.
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
    value_type* First;                                     // +0x00 (CArray: STLport vector)
    value_type* Last;                                      // +0x04
    value_type* EndOfStorage;                              // +0x08
    TCompare    Compare;                                   // +0x0C
    bool        Dirty;                                     // +0x0D
    value_type* end() { return Last; }
    value_type* LowerBound(const K& key);                  // 0x0040F020 for the interface map
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x1C];
    unsigned long PresentInterfaces[9];                    // +0x20 (the bitset of present interface types)
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
class CAIStateGroup_MinionSitAtHome : public CAIStateGroupBase {
protected:
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};
void CAIStateGroup_MinionSitAtHome::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    CTCIdleScheduler* scheduler;
    if (creature.GetInterface(&scheduler, (ETCInterfaceType)0xD3))
        scheduler->RegisterIdleStateGroup(this, (EIdleStateGroup)3);
}