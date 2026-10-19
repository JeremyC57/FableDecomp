#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
struct CTCBase;
struct CThingCreatureBase;
enum EUnsheatheWeaponType { UNSHEATHE_WEAPON_TYPE_DEFAULT };

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
};

bool __fastcall ShouldPlayUnsheatheWeaponAction(CThing* creature, EUnsheatheWeaponType type, bool* pCanUnsheathe,
                                                bool* pSheathe);   // 0x008B7CF0

struct CTCCombat {
    unsigned char        _pad0[0x6c];
    EUnsheatheWeaponType m_unsheatheWeaponType; // +0x6c
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};

struct CCombatActionBase {
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_UnsheatheWeapons : CCombatActionBase {
    EUnsheatheWeaponType WeaponType;            // +0x114
    virtual void Initialise();
};

void CCreatureAction_UnsheatheWeapons::Initialise()
{
    bool sheathe = false;
    bool canUnsheathe;
    ShouldPlayUnsheatheWeaponAction(PCreature, WeaponType, &canUnsheathe, &sheathe);
    {
        CCharString name(sheathe ? "STANDARD_SHEATHE" : "STANDARD_UNSHEATHE", -1);
        SetAnimationName(name);
    }
    CTCCombat* combat;
    if (PCreature.GetPItem()->GetInterface(&combat, 0x30))
        combat->m_unsheatheWeaponType = WeaponType;
    CCombatActionBase::Initialise();
}

// ---- stubs ----
static const char* g_lastName = 0;
static int g_baseInit = 0;
static bool g_sheatheOut = false;
static EUnsheatheWeaponType g_typeSeen;
static CThing* g_creatureSeen = 0;
CCharString::CCharString(const char* text, long) { m_pData = (void*)text; }
CCharString::~CCharString() { }
static CInterfaceMap::SEntry g_entries[2];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    for (SEntry* e = first; e != last; ++e) if (e->type >= *type) return e;
    return last;
}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
bool __fastcall ShouldPlayUnsheatheWeaponAction(CThing* creature, EUnsheatheWeaponType type, bool* pCan, bool* pSheathe)
{
    g_creatureSeen = creature; g_typeSeen = type; *pCan = true; if (g_sheatheOut) *pSheathe = true; return true;
}
void CCombatActionBase::v00() { }
void CCombatActionBase::Initialise() { ++g_baseInit; }
void CCombatActionBase::SetAnimationName(const CCharString& name) { g_lastName = (const char*)name.m_pData; }

int main()
{
    static CThing creature; std::memset(&creature, 0, sizeof(creature));
    static CTCCombat combat; combat.m_unsheatheWeaponType = (EUnsheatheWeaponType)0;
    g_entries[0].type = 0x30; g_entries[0].component = (CTCBase*)&combat;
    creature.m_interfaces.first = g_entries; creature.m_interfaces.last = g_entries + 1;
    creature.m_presentBits[1] = 0x10000;
    CCreatureAction_UnsheatheWeapons a;
    a.PCreature.m_pItem = &creature;
    a.WeaponType = (EUnsheatheWeaponType)3;
    bool ok = true;
    a.Initialise();
    if (std::strcmp(g_lastName, "STANDARD_UNSHEATHE") != 0 || combat.m_unsheatheWeaponType != 3 || g_baseInit != 1
        || g_creatureSeen != &creature || g_typeSeen != 3) ok = false;
    g_sheatheOut = true; combat.m_unsheatheWeaponType = (EUnsheatheWeaponType)0;
    creature.m_presentBits[1] = 0;   // no combat component: no store
    a.Initialise();
    if (std::strcmp(g_lastName, "STANDARD_SHEATHE") != 0 || combat.m_unsheatheWeaponType != 0 || g_baseInit != 2) ok = false;
    if (!ok) { std::printf("008b7ed0_TEST FAIL\n"); return 1; }
    std::printf("008b7ed0_TEST PASS\n");
    return 0;
}
