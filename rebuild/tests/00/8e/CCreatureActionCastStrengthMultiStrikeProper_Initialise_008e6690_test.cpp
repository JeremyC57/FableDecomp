// Behaviour test for 008e6690: src/008e6690.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CCreatureAction_CastStrengthMultiStrikeProper::Initialise()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCreatureAction_CastStrengthMultiStrikeProper::Initialise() -- load SPECIAL_ABILITIES_MULTI_STRIKE_DEF, play its sound for this spell level through the owner's sound component (if any), then CCombatActionBase::Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6690, length 226 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("SPECIAL_ABILITIES_MULTI_STRIKE_DEF" @ 0x01235A9C), CGameDefinitionManager::Get()
// 0x0044C6B0 (`__fastcall`, twice), the manager's GetDefIndexFromName(const CCharString&, long) 0x009AD390 (manifest label
// "COptimisedPrimitive::ActualToUniqueIndex" is propagated), CDefinitionManager::GetDef<CSpecialAbilitiesMultiStrikeDef>(long,
// CDefPointer*) 0x007CD760, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0x10),
// CDefStringTable::GetString(CDefString) 0x009D49B0 on the global table @ 0x013CA828 (CCharString by hidden pointer), the sound
// component's play-by-name 0x0066BD00, CCombatActionBase::Initialise 0x00857520; the def's virtual slot 1 on the last release.
// Shape: the def-name string is a named local in its own block (as the landed CTCHeroStats::OnInitialActivate 0x0057D1A4); the
// def pointer is a function-scope local released at the end (refcount at +4); Sounds[SpellLevel - 1] reads `[def+level*4+3Ch]`.
// Layout: SpellLevel at retail +0x114 (PDB +0x12c); CSpecialAbilitiesMultiStrikeDef Sounds (CDefString[4]) at +0x40 (Ego_r.pdb).
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

class CDefString { public: long Index; };

struct CDefStringTable {
    CCharString GetString(CDefString name);     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828

class CSpecialAbilitiesMultiStrikeDef { public:
    virtual void v00();
    virtual void Release();                     // slot 1 (+4): destroys the def when the last reference goes
    long          RefCount;                     // +0x04
    unsigned char _pad08[0x40 - 0x08];
    CDefString    Sounds[4];                    // +0x40
    CDefString    Animations[4];                // +0x50
};

struct CDefPointerMultiStrike {                 // CDefPointer<const CSpecialAbilitiesMultiStrikeDef>
    CSpecialAbilitiesMultiStrikeDef* Object;
    CDefPointerMultiStrike() : Object(0) { }
    ~CDefPointerMultiStrike()
    {
        if (Object && --Object->RefCount == 0)
            Object->Release();
    }
    CSpecialAbilitiesMultiStrikeDef* operator->() const { return Object; }
};

class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                      // 0x0044C6B0
    long GetDefIndexFromName(const CCharString& name, long column);       // 0x009AD390
    bool GetDef(long index, CDefPointerMultiStrike* out);                 // 0x007CD760
};
void* FindDef_CGameDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long index, CDefPointerMultiStrike* out)
{
    void* def = FindDef_CGameDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CTCSound {                               // the interface-0x10 component
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
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

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
};

class CCreatureAction_CastStrengthMultiStrikeProper : public CCombatActionBase {
public:
    long SpellLevel;                            // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_CastStrengthMultiStrikeProper::Initialise()
{
    CDefPointerMultiStrike def;
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_MULTI_STRIKE_DEF");
        index = CGameDefinitionManager::Get().GetDefIndexFromName(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &def);
    CTCSound* sound;
    if (PCreature.GetPItem()->GetInterface(&sound, 0x10)) {
        CCharString soundName = g_DefStringTable.GetString(def->Sounds[SpellLevel - 1]);
        sound->PlaySound(soundName, 1, 0, 0, -1);
    }
    CCombatActionBase::Initialise();
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_released = 0; static int g_strCtor = 0, g_strDtor = 0;
static long g_column = -1; static long g_soundAsked = -1; static int g_play = 0; static long g_pa = -9, g_pb = -9, g_pc = -9, g_pd = -9; static const void* g_soundObj = 0;
static int g_baseInit = 0; static CThing* g_item = 0;
void* FindDef_CGameDefinitionManager_GetDef_0() { return g_def; }
CCharString::CCharString(const char* text, long) { ++g_strCtor; PStringData = (void*)text; }
CCharString::~CCharString() { ++g_strDtor; }
CDefStringTable g_DefStringTable;
CCharString CDefStringTable::GetString(CDefString name) { g_soundAsked = name.Index; return CCharString("SND", -1); }
void CSpecialAbilitiesMultiStrikeDef::v00() {}
void CSpecialAbilitiesMultiStrikeDef::Release() { ++g_released; }
static CGameDefinitionManager g_mgr;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_mgr; }
long CGameDefinitionManager::GetDefIndexFromName(const CCharString&, long column) { g_column = column; return 5; }
unsigned long CTCSound::PlaySound(const CCharString&, long a, long b, long c, long d) { ++g_play; g_soundObj = this; g_pa = a; g_pb = b; g_pc = c; g_pd = d; return 0; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CCombatActionBase::Initialise() { ++g_baseInit; }
int main()
{
    CSpecialAbilitiesMultiStrikeDef def; def.RefCount = 3; for (int i = 0; i < 4; ++i) { def.Sounds[i].Index = 10 + i; def.Animations[i].Index = 0; }
    g_def = &def;
    int soundObj = 0;
    CInterfaceMap::SEntry e[2]; e[0].type = 0x10; e[0].component = (CTCBase*)&soundObj; e[1].type = 0x20; e[1].component = 0;
    CThing creature; memset(&creature, 0, sizeof(creature)); creature.m_interfaces.first = e; creature.m_interfaces.last = e + 2; creature.m_presentBits[0] = 1UL << 0x10;
    CCreatureAction_CastStrengthMultiStrikeProper a; a.PCreature.__vftable = 0; a.PCreature.m_pItem = &creature; a.SpellLevel = 2;
    a.Initialise();
    CHECK(g_column == 1); CHECK(g_soundAsked == 11); CHECK(g_play == 1 && g_soundObj == &soundObj); CHECK(g_pa == 1 && g_pb == 0 && g_pc == 0 && g_pd == -1);
    CHECK(g_baseInit == 1); CHECK(def.RefCount == 2); CHECK(g_strCtor == 2 && g_strDtor == 2);
    creature.m_presentBits[0] = 0; a.SpellLevel = 4; def.RefCount = 1; a.Initialise();
    CHECK(g_play == 1); CHECK(g_baseInit == 2); CHECK(g_released == 1); CHECK(g_strCtor == 3 && g_strDtor == 3);
    if (fails == 0) printf("008e6690_TEST PASS\n"); else printf("008e6690_TEST FAIL\n");
    return fails;
}