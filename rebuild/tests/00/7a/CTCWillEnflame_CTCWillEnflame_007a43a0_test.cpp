#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
class CThing;
class CBaseObject;
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSpecialAbilitiesEnflameDef {
public:
    virtual void v00();
    long RefCount;                                  // +0x04
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
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x007A5080 for the enflame def
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();                                     // 0x0044C6B0
};

template<> bool CDefinitionManager::GetDef<CSpecialAbilitiesEnflameDef>(long index, CDefPointer<const CSpecialAbilitiesEnflameDef>* out) const;   // test: declared before use
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
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
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
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

struct CTCSpellCastState {                          // interface type 0xA7 (class not pinned)
    unsigned char _pad00[0x0c];
    bool Active;                                    // +0x0c
    unsigned char _pad0d[3];
    long Mode;                                      // +0x10
};

class CTCBase {
public:
    CTCBase(CThing* parent);                        // 0x00686800
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x4];                      // ToBeRemoved, IsInGlobalUpdateTCs
};

#pragma pack(push, 1)
class CTCWillEnflame : public CTCBase {             // vtable 0x0126C824
public:
    bool EnflameActionInitialised;                  // +0x0C
    bool Released;                                  // +0x0D
    unsigned char _pad0e[2];
    CDefPointer<const CSpecialAbilitiesEnflameDef> PDef;   // +0x10
    long SpellLevel;                                // +0x14
    CIntelligentPointer<CThing> PItemRemoved;       // +0x18
    bool IsZTargetting;                             // +0x20
    CIntelligentPointer<CThing> PTarget;            // +0x21 (unaligned, as the PDB)
    CTCWillEnflame(CThing* parent);
    virtual void v00();
};
#pragma pack(pop)

CTCWillEnflame::CTCWillEnflame(CThing* parent)
    : CTCBase(parent), EnflameActionInitialised(false), Released(false), SpellLevel(0), PItemRemoved(0),
      IsZTargetting(false), PTarget(0)
{
    if (ParentThing->HasTC(0xa7)) {
        CTCSpellCastState* state = (CTCSpellCastState*)ParentThing->GetTC(0xa7);
        state->Active = true;
        state->Mode = 3;
    }
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_ENFLAME_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    const CThing* parentThing = ParentThing;
    if (parentThing->HasTC(0x6f))
        SpellLevel = ((CTCInventoryAbilities*)parentThing->GetTC(0x6f))->GetAbilityLevel(3);
}
// ---- test doubles ----
static const char* g_name = 0;
CCharString::CCharString(const char* t, long) : PStringData((void*)t) { g_name = t; }
CCharString::~CCharString() {}
static CGameDefinitionManager g_gdm;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_gdm; }
long CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString& n, long ci) const { return (std::strcmp((const char*)n.PStringData, "SPECIAL_ABILITIES_ENFLAME_DEF") == 0 && ci == 1) ? 42 : -1; }
static CSpecialAbilitiesEnflameDef* g_def = 0; static long g_defIndex = 0;
template<> bool CDefinitionManager::GetDef<CSpecialAbilitiesEnflameDef>(long index, CDefPointer<const CSpecialAbilitiesEnflameDef>* out) const
{ g_defIndex = index; out->Object = g_def; return true; }
void CSpecialAbilitiesEnflameDef::v00() {}
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem((CThing*)1) {}
void CBaseIntelligentPointer::SetPItem(CBaseObject* p) { m_pItem = (CThing*)p; }
template<> void CIntelligentPointer<CThing>::v00() {}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static long g_ability = -1;
long CTCInventoryAbilities::GetAbilityLevel(long a) { g_ability = a; return 2; }
CTCBase::CTCBase(CThing* p) : ParentThing(p) {}
void CTCBase::v00() {}
void CTCWillEnflame::v00() {}
int main()
{
    CSpecialAbilitiesEnflameDef def; g_def = &def;
    CTCSpellCastState state; std::memset(&state, 0, sizeof state);
    CTCInventoryAbilities abilities;
    CThing owner; std::memset(&owner, 0, sizeof owner);
    CInterfaceMap::SEntry e[2] = { { 0x6f, (CTCBase*)&abilities }, { 0xa7, (CTCBase*)&state } };
    owner.m_interfaces.first = e; owner.m_interfaces.last = e + 2;
    owner.m_presentBits[0xa7 >> 5] |= 1UL << (0xa7 & 31); owner.m_presentBits[0x6f >> 5] |= 1UL << (0x6f & 31);
    CTCWillEnflame w(&owner);
    bool ok = !w.EnflameActionInitialised && !w.Released && w.PDef.Object == &def && g_defIndex == 42 && w.SpellLevel == 2
        && g_ability == 3 && w.PItemRemoved.m_pItem == 0 && !w.IsZTargetting && w.PTarget.m_pItem == 0
        && state.Active && state.Mode == 3 && w.ParentThing == &owner;
    CThing bare; std::memset(&bare, 0, sizeof bare); bare.m_interfaces.first = e; bare.m_interfaces.last = e;
    g_ability = -1;
    CTCWillEnflame w2(&bare);
    ok = ok && w2.SpellLevel == 0 && g_ability == -1 && w2.PDef.Object == &def;
    if (!ok) { std::printf("007a43a0_TEST FAIL\n"); return 1; }
    std::printf("007a43a0_TEST PASS\n");
    return 0;
}