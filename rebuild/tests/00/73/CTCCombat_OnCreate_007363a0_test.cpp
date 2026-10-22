// Behaviour test for 007363a0: final.cpp copied verbatim with its same-TU stand-in bodies replaced by observable stubs.
// CTCCombat::OnCreate()
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[2048];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim, stand-ins removed) ----
// CTCCombat::OnCreate() -- cache the parent creature; for a creature thing, load the hero combat def's default melee abilities into the ability list (when it has a hero combat def) and look up the block-counter and 360-flourish abilities by name; then fetch the creature's combat type def and, when found, set it and take its starts-ready-for-melee flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007363A0, length 482 bytes.
// Callees: CThingCreatureBase::GetPHeroCombatDef @ 0x00663340 (CDefPointer by value), CTCBase::GetDefinitionManager @ 0x006869D0,
//          CDefinitionManager::GetDef(long, CDefPointer<CMeleeCombatAbilityDef>*) @ 0x00737F80 / GetDef(const CCharString&,
//          CDefPointer&) @ 0x00737FF0 / GetDef(long, CDefPointer<CCombatTypeDef>&) @ 0x00669640 (same-TU out-only stand-ins),
//          vector<CDefPointer>::_M_insert_overflow @ 0x00739BC0 (stand-in), CThingCreatureBase::GetCombatTypeIndex @ 0x00662170,
//          CTCCombat::SetCombatType @ 0x007355F0 (stand-in), CCharString ctor @ 0x0099EBF0 / dtor @ 0x0099EAE0, the defs' virtual slot 1.
// Shape: retry A's v15 (out-only stand-ins for every address-taking callee, by-value GetPHeroCombatDef with its temporary released
//          by hand through the inline dtor) plus a named `long index = *it;` for the loop lookup: the value load then takes eax and
//          the out slot's lea ecx (`mov eax,[ebp] ; lea ecx,[esp+14h] ; push ecx ; push eax`), where `GetDef(*it, &ability)` colours
//          them the other way round. A by-reference out parameter does not change it; a forceinline (out, key) wrapper also matches.
// Leaf: OnCreate
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
    T* operator->() const { return m_p; }
    bool IsNull() const { return m_p == 0; }
};

inline void* operator new(size_t, void* where) throw() { return where; }   // size_t for the test's host compiler
struct __false_type { };
template <class T> struct CDefPointerVector {           // std::vector<CDefPointer<T> >
    CDefPointer<T>* _M_start;
    CDefPointer<T>* _M_finish;
    CDefPointer<T>* _M_end_of_storage;
    void _M_insert_overflow(CDefPointer<T>* pos, const CDefPointer<T>& x, const __false_type& pod, unsigned int fillLen,
                            bool atEnd);                // 0x00739BC0
    void push_back(const CDefPointer<T>& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CDefPointer<T>(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    const long* begin() const { return _M_start; }
    const long* end() const { return _M_finish; }
};
struct CHeroCombatDef : CDefBase {
    unsigned char _pad08[0x11c - 0x08];
    CLongVector   DefaultCombatAbilities;               // +0x11c
};
struct CCombatTypeDef : CDefBase {
    unsigned char _pad08[0xa8 - 0x08];
    bool          StartsAsReadyForMelee;                // +0xa8
};
struct CMeleeCombatAbilityDef : CDefBase { };

struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CMeleeCombatAbilityDef>* out) const;     // 0x00737F80
    bool GetDef(const CCharString& name, CDefPointer<CMeleeCombatAbilityDef>& out);   // 0x00737FF0
    bool GetDef(long index, CDefPointer<CCombatTypeDef>& out);                   // 0x00669640
};



struct CThing {
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                           // +0x10 (2: creature)
};
struct CThingCreatureBase : CThing {
    long GetCombatTypeIndex() const;                    // 0x00662170
    CDefPointer<CHeroCombatDef> GetPHeroCombatDef() const;  // 0x00663340
};

struct CTCCombat {
    void*               __vftable;                      // +0x00
    CThing*             ParentThing;                    // +0x04 (CTCBase)
    unsigned char       _pad08[0x24 - 0x08];
    CThingCreatureBase* ParentCreature;                 // +0x24
    CDefPointer<CCombatTypeDef> PCombatTypeDef;         // +0x28
    CDefPointerVector<CMeleeCombatAbilityDef> AbilityList;   // +0x2c
    unsigned char       _pad38[0x48 - 0x38];
    CDefPointer<CMeleeCombatAbilityDef> PBlockCounterAbility;   // +0x48
    CDefPointer<CMeleeCombatAbilityDef> PFlourish360Ability;    // +0x4c
    unsigned char       _pad50[0x80 - 0x50];
    bool                ReadyForMelee;                  // +0x80
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    const CThingCreatureBase& GetParentCreature() const { return *ParentCreature; }
    CDefPointer<CHeroCombatDef> GetPHeroCombatDef() const { return GetParentCreature().GetPHeroCombatDef(); }
    void SetCombatType(const CDefPointer<CCombatTypeDef>& def);   // 0x007355F0
    void OnCreate();
};



void CTCCombat::OnCreate()
{
    ParentCreature = (CThingCreatureBase*)ParentThing;
    if (ParentThing->ThingClass == 2) {
        if (!GetPHeroCombatDef().IsNull()) {
            for (const long* it = GetPHeroCombatDef()->DefaultCombatAbilities.begin();
                 it != GetPHeroCombatDef()->DefaultCombatAbilities.end(); ++it) {
                CDefPointer<CMeleeCombatAbilityDef> ability;
                long index = *it;
                GetDefinitionManager()->GetDef(index, &ability);
                AbilityList.push_back(ability);
            }
        }
        GetDefinitionManager()->GetDef("HERO_MELEE_COMBAT_ABILITY_BLOCK_COUNTER", PBlockCounterAbility);
        GetDefinitionManager()->GetDef("HERO_MELEE_COMBAT_ABILITY_FLOURISH_360_STRIKE", PFlourish360Ability);
    }
    CDefPointer<CCombatTypeDef> combatType;
    long combatTypeIndex = GetParentCreature().GetCombatTypeIndex();
    if (GetDefinitionManager()->GetDef(combatTypeIndex, combatType)) {
        SetCombatType(combatType);
        ReadyForMelee = combatType->StartsAsReadyForMelee;
    }
}

// ---- stubs ----
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; CHECK(length == -1); }
CCharString::~CCharString() { PStringData = 0; }
void CDefVirtual::v00() {}
static int g_destroyed = 0;
void CDefVirtual::Destroy() { ++g_destroyed; note("destroy"); }
static CMeleeCombatAbilityDef g_abilities[4];
static CHeroCombatDef g_heroDef; static bool g_haveHero = true;
static long g_defaults[3] = { 1, 2, 3 };
static CCombatTypeDef g_combatType; static bool g_haveCombatType = true;
static CDefinitionManager g_manager;
static long g_combatIndex = 77;
long CThingCreatureBase::GetCombatTypeIndex() const { note("ctindex"); return g_combatIndex; }
CDefPointer<CHeroCombatDef> CThingCreatureBase::GetPHeroCombatDef() const
{
    note("herodef");
    CDefPointer<CHeroCombatDef> p;
    if (g_haveHero) { p.m_p = &g_heroDef; ++g_heroDef.m_count; }
    return p;
}
CDefinitionManager* CTCCombat::GetDefinitionManager() const { note("mgr"); return &g_manager; }
bool CDefinitionManager::GetDef(long index, CDefPointer<CMeleeCombatAbilityDef>* out) const
{
    char b[64]; _snprintf(b, sizeof b, "getdef%ld", index); note(b); CHECK(this == &g_manager);
    if (index < 1 || index > 3) return false;
    out->m_p = &g_abilities[index]; ++g_abilities[index].m_count; return true;
}
bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<CMeleeCombatAbilityDef>& out)
{
    char b[96]; _snprintf(b, sizeof b, "getdef(%s)", (const char*)name.PStringData); note(b);
    out.m_p = &g_abilities[0]; ++g_abilities[0].m_count; return true;
}
bool CDefinitionManager::GetDef(long index, CDefPointer<CCombatTypeDef>& out)
{
    char b[64]; _snprintf(b, sizeof b, "getct%ld", index); note(b);
    if (!g_haveCombatType) return false;
    out.m_p = &g_combatType; ++g_combatType.m_count; return true;
}
template <class T> void CDefPointerVector<T>::_M_insert_overflow(CDefPointer<T>* pos, const CDefPointer<T>& x, const __false_type& pod, unsigned int fillLen, bool atEnd)
{
    note("overflow"); CHECK(pos == _M_finish && fillLen == 1 && atEnd); (void)pod;
    new (_M_finish) CDefPointer<T>(x); ++_M_finish; ++_M_end_of_storage;   // the test array has spare room
}
static const CCombatTypeDef* g_setCombatType = 0;
void CTCCombat::SetCombatType(const CDefPointer<CCombatTypeDef>& def) { note("setct"); g_setCombatType = def.m_p; }

static void reset()
{
    g_log[0] = 0; g_destroyed = 0; g_setCombatType = 0;
    for (int i = 0; i < 4; ++i) g_abilities[i].m_count = 10;
    g_heroDef.m_count = 10; g_combatType.m_count = 10; g_combatType.StartsAsReadyForMelee = true;
    g_heroDef.DefaultCombatAbilities._M_start = g_defaults; g_heroDef.DefaultCombatAbilities._M_finish = g_defaults + 3;
}
static CDefPointer<CMeleeCombatAbilityDef> g_slots[8];
static void init(CTCCombat& c, CThingCreatureBase& thing, unsigned char cls)
{
    memset(&c, 0, sizeof c); memset(&thing, 0, sizeof thing); thing.ThingClass = cls; c.ParentThing = &thing;
    memset(g_slots, 0, sizeof g_slots); c.AbilityList._M_start = g_slots; c.AbilityList._M_finish = g_slots; c.AbilityList._M_end_of_storage = g_slots + 2;
}

int main()
{
    CTCCombat c; CThingCreatureBase thing;
    // 1. creature with a hero combat def: three abilities (the third through the overflow path), two named lookups, combat type set
    reset(); init(c, thing, 2); g_haveHero = true; g_haveCombatType = true;
    c.OnCreate();
    CHECK(c.ParentCreature == &thing);
    CHECKLOG("herodef;herodef;herodef;mgr;getdef1;herodef;mgr;getdef2;herodef;mgr;getdef3;overflow;herodef;mgr;getdef(HERO_MELEE_COMBAT_ABILITY_BLOCK_COUNTER);mgr;getdef(HERO_MELEE_COMBAT_ABILITY_FLOURISH_360_STRIKE);ctindex;mgr;getct77;setct;");
    CHECK(c.AbilityList._M_finish - c.AbilityList._M_start == 3);
    CHECK(g_slots[0].m_p == &g_abilities[1] && g_slots[1].m_p == &g_abilities[2] && g_slots[2].m_p == &g_abilities[3]);
    CHECK(g_abilities[1].m_count == 11 && g_abilities[2].m_count == 11 && g_abilities[3].m_count == 11);   // one reference each, held by the list
    CHECK(g_heroDef.m_count == 10);                                                                           // every by-value temporary released
    CHECK(c.PBlockCounterAbility.m_p == &g_abilities[0] && c.PFlourish360Ability.m_p == &g_abilities[0] && g_abilities[0].m_count == 12);
    CHECK(c.PCombatTypeDef.m_p == 0 && g_setCombatType == &g_combatType && c.ReadyForMelee == true && g_combatType.m_count == 10);
    CHECK(g_destroyed == 0);
    // 2. creature without a hero combat def: no ability loop, named lookups still run; no combat type found
    reset(); init(c, thing, 2); g_haveHero = false; g_haveCombatType = false; g_combatType.StartsAsReadyForMelee = false;
    c.OnCreate();
    CHECKLOG("herodef;mgr;getdef(HERO_MELEE_COMBAT_ABILITY_BLOCK_COUNTER);mgr;getdef(HERO_MELEE_COMBAT_ABILITY_FLOURISH_360_STRIKE);ctindex;mgr;getct77;");
    CHECK(c.AbilityList._M_finish == c.AbilityList._M_start); CHECK(c.ReadyForMelee == false && g_setCombatType == 0);
    // 3. a non-creature thing: only the combat type lookup
    reset(); init(c, thing, 1); g_haveHero = true; g_haveCombatType = true; g_combatType.StartsAsReadyForMelee = false;
    c.OnCreate();
    CHECKLOG("ctindex;mgr;getct77;setct;"); CHECK(c.ReadyForMelee == false && g_setCombatType == &g_combatType);
    CHECK(g_combatType.m_count == 10);
    if (fails == 0) printf("007363a0_TEST PASS\n"); else printf("007363a0_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}