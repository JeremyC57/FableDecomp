// Behaviour test for 008e6c90: src/008e6c90.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CCreatureAction_CastStrengthMultiStrikeProper::CCreatureAction_CastStrengthMultiStrikeProper(CThingCreatureBase*, long)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCreatureAction_CastStrengthMultiStrikeProper::CCreatureAction_CastStrengthMultiStrikeProper(CThingCreatureBase*, long) -- untargeted form: base CCombatActionBase(owner, 150, zeroed sound), SpellLevel, then the animation named by SPECIAL_ABILITIES_MULTI_STRIKE_DEF's Animations[SpellLevel - 1]
// Leaf: ??0CCreatureAction_CastStrengthMultiStrikeProper@@QAE@PAUCThingCreatureBase@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6C90, length 210 bytes, `ret 8`. Vtable 0x012877A4.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) 0x00858030, CCharString ctor
// 0x0099EBF0 / dtor 0x0099EAE0 ("SPECIAL_ABILITIES_MULTI_STRIKE_DEF" @ 0x01235A9C), CGameDefinitionManager::Get() 0x0044C6B0 (twice),
// GetDefIndexFromName 0x009AD390, GetDef<CSpecialAbilitiesMultiStrikeDef> 0x007CD760, CDefStringTable::GetString 0x009D49B0 (global
// table @ 0x013CA828), SetAnimationName 0x008574D0; the def's virtual slot 1 on the last release.
// Shape: as CCreatureAction_CastStrengthMultiStrikeProper::Initialise 0x008E6690 (named def-name block, function-scope CDefPointer);
// the GetString result is a temporary argument of SetAnimationName; Animations[SpellLevel - 1] reads `[def+level*4+4Ch]`.
// Layout: SpellLevel at retail +0x114 (PDB +0x12c); Animations (CDefString[4]) at def +0x50 (Ego_r.pdb).
struct CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CDefString { long Index; };

struct CDefStringTable {
    CCharString GetString(CDefString name);     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828

struct CSpecialAbilitiesMultiStrikeDef {
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

struct CGameDefinitionManager {
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

struct CThingCreatureBase;
struct CThing;
struct CSoundPair {                             // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                     // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_CastStrengthMultiStrikeProper : CCombatActionBase {   // vtable 0x012877A4
    long SpellLevel;                            // +0x114 (PDB +0x12c)
    virtual void v00();
    CCreatureAction_CastStrengthMultiStrikeProper(CThingCreatureBase* owner, CThing* target, long spellLevel);
    CCreatureAction_CastStrengthMultiStrikeProper(CThingCreatureBase* owner, long spellLevel);
};

CCreatureAction_CastStrengthMultiStrikeProper::CCreatureAction_CastStrengthMultiStrikeProper(CThingCreatureBase* owner, long spellLevel)
    : CCombatActionBase(owner, 150, CSoundPair())
    , SpellLevel(spellLevel)
{
    CDefPointerMultiStrike def;
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_MULTI_STRIKE_DEF");
        index = CGameDefinitionManager::Get().GetDefIndexFromName(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &def);
    {
        CCharString animation = g_DefStringTable.GetString(def->Animations[SpellLevel - 1]);
        SetAnimationName(animation);
    }
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_released = 0; static int g_strCtor = 0, g_strDtor = 0; static char g_lastText[64];
static long g_column = -1; static long g_nameIndex = 0x77; static long g_getDefIndex = -1; static long g_animAsked = -1; static const CCharString* g_animSet = 0;
static CThingCreatureBase* g_owner = 0; static CThing* g_target = 0; static long g_code = 0; static long g_sound0 = -1, g_sound1 = -1;
void* FindDef_CGameDefinitionManager_GetDef_0() { return g_def; }
CCharString::CCharString(const char* text, long) { ++g_strCtor; strncpy(g_lastText, text, 63); PStringData = (void*)text; }
CCharString::~CCharString() { ++g_strDtor; }
CDefStringTable g_DefStringTable;
CCharString CDefStringTable::GetString(CDefString name) { g_animAsked = name.Index; return CCharString("ANIM", -1); }
void CSpecialAbilitiesMultiStrikeDef::v00() {}
void CSpecialAbilitiesMultiStrikeDef::Release() { ++g_released; }
static CGameDefinitionManager g_mgr;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_mgr; }
long CGameDefinitionManager::GetDefIndexFromName(const CCharString& name, long column) { g_column = column; return g_nameIndex; }
CCombatActionBase::CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
{ g_owner = owner; g_target = 0; g_code = actionCode; g_sound0 = sound.FirstSound; g_sound1 = sound.LastSound; }
CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) {}
void CCombatActionBase::SetAnimationName(const CCharString& name) { g_animSet = &name; strncpy(g_lastText, (const char*)name.PStringData, 63); }
void CCombatActionBase::v00() {}
void CCreatureAction_CastStrengthMultiStrikeProper::v00() {}
int main()
{
    CSpecialAbilitiesMultiStrikeDef def; def.RefCount = 2; for (int i = 0; i < 4; ++i) { def.Animations[i].Index = 100 + i; def.Sounds[i].Index = 0; }
    g_def = &def;
    CThingCreatureBase* owner = (CThingCreatureBase*)0x1234; CThing* target = (CThing*)0x5678;
    {
        CCreatureAction_CastStrengthMultiStrikeProper a(owner, 3);
        CHECK(g_owner == owner && g_target == 0 && g_code == 150 && g_sound0 == 0 && g_sound1 == 0);
        CHECK(a.SpellLevel == 3); CHECK(g_column == 1); CHECK(g_animAsked == 102); CHECK(strcmp(g_lastText, "ANIM") == 0);
        CHECK(g_strCtor == 2 && g_strDtor == 2); CHECK(def.RefCount == 1); CHECK(g_released == 0);
    }
    {
        CCreatureAction_CastStrengthMultiStrikeProper a(owner, 1);
        CHECK(g_animAsked == 100); CHECK(def.RefCount == 0); CHECK(g_released == 1);
    }
    if (fails == 0) printf("008e6c90_TEST PASS\n"); else printf("008e6c90_TEST FAIL\n");
    return fails;
}