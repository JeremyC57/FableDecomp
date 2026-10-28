// CAIStateGroup_NymphFloatAround::IsRunnable() -- gives the creature a "CTCNymph" component (interface 0xF5) when it has none, then is runnable while it has one
// Grade: DIFFER(81v81) -- register choice only: retail threads every PBrain->Creature load through eax (and loads the AddTC this as `mov edx,[esi+4]` before the pushes, `mov ecx,[edx+18h]` after); the lane uses ecx/edx chains (GetCreature accessor, creature local, named-string, bool-local and __forceinline HasTC forms probed)
// Leaf: ?IsRunnable@CAIStateGroup_NymphFloatAround@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091C0D0, 81 bytes (+ int3 padding), thiscall virtual (slot 11), `ret`.
// Identity (proven): Ego_r.pdb ?IsRunnable@CAIStateGroup_NymphFloatAround@@UAE_NXZ (0x0093F630), vtable slot 11; the label agrees.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CThing::AddTC(const CCharString&, bool, const
//   CTCParamsBase*) 0x004C9D60 (Ego_r.pdb), CCharString::~CCharString() 0x0099EAE0. Data: "CTCNymph" @ 0x0123DEA0.
// Shape: CThing::HasTC inline (Ego_r.pdb `bool HasTC(ETCInterfaceType) const`, written `if (bit) return true; return false;`
//   so the test folds to `test dword ptr [eax+3Ch],200000h` and the return to `shr ; and al,1`); the name is the implicit
//   const char* -> CCharString conversion (built before the other arguments are pushed).
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
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTCParamsBase;
class CBaseClass {
public:
    virtual ~CBaseClass();
};
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
    value_type* end() { return Last; }
    const value_type* end() const { return Last; }
    value_type* LowerBound(const K& key);               // 0x0040F020
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
    bool HasTC(ETCInterfaceType type) const             // Ego_r.pdb ?HasTC@CThing@@QBE_NW4ETCInterfaceType@@@Z (inline here)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
    template <class T> __forceinline bool GetInterface(T** out, ETCInterfaceType type)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetTC(ETCInterfaceType type)
    {
        ETCInterfaceType key = type;
        CInterfaceMap* map = &Components;
        CInterfaceMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
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
    const CWorld& PeekWorld() const;                    // 0x008FCFA0
    CWorld& GetWorld() const;                           // 0x008FCFC0
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    bool IsStopping() const;                            // 0x008FD010
};
class CAIStateGroup_NymphFloatAround : public CAIStateGroupBase {
public:
    virtual bool IsRunnable();
};

bool CAIStateGroup_NymphFloatAround::IsRunnable()
{
    if (!PBrain->Creature->HasTC((ETCInterfaceType)0xF5))
        PBrain->Creature->AddTC("CTCNymph", false, 0);
    return PBrain->Creature->HasTC((ETCInterfaceType)0xF5);
}
