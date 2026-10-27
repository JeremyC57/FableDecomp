// CAIStateGroup_VillageTaskSecuritySweep::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, EScriptAIPriority priority, const CCharString& name) -- the base initialisation, then cache the creature's CTCVillageMember component
// Leaf: ?Initialise@CAIStateGroup_VillageTaskSecuritySweep@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093fc70, 96 bytes, thiscall, `ret 14h`.
// Identity (proven): vtable 0x0128BAA0 (RTTI .?AVCAIStateGroup_VillageTaskSecuritySweep@@) slot 1 (Initialise); Ego_r.pdb ?Initialise@CAIStateGroup_VillageTaskSecuritySweep@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z; it fills Ego_r.pdb PTCVillageMember (+0x14).
// Callees: CAIStateGroupBase::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&) 0x008FCEC0
//   (Ego_r.pdb, qualified base call), CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020 (the inlined GetInterface
//   on PBrain->GetCreature(), its out pointer the member itself, the key homed in the dead name-argument slot).
// Header debt: engine/CAIStateGroup_VillageTaskSecuritySweep.h / CAIStateGroupBase.h are flat layouts; modelled locally.
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
class CAIStateGroup_VillageTaskSecuritySweep : public CAIStateGroupBase {
public:
    CTCVillageMember* PTCVillageMember;                    // +0x14
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, EScriptAIPriority priority, const CCharString& name);
};
void CAIStateGroup_VillageTaskSecuritySweep::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, EScriptAIPriority priority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, id, priority, name);
    PBrain->GetCreature().GetInterface(&PTCVillageMember, (ETCInterfaceType)0x23);
}