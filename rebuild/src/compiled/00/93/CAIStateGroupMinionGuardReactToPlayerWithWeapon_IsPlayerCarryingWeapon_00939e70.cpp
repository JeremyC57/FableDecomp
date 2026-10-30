// CAIStateGroup_MinionGuardReactToPlayerWithWeapon::IsPlayerCarryingWeapon() const -- true when the creature's opinion of the hero (interface 0x4F) holds a still-happening deed of type 1
// Leaf: ?IsPlayerCarryingWeapon@CAIStateGroup_MinionGuardReactToPlayerWithWeapon@@ABE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00939E70, 90 bytes (+ int3 padding), thiscall, `ret`.
// Identity (proven): Ego_r.pdb donor order CheckForOtherGuardsNewWarning (0x0095D100) / IsPlayerCarryingWeapon (0x0095D160) /
//   IsRunnable (0x0095D1D0) = the landed retail 0x00939D70 / this row / the landed 0x00939ED0 (which calls this row).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound const 0x004365B0 (the inlined component lookup),
//   CTCCreatureOpinionOfHero::FindMostRecentDeed<COpinionDeedFilter_IsStillHappening>(EOpinionDeedType, const
//   COpinionDeedFilter_IsStillHappening&, long*, bool) const 0x0093A3C0 (Ego_r.pdb template; landed).
// Shape: HasTC(0x4F) (bit 15 of the +0x28 word: `test ch,ch ; jns`), then the component picked through the end/hit ternary
//   (component straight into ecx; the out-pointer GetInterface form leaves it in eax, GOTCHAS chunk 254); the empty filter
//   temporary sits in the high byte of the dead key slot (`lea edx,[esp+0xf]`). PeekTC is a descriptive inline helper.
// ---- shared AI state-group model (retail STLport layout; the generated engine headers are flat Ego_r layouts) ----
// CAIStateGroupBase (RTTI: : CBaseClass): vptr +0, PBrain +4, ScriptAIPriority +8, Name +0xC, Terminated +0x10, Priority
// +0x11, Enabled +0x12 (0x14 bytes). CAIBrain::Creature +0x18 (engine/CAIBrain.h agrees). CThing (retail): UsingComponents
// bitset<274> +0x20, Components CVectorMap +0x44, Dead bit 0 of +0x91 (Ego_r +0xA1 minus 0x10: bitset, CVectorMap, list).
// Getter identities (Ego_r.pdb donor order = retail order 0x008FCF40..0x008FD010): Terminate 0x008FCF40, PeekWorld
// 0x008FCFA0, GetWorld 0x008FCFC0, GetPWorldSeed 0x008FCFD0, PeekThingSearchTools 0x008FD000, IsStopping 0x008FD010.
#include "engine/CAIBrain.h"                           // Creature +0x18
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObject : public CBaseClass { };
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
private:
    void* PData;                                        // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
};
template <class T> class CCountedPointer {
public:
    T* Data;                                            // +0x00
    T* operator->() const { return Data; }
};
class C3DVector {
public:
    float X, Y, Z;
};
enum ETCInterfaceType { ETCInterfaceType_FIRST };
class CTCBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes (the hand955 L083/L084 model)
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                  // +0x00 (CArray: STLport vector)
    value_type* Last;                                   // +0x04
    value_type* EndOfStorage;                           // +0x08
    TCompare    Compare;                                // +0x0C
    bool        Dirty;                                  // +0x0D
    const value_type* end() const { return Last; }
    const value_type* LowerBound(const K& key) const;   // 0x004365B0 (the const copy)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CClass : public CBaseObject { };
class CThing : public CClass {
public:
    unsigned char _pad04[0x14];
    unsigned __int64 UID;                               // +0x18
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    CInterfaceMap Components;                           // +0x44
    unsigned char _pad54[0x91 - 0x54];
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPos() const;                    // 0x004C73D0
    unsigned long HasTC(ETCInterfaceType type) const   // the masked word (a bool return narrows to `shr ; test`)
    {
        return UsingComponents[type >> 5] & (1UL << (type & 31));
    }
    template <class T> __forceinline const T* PeekTC(ETCInterfaceType type) const
    {
        ETCInterfaceType key = type;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        return (const T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
    template <class T> __forceinline bool GetInterface(const T** out, ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = type;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (const T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject {
public:
    bool IsPerformingAnyAction() const;                 // 0x006622A0
};
class CThingAICreature : public CThingCreatureBase { };
class CThingObject : public CThingGameObject { };
class CThingBuilding;

#include "engine/CWorld.h"                            // PThingCombatManager_ptr +0x4C, PMessageEventManager_Data +0x60
class CThingSearchTools;
class CWorld;
class CAIStateGroupBase : public CBaseClass {
public:
    CAIBrain*     PBrain;                               // +0x04
    long          ScriptAIPriority;                     // +0x08
    CCharString   Name;                                 // +0x0C
    bool          Terminated;                           // +0x10
    unsigned char Priority;                             // +0x11
    bool          Enabled;                              // +0x12
    void Terminate();                                   // 0x008FCF40
protected:
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
protected:
    CWorld& GetWorld() const;    // 0x008FCFC0
public:
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    bool IsStopping() const;                            // 0x008FD010
};
class COpinionDeed;
class COpinionDeedFilter_IsStillHappening {             // empty predicate
public:
    COpinionDeedFilter_IsStillHappening() { }
};
class CTCCreatureOpinionOfHero {                        // interface 0x4F
public:
    template <class TFilter>
    const COpinionDeed* FindMostRecentDeed(long deedType, const TFilter& filter, long* outHandle, bool includePending) const;
};
class CAIStateGroup_MinionGuardReactToPlayerWithWeapon : public CAIStateGroupBase {
private:
    bool IsPlayerCarryingWeapon() const;
};

bool CAIStateGroup_MinionGuardReactToPlayerWithWeapon::IsPlayerCarryingWeapon() const
{
    const CThingAICreature* creature = PBrain->Creature;
    if (creature->HasTC((ETCInterfaceType)0x4F)) {
        const CTCCreatureOpinionOfHero* opinion = creature->PeekTC<CTCCreatureOpinionOfHero>((ETCInterfaceType)0x4F);
        if (opinion->FindMostRecentDeed(1, COpinionDeedFilter_IsStillHappening(), 0, false))
            return true;
    }
    return false;
}