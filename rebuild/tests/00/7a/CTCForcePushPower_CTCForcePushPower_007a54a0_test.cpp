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

class CSpecialAbilitiesForcePushDef {
public:
    virtual void v00();
    long RefCount;                                  // +0x04
    unsigned char _pad08[0x98 - 0x08];
    CRetailVector<float> TimeDelayBetweenCastsSec;  // +0x98
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
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x00798C10 for the force-push def
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
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
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

class CTCForcePushPower : public CTCBase {          // vtable 0x0126C9BC
public:
    CDefPointer<const CSpecialAbilitiesForcePushDef> PDef;   // +0x0C
    long SpellLevel;                                // +0x10
    CIntelligentPointer<CThing> PItemRemoved;       // +0x14
    bool Casted;                                    // +0x1C
    long CurTimeDelayBetweenCastsFrames;            // +0x20
    bool IgnoreFriendlies;                          // +0x24
    CTCForcePushPower(CThing& parent);
    virtual void v00();
};

CTCForcePushPower::CTCForcePushPower(CThing& parent)
    : CTCBase(&parent), SpellLevel(0), PItemRemoved(0), Casted(false), CurTimeDelayBetweenCastsFrames(0),
      IgnoreFriendlies(false)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_FORCE_PUSH_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    if (ParentThing->HasTC(0x6f))
        SpellLevel = ((CTCInventoryAbilities*)ParentThing->GetTC(0x6f))->GetAbilityLevel(1);
    CurTimeDelayBetweenCastsFrames = (long)(g_framesPerSecond * PDef->TimeDelayBetweenCastsSec._M_start[SpellLevel]);
}

// ---- test stubs for the retail callees ----
static const char* g_lastName; static long g_lastClassIndex; static long g_defIndex;
static CSpecialAbilitiesForcePushDef g_def;
static CGameDefinitionManager g_manager;
static long g_ability; static long g_abilityLevel = 2;
CCharString::CCharString(const char* text, long) : PStringData((void*)text) {}
CCharString::~CCharString() {}
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_manager; }
long CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString& n, long ci) const
{ g_lastName = (const char*)n.PStringData; g_lastClassIndex = ci; return 7; }
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
void CSpecialAbilitiesForcePushDef::v00() {}
void CTCForcePushPower::v00() {}

int main()
{
    int ok = 1;
    float delays[3] = { 1.0f, 0.5f, 2.0f };
    g_def.TimeDelayBetweenCastsSec._M_start = delays;
    CTCInventoryAbilities abilities;
    CInterfaceMap::SEntry e; e.type = 0x6f; e.component = (CTCBase*)&abilities;
    CThing thing; std::memset(&thing, 0, sizeof(thing));
    thing.m_interfaces.first = &e; thing.m_interfaces.last = &e + 1;
    thing.m_presentBits[3] = 1u << 15;
    CTCForcePushPower a(thing);
    if (a.ParentThing != &thing || a.PDef.Object != &g_def || a.SpellLevel != 2 || g_ability != 1) ok = 0;
    if (a.PItemRemoved.m_pItem != 0 || a.Casted || a.IgnoreFriendlies) ok = 0;
    if (a.CurTimeDelayBetweenCastsFrames != 60 || g_defIndex != 7 || g_lastClassIndex != 1) ok = 0;
    if (std::strcmp(g_lastName, "SPECIAL_ABILITIES_FORCE_PUSH_DEF") != 0) ok = 0;
    thing.m_presentBits[3] = 0;   // no ability inventory -> level 0
    CTCForcePushPower b(thing);
    if (b.SpellLevel != 0 || b.CurTimeDelayBetweenCastsFrames != 30) ok = 0;
    if (!ok) { std::printf("007a54a0_TEST FAIL\n"); return 1; }
    std::printf("007a54a0_TEST PASS\n");
    return 0;
}