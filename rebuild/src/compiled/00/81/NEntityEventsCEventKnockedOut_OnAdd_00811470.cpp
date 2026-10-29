// NEntityEvents::CEventKnockedOut::OnAdd(const CTCEntityEvents& events) -- when the knocker-out is a hero with hero stats, tell the stats about it (a CKnockedThingUnconscious modifier event carrying the victim's creature-def value) unless the victim is flagged or is a ghost sword
// Leaf: OnAdd
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00811470, length 297 bytes (+ int3 padding), `ret 4`.
// Label correction: the catalog says CEventDied::OnAdd (byte twin 0x008115A0); this copy stores the
// NHeroStatModifierEvent::CKnockedThingUnconscious vtable 0x01272334, so it is ?OnAdd@CEventKnockedOut@NEntityEvents@@
// (Ego_r.pdb); no pointer to it survives in the exe (no vtable slot references it).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), the interface map LowerBound @ 0x0040F020 and its const
// copy @ 0x004365B0, CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall), CDefinitionManager::GetDef<T> @ 0x0042AF3C,
// CDefClassBase::GetSubDef<CCreatureDef> @ 0x00485815 (landed; the catalog says CAbilityDef), IsThingGhostSword(CThing*) @ 0x007C53F0 (__fastcall),
// CTCHeroStats::InformOfEvent(const NHeroStatModifierEvent::CBase*) @ 0x0057A56C, the defs' virtual slot 1 on release.
// Vtable: NHeroStatModifierEvent::CKnockedThingUnconscious 0x01272334 (RTTI) for the stack event.
// Layout: the event's knocked-out-by intelligent pointer +8 (GetPKnockedOutBy); CTCEntityEvents ParentThing +4; retail CThing interface bitset
// +0x20, interface map +0x44, def index (word) +0x8c; CCreatureDef's reported value +0x40; component 0xa0's flag +0x10.
// Shape: the out-parameter GetInterface template; the def index goes through inline GetThing()/GetGlobalDefIndex()
// accessors (a direct member read loads the thing before the hero-stats component); the def pointers are CDefPointer locals released by their inline
// destructors at the end (null test, DecRefToZero, `dec [p+4] ; jne`, slot 1). Same-TU stand-ins compiled above the
// function under `auto_inline(off)` (retail's LTCG knowledge, precedent 0x00848730): both interface-map LowerBound
// copies (0x0040F020 / const 0x004365B0), CDefinitionManager::GetDef<T> (0x0042AF3C, the landed body) and
// CDefClassBase::GetSubDef<CCreatureDef> (0x00485815, the landed body) -- VC7.1 then knows none of them keeps the
// address of the key or the def pointers, so the 4-key shares its frame word with the def pointer, the CCreatureDef
// pointer lives in the dead `events` slot and the 0xa0 key shares its word with the stack event.
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-ins for the two LowerBound copies (their retail body: lower_bound over the 8-byte entries).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short m_globalDefIndex;                    // +0x8c
    long GetGlobalDefIndex() const { return m_globalDefIndex; }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CBaseObject;
struct CBaseIntelligentPointer {
    void* _vptr;
    void* m_pItem;
    CBaseObject* GetPItem() const;                           // 0x00A01B50
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->m_refCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
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
class CCreatureDef : public CDefPointee { public:
    unsigned char _pad08[0x40 - 0x08];
    long m_value40;                                     // +0x40 the value the kill event reports
};
struct CDefinitionManagerBase {
    CDefPointer<CDefPointee> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CDefClassBase : public CDefPointee { public:
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad08[0x1c - 0x08];
    CDefinitionManagerBase* m_pManager;                 // +0x1c
    bool GetSubDef(CDefPointer<CCreatureDef>* out);     // 0x00485815
};
class CDefinitionManager { public:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009AD9E0 (hidden result pointer)
    bool GetDef(long globalIndex, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C
};
#pragma auto_inline(off)
// Same-TU stand-ins: the landed bodies of GetDef<T> 0x0042AF3C and GetSubDef<CCreatureDef> 0x00485815.
bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CDefClassBase>* out)
{
    if (globalIndex <= 0)
        return false;
    CDefPointer<CDefClassBase> def = GetPDefFromGlobalIndex(globalIndex);
    if (def.Object) {
        *out = def;
        return true;
    }
    return false;
}
bool CDefClassBase::GetSubDef(CDefPointer<CCreatureDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCreatureDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CDefPointee> base = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        CDefPointer<CCreatureDef> result;
        result.Object = (CCreatureDef*)base.Object;
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
struct CKnockedThingUnconscious : CBase {               // vtable 0x01272334
    long m_value;
    CKnockedThingUnconscious(long value) : m_value(value) {}
    virtual void v00();
};
}
class CTCHeroStats { public:
    void InformOfEvent(const NHeroStatModifierEvent::CBase* event);   // 0x0057A56C
};
struct CTCInterface_a0 {
    unsigned char _pad00[0x10];
    bool m_flag10;                                      // +0x10
};
bool __fastcall IsThingGhostSword(const CThing* thing); // 0x007C53F0

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
class CEventKnockedOut : public CEventBase { public:
    CBaseIntelligentPointer m_pKnockedOutBy;            // +0x08
    virtual void OnAdd(const CTCEntityEvents& events);
};

void CEventKnockedOut::OnAdd(const CTCEntityEvents& events)
{
    if (((CThing*)m_pKnockedOutBy.GetPItem()) == 0)
        return;
    CTCHeroStats* stats;
    if (!((CThing*)m_pKnockedOutBy.GetPItem())->GetInterface(&stats, 4))
        return;
    CDefPointer<CDefClassBase> def;
    CGameDefinitionManager::Get().GetDef(events.GetThing()->GetGlobalDefIndex(), &def);
    CDefPointer<CCreatureDef> creatureDef;
    def->GetSubDef(&creatureDef);
    const CThing* thing = events.ParentThing;
    bool flagged = false;
    CTCInterface_a0* tc;
    if (thing->GetInterface(&tc, 0xa0) && tc->m_flag10)
        flagged = true;
    bool ghost = IsThingGhostSword(thing);
    if (!flagged && !ghost) {
        NHeroStatModifierEvent::CKnockedThingUnconscious event(creatureDef->m_value40);
        stats->InformOfEvent(&event);
    }
}
}