// NEntityEvents::CEventDied::OnAdd(const CTCEntityEvents& events) -- when the killer is a hero with hero stats, tell the stats about the kill (a CKilledThing modifier event carrying the dead thing's creature-def value) unless the dead thing is flagged or is a ghost sword
// Leaf: OnAdd
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z; 004365b0: ?LowerBound@CInterfaceMap@@QBEPBUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008115A0, length 297 bytes (+ int3 padding), `ret 4`.
// CEventDied vtable 0x0125AAF4 slot 2 (Ego_r.pdb ?OnAdd@CEventDied@NEntityEvents@@UAEXABVCTCEntityEvents@@@Z).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), the interface map LowerBound @ 0x0040F020 and its const
// copy @ 0x004365B0, CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall), CDefinitionManager::GetDef<T> @ 0x0042AF3C,
// CDefClassBase::GetSubDef<CCreatureDef> @ 0x00485815 (landed; the catalog says CAbilityDef), CTCGhostSword::IsThingGhostSword(const CThing*) @ 0x007C53F0 (static __fastcall),
// CTCHeroStats::InformOfEvent(const NHeroStatModifierEvent::CBase&) @ 0x0057A56C, the defs' virtual slot 1 on release.
// Vtable: NHeroStatModifierEvent::CKilledThing 0x0127233C (RTTI) for the stack event.
// Layout: the event's killer intelligent pointer +8; CTCEntityEvents ParentThing +4; retail CThing interface bitset
// +0x20, interface map +0x44, def index (word) +0x8c; CCreatureDef's reported value +0x40; component 0xa0's flag +0x10.
// Shape: the out-parameter GetInterface template; the def index goes through inline GetThing()/GetGlobalDefIndex()
// accessors (a direct member read loads the thing before the hero-stats component); the def pointers are CDefPointer locals released by their inline
// destructors at the end (null test, DecRefToZero, `dec [p+4] ; jne`, slot 1). Same-TU stand-ins compiled above the
// function under `auto_inline(off)` (retail's LTCG knowledge, precedent 0x00848730): both interface-map LowerBound
// copies (0x0040F020 / const 0x004365B0), CDefinitionManager::GetDef<T> (0x0042AF3C, the landed body) and
// CDefClassBase::GetSubDef<CCreatureDef> (0x00485815, the landed body) -- VC7.1 then knows none of them keeps the
// address of the key or the def pointers, so the 4-key shares its frame word with the def pointer, the CCreatureDef
// pointer lives in the dead `events` slot and the 0xa0 key shares its word with the stack event.
#include "fable_compiler.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad54[0x38];
    unsigned short m_globalDefIndex;                    // +0x8c
    long GetGlobalDefIndex() const { return m_globalDefIndex; }
    template <class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template <class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
static FABLE_FORCEINLINE bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +0x04
};
template <class T> class CDefPointer { public:
    CDefPointee* Object;                                // the pointee as its non-const base (engine/CDefPointer.h)
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->m_refCount)) Object->Destroy(); } }
    T* operator->() const { return static_cast<T*>(Object); }
    template <class U> CDefPointer& operator=(const CDefPointer<U>& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->m_refCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_refCount;
        }
        return *this;
    }
};
class CDefinitionManager;
class CDefClassBase : public CDefPointee { public:
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual long* GetSubDefIndex(const CCharString& name) const;   // slot 14 (+0x38)
    unsigned char _pad08[0x1c - 0x08];
    CDefinitionManager* m_pManager;                     // +0x1c
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x00485815 GetSubDef<CCreatureDef> (pushes "CCreatureDef")
};
class CThingBaseDef : public CDefClassBase { public: };
class CCreatureDef : public CDefClassBase { public:
    unsigned char _pad20[0x40 - 0x20];
    long m_value40;                                     // +0x40 the value the kill event reports
};
class CDefinitionManager { public:
    friend class CDefClassBase;
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0 (Ego_r.pdb QBE; hidden result pointer)
    template <class T> bool GetDef(long globalIndex, CDefPointer<const T>* out) const;   // 0x0042AF3C GetDef<CThingBaseDef>
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (Ego_r.pdb IAE: the twin after the const one)
};
#pragma auto_inline(off)
// Same-TU stand-ins: the landed bodies of GetDef<T> 0x0042AF3C and GetSubDef<CCreatureDef> 0x00485815.
template <> __declspec(noinline) bool CDefinitionManager::GetDef<CThingBaseDef>(long globalIndex, CDefPointer<const CThingBaseDef>* out) const
{
    if (globalIndex <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(globalIndex);
    if (def.Object) {
        *out = def;
        return true;
    }
    return false;
}
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    long* pIndex;
    {
        CCharString name("CCreatureDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CDefClassBase> base = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        CDefPointer<const CCreatureDef> result;
        result.Object = base.Object;
        if (result.Object)
            ++result.Object->m_refCount;
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
};

namespace NHeroStatModifierEvent {
class CBase { public:
    virtual void v00();
};
struct CKilledThing : CBase {                           // vtable 0x0127233C
    long m_value;
    CKilledThing(long value) : m_value(value) {}
    virtual void v00();
};
}
class CTCHeroStats { public:
    void InformOfEvent(const NHeroStatModifierEvent::CBase& event);   // 0x0057A56C (Ego_r.pdb, by reference)
};
struct CTCInterface_a0 {
    unsigned char _pad00[0x10];
    bool m_flag10;                                      // +0x10
};
class CTCGhostSword { public:
    static bool __fastcall IsThingGhostSword(const CThing* thing);   // 0x007C53F0 (Ego_r.pdb static)
};

class CTCEntityEvents { public:
    void* _vptr;
    CThing* ParentThing;                                // +0x04
    const CThing* GetThing() const { return ParentThing; }
};

namespace NEntityEvents {
class CEventBase { public:
    virtual void v00();
    virtual void v01();
    virtual void OnAdd(const CTCEntityEvents& events);
    long m_frame;                                       // +0x04
};
struct CEventDied : CEventBase {
    CBaseIntelligentPointer m_pKiller;                  // +0x08
    virtual void OnAdd(const CTCEntityEvents& events);
};

void CEventDied::OnAdd(const CTCEntityEvents& events)
{
    if (((CThing*)m_pKiller.GetPItem()) == 0)
        return;
    CTCHeroStats* stats;
    if (!((CThing*)m_pKiller.GetPItem())->GetInterface(&stats, 4))
        return;
    CDefPointer<const CThingBaseDef> def;
    CGameDefinitionManager::Get().GetDef(events.GetThing()->GetGlobalDefIndex(), &def);
    CDefPointer<const CCreatureDef> creatureDef;
    def->GetSubDef(&creatureDef);
    const CThing* thing = events.ParentThing;
    bool flagged = false;
    CTCInterface_a0* tc;
    if (thing->GetInterface(&tc, 0xa0) && tc->m_flag10)
        flagged = true;
    bool ghost = CTCGhostSword::IsThingGhostSword(thing);
    if (!flagged && !ghost) {
        NHeroStatModifierEvent::CKilledThing event(creatureDef->m_value40);
        stats->InformOfEvent(event);
    }
}
}