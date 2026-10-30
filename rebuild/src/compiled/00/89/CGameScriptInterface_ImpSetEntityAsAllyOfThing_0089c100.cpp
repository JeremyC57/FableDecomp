// CGameScriptInterface::ImpSetEntityAsAllyOfThing(const CScriptThing& entity, const CScriptThing& other, bool isAlly) const -- add the other thing to (or remove it from) the ally list of the entity's kind-0x49 component when both script things are valid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089C100, length 131 bytes.
// Callees: the script things' slots 75 (+0x12c) and 11 (+0x2c), the component-map LowerBound @ 0x0040F020,
//          the ally add @ 0x0076D270 (manifest "CCreature_AddAllyToList") and the ally removal @ 0x0076C850
//          (manifest "CCreatureActionBase::AddDependentThing", propagated).
// Shape: the other thing is fetched before the flag test; the `push eax ; mov ecx,esi` is shared by both arms.
// Leaf: ImpSetEntityAsAllyOfThing
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry_Retail;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const ETCInterfaceType& kind);         // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound; the key by address)
    const STCEntry_Retail* LowerBound(const ETCInterfaceType& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind)          // the inlined map `find`: lower bound (the key in a stack temporary), end when the key is absent
    {
        const STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return ((CTCBase_Retail*)it->second);
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        const STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return ((CTCBase_Retail*)it->second);
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap_Retail;
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = a creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word
};
class CThingPlayerCreature : public CThing_Retail { };
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;              // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;        // 0x00487DD0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();               // 0x00449970
};
class CCharString;
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CWorld; class CMainGameComponent; class CDisplayEngine; class CThingSearchTools;
__forceinline CTCBase_Retail* GetTCIfPresent(CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;                            // deliberately unset when the component is absent (retail reads a dead slot)
    if (thing->HasTC(kind))
        tc = GetTC(thing, kind);
    return tc;
}
__forceinline CTCBase_Retail* GetTCIfPresentConst(const CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;
    if (thing->HasTC(kind))
        tc = GetTCConst(thing, kind);
    return tc;
}
template <class T> __forceinline bool GetTCOut(CThing_Retail* thing, long kind, T** out)   // the out-parameter lookup: the component stays in eax
{
    if (thing->HasTC(kind)) {
        *out = (T*)GetTC(thing, kind);
        return true;
    }
    return false;
}
class CCharString { public:                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
__forceinline bool IsThingAlive(const CScriptThing& thing)   // the thing exists and is not dead
{
    CThing_Retail* t = ((CThing_Retail*)thing.GetPThing());
    return t && !t->m_dead;
}
struct CTCAllies_Retail {
    void AddAlly(CThing_Retail* thing);            // 0x0076D270
    void RemoveAlly(CThing_Retail* thing);         // 0x0076C850
};
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                     World;             // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent*         Component;         // +0x08
    CDisplayEngine*             DisplayEngine;     // +0x0c
    CDefinitionManager_Retail*  DefinitionManager; // +0x10
    CPlayerManager*      PlayerManager;     // +0x14
    CThingSearchTools*          ThingSearchTools;  // +0x18
    void ImpSetEntityAsAllyOfThing(const CScriptThing& entity, const CScriptThing& other, bool isAlly) const;
};

void CGameScriptInterface::ImpSetEntityAsAllyOfThing(const CScriptThing& entity, const CScriptThing& other, bool isAlly) const
{
    if (entity.IsAlive() && other.IsAlive()) {
        CThing_Retail* t = ((CThing_Retail*)entity.GetPThing());
        if (t->HasTC(0x49)) {
            CTCAllies_Retail* allies = (CTCAllies_Retail*)GetTC(t, 0x49);
            CThing_Retail* o = ((CThing_Retail*)other.GetPThing());
            if (isAlly)
                allies->AddAlly(o);
            else
                allies->RemoveAlly(o);
        }
    }
}