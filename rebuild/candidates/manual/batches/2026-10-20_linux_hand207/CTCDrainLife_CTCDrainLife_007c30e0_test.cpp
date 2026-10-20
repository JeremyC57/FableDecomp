#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#include <cstdio>
#include <cstring>
class CThing;
class CBaseObject;
class CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSpecialAbilitiesDrainLifeDef {
public:
    virtual void v00();
    long RefCount;                                  // +0x04
    unsigned char _pad08[0x6c - 0x08];
    float SpellDelaySecs;                           // +0x6C
};

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return Object; }
};

class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x007C3A60 for the drain-life def
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();                                     // 0x0044C6B0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

template <class T> struct SRetailAllocProxy {       // STLport _STLP_alloc_proxy
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};

class CTCInventoryAbilities {
public:
    long GetAbilityLevel(long ability);             // 0x005EDE93 (EHeroAbility)
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

extern long g_framesPerSecond;                      // 0x01375550

class CTCBase {
public:
    CTCBase(CThing* parent);                        // 0x00686800
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x4];                      // ToBeRemoved, IsInGlobalUpdateTCs
};

class CTCDrainLife : public CTCBase {               // vtable 0x0126E284
public:
    CDefPointer<const CSpecialAbilitiesDrainLifeDef> PDef;   // +0x0C
    long SpellLevel;                                // +0x10
    CIntelligentPointer<CThing> PItemRemoved;       // +0x14
    CRetailVector< CIntelligentPointer<CThing> > Shots;      // +0x1C
    bool JustReleased;                              // +0x28
    long SpellDelayFrames;                          // +0x2C
    CTCDrainLife(CThing& parent);
    virtual void v00();
};

CTCDrainLife::CTCDrainLife(CThing& parent)
    : CTCBase(&parent), SpellLevel(0), PItemRemoved(0), JustReleased(true)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_DRAIN_LIFE_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    CTCInventoryAbilities* abilities;
    if (ParentThing->GetInterface(&abilities, 0x6f))
        SpellLevel = abilities->GetAbilityLevel(6);
    SpellDelayFrames = (long)(g_framesPerSecond * PDef->SpellDelaySecs);
}

// ---- test stubs for the retail callees ----
static const char* g_lastName; static long g_lastClassIndex; static long g_defIndex;
static CSpecialAbilitiesDrainLifeDef g_def;
static CGameDefinitionManager g_manager;
static long g_ability; static long g_abilityLevel = 3;
CCharString::CCharString(const char* text, long) : PStringData((void*)text) {}
CCharString::~CCharString() {}
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_manager; }
long CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString& n, long ci) const
{ g_lastName = (const char*)n.PStringData; g_lastClassIndex = ci; return 42; }
template<class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{ g_defIndex = index; out->Object = &g_def; return true; }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem((CThing*)1) {}
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
void CBaseIntelligentPointer::v00() {}
template<class T> void CIntelligentPointer<T>::v00() {}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
long CTCInventoryAbilities::GetAbilityLevel(long ability) { g_ability = ability; return g_abilityLevel; }
long g_framesPerSecond = 30;
CTCBase::CTCBase(CThing* parent) : ParentThing(parent) {}
void CTCBase::v00() {}
void CSpecialAbilitiesDrainLifeDef::v00() {}
void CTCDrainLife::v00() {}

int main()
{
    int ok = 1;
    g_def.SpellDelaySecs = 1.5f;
    CTCInventoryAbilities abilities;
    CInterfaceMap::SEntry e; e.type = 0x6f; e.component = (CTCBase*)&abilities;
    CThing thing; std::memset(&thing, 0, sizeof(thing));
    thing.m_interfaces.first = &e; thing.m_interfaces.last = &e + 1;
    thing.m_presentBits[3] = 1u << 15;
    CTCDrainLife a(thing);
    if (a.ParentThing != &thing || a.PDef.Object != &g_def || a.SpellLevel != 3 || g_ability != 6) ok = 0;
    if (a.PItemRemoved.m_pItem != 0 || a.Shots._M_start || a.Shots._M_finish || a.Shots._M_end_of_storage._M_data) ok = 0;
    if (!a.JustReleased || a.SpellDelayFrames != 45 || g_defIndex != 42 || g_lastClassIndex != 1) ok = 0;
    if (std::strcmp(g_lastName, "SPECIAL_ABILITIES_DRAIN_LIFE_DEF") != 0) ok = 0;
    thing.m_presentBits[3] = 0;   // no ability inventory -> level 0
    CTCDrainLife b(thing);
    if (b.SpellLevel != 0 || b.SpellDelayFrames != 45) ok = 0;
    if (!ok) { std::printf("007c30e0_TEST FAIL\n"); return 1; }
    std::printf("007c30e0_TEST PASS\n");
    return 0;
}
