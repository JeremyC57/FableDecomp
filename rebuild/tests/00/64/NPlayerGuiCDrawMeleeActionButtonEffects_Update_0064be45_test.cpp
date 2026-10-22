#include <stdio.h>
#include <string.h>
// NPlayerGui::CDrawMeleeActionButtonEffects::Update() -- pick the hero's current melee ability def (the flourish ability when a flourish is valid, else the base ability when the combat component says so), restart the flash timers when it changes, clear the effects when there is none, and count the timers down
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064BE45, length 373 bytes (frame pointer, `ret`).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetControlledCreature @ 0x00487DD0, IS_THING_ALIVE @ 0x0043DBF0
// (`__fastcall`, by reference), std::bitset::test @ 0x00410DE0, the interface map LowerBound const @ 0x004365B0, CTCCombat::
// IsMeleeFlourishValid @ 0x00733EE0, ::GetPMeleeFlourishAbility @ 0x00733480 (CDefPointer by hidden pointer), CTCCombat @ 0x00733290
// (a bool query; the manifest label "CThingFilter_IsUnconscious::operator()" is propagated), the base-ability getter @ 0x007334B0
// (thiscall on the CTCCombat component, CDefPointer by hidden pointer), ClearEffects @ 0x006445BA (thiscall on the Effects vector), the def's virtual slot 1 on release.
// Layout: Ego_r.pdb CDrawMeleeActionButtonEffects minus 4 (the 16-byte PDB vector is 12 in retail): PlayerManager +0x08, Effects +0x28,
// FlashTimer +0x34, ComboLoadingIconTimer +0x38, FlourishCounter +0x3c, EffectCount +0x40, PCurrentAbility +0x44 (names of the last two
// descriptive). CTCCombat = interface 0x30.
// Shape: frame-pointer size TU; IS_THING_ALIVE (out of line in retail) has a same-TU read-only stand-in body so the hero's frame word
// does not escape and is reused for the lookup key and the second temporary; the component comes from the out-param GetInterface
// (the type constant 0x30 is held in ebx); both getters are called on the CTCCombat component; the chosen def is a CDefPointer local
// filled from each getter's full-expression temporary through an AddRef copy; CDefPointer release and assignment clear the pointer
// only inside the non-null arm (`if (p) { p->Release(); p = 0; }`).
struct CTCBase;
struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
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
struct CRefCounted {
    long m_count;
    void AddRef() { ++m_count; }
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
struct CAbilityDef : CDefVirtual, CRefCounted {
    __forceinline void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    void Adopt(const CDefPointer& other)
    {
        T* object = other.Object;
        if (object) {
            object->AddRef();
            Object = object;
        }
    }
    __forceinline void Assign(T* object)
    {
        if (Object != object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = object;
            if (object)
                object->AddRef();
        }
    }
};
struct CThing {
    unsigned char _pad0[0x20];
    mutable CPresenceBits m_present;               // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long type) const
    {
        if (m_present.test(type)) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
bool IsThingAliveImpl(const void* thing);          // stand-in callee of the stand-in body below
#pragma auto_inline(off)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && IsThingAliveImpl(thing);
}
#pragma auto_inline(on)
struct CPlayer {
    CThing* GetControlledCreature();               // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
};
struct CTCCombat {
    bool IsMeleeFlourishValid();                   // 0x00733EE0
    CDefPointer<CAbilityDef> GetPMeleeFlourishAbility();   // 0x00733480
    bool IsShowingBaseAbility();                   // 0x00733290 (label unverified)
    CDefPointer<CAbilityDef> GetPBaseAbility();    // 0x007334B0
};
struct CEffectVector {
    void ClearEffects();                           // 0x006445BA
};
struct CDrawMeleeActionButtonEffects {
    unsigned char            _pad0[0x08];
    CPlayerManager*          PlayerManager;        // +0x08
    unsigned char            _pad1[0x28 - 0x0c];
    CEffectVector            Effects;              // +0x28
    unsigned char            _pad2[0x34 - 0x28 - sizeof(CEffectVector)];
    long                     FlashTimer;           // +0x34
    long                     ComboLoadingIconTimer;// +0x38
    long                     FlourishCounter;      // +0x3c
    long                     EffectCount;          // +0x40
    CDefPointer<CAbilityDef> PCurrentAbility;      // +0x44
    void Update();
};

void CDrawMeleeActionButtonEffects::Update()
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetControlledCreature();
    CDefPointer<CAbilityDef> ability;
    if (IS_THING_ALIVE(hero)) {
        CTCCombat* combat;
        if (hero->GetInterface(&combat, 0x30)) {
            if (combat->IsMeleeFlourishValid()) {
                ability.Adopt(combat->GetPMeleeFlourishAbility());
            }
            else if (combat->IsShowingBaseAbility()) {
                ability.Adopt(combat->GetPBaseAbility());
            }
            if (ability.Object != PCurrentAbility.Object) {
                ComboLoadingIconTimer = -1;
                FlashTimer = 15;
            }
        }
    }
    PCurrentAbility.Assign(ability.Object);
    if (FlashTimer == 0)
        ComboLoadingIconTimer = 30;
    if (!ability.Object) {
        Effects.ClearEffects();
        FlourishCounter = -1;
        EffectCount = 0;
        ComboLoadingIconTimer = -1;
        FlashTimer = -1;
    }
    if (ComboLoadingIconTimer > 0)
        --ComboLoadingIconTimer;
    if (FlashTimer > 0)
        --FlashTimer;
}

// ---- stubs ----
static int g_hasCombat, g_alive, g_flourish, g_showBase, g_clears, g_destroyed;
static CAbilityDef* g_flourishDef;
static CAbilityDef* g_baseDef;
static CThing* g_hero;
static CTCCombat g_combat;
static CPlayer g_player;
bool CPresenceBits::test(long bit) { return g_hasCombat && bit == 0x30; }
bool IsThingAliveImpl(const void* thing) { return g_alive != 0; }
CThing* CPlayer::GetControlledCreature() { return g_hero; }
CPlayer* CPlayerManager::GetMainPlayer() { return &g_player; }
bool CTCCombat::IsMeleeFlourishValid() { return g_flourish != 0; }
bool CTCCombat::IsShowingBaseAbility() { return g_showBase != 0; }
static CDefPointer<CAbilityDef> MakeDef(CAbilityDef* def)
{
    CDefPointer<CAbilityDef> r;
    r.Object = def;
    if (def) { def->AddRef(); def->AddRef(); }   // no NRVO in VC7.1: the local copy is released on return
    return r;
}
CDefPointer<CAbilityDef> CTCCombat::GetPMeleeFlourishAbility() { return MakeDef(g_flourishDef); }
CDefPointer<CAbilityDef> CTCCombat::GetPBaseAbility() { return MakeDef(g_baseDef); }
void CEffectVector::ClearEffects() { ++g_clears; }
void CDefVirtual::v00() {}
void CDefVirtual::Destroy() { ++g_destroyed; }

static int g_fail;
static void check(int cond, const char* what) { if (!cond) { printf("FAIL %s\n", what); g_fail = 1; } }

struct SObj {
    CDrawMeleeActionButtonEffects d;
    CPlayerManager pm;
};
static SObj o;
static CThing hero;
static CInterfaceMap::SEntry entries[2];

static void reset(long flash, long combo, CAbilityDef* current)
{
    memset(&o, 0, sizeof(o));
    o.d.PlayerManager = &o.pm;
    o.d.FlashTimer = flash;
    o.d.ComboLoadingIconTimer = combo;
    o.d.FlourishCounter = 5;
    o.d.EffectCount = 7;
    o.d.PCurrentAbility.Object = current;
    g_clears = 0; g_destroyed = 0;
}

int main()
{
    CAbilityDef a, b, c;
    a.m_count = 1; b.m_count = 1; c.m_count = 1;
    memset(&hero, 0, sizeof(hero));
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x30; entries[1].component = (CTCBase*)&g_combat;
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 2;

    // 1. no hero: ability null -> current released (destroyed at 0), effects cleared, timers reset to -1
    g_hero = 0; g_alive = 1; g_hasCombat = 1; g_flourish = 1; g_flourishDef = &a;
    c.m_count = 1; reset(5, 4, &c); c.m_count = 1;
    o.d.Update();
    check(o.d.PCurrentAbility.Object == 0, "1 current");
    check(g_destroyed == 1 && c.m_count == 0, "1 release");
    check(g_clears == 1 && o.d.FlourishCounter == -1 && o.d.EffectCount == 0, "1 clear");
    check(o.d.ComboLoadingIconTimer == -1 && o.d.FlashTimer == -1, "1 timers");
    o.d.PCurrentAbility.Object = 0;

    // 2. hero dead
    g_hero = &hero; g_alive = 0; reset(3, 2, 0);
    o.d.Update();
    check(g_clears == 1 && o.d.FlashTimer == -1 && o.d.ComboLoadingIconTimer == -1, "2 dead");

    // 3. alive, no combat component
    g_alive = 1; g_hasCombat = 0; reset(0, 2, 0);
    o.d.Update();
    check(g_clears == 1 && o.d.PCurrentAbility.Object == 0, "3 no combat");

    // 4. flourish valid, new def a (current null): timers restart, current = a with a reference
    g_hasCombat = 1; g_flourish = 1; g_flourishDef = &a; a.m_count = 1; reset(0, 9, 0);
    o.d.Update();
    check(o.d.PCurrentAbility.Object == &a && a.m_count == 2, "4 assign");
    check(o.d.FlashTimer == 14 && o.d.ComboLoadingIconTimer == -1, "4 timers");
    check(g_clears == 0 && o.d.FlourishCounter == 5 && o.d.EffectCount == 7, "4 no clear");

    // 5. flourish valid, same def as current: no restart; FlashTimer 0 -> combo 30 -> 29
    a.m_count = 2; reset(0, 9, &a);
    o.d.Update();
    check(o.d.PCurrentAbility.Object == &a && a.m_count == 2, "5 same");
    check(o.d.FlashTimer == 0 && o.d.ComboLoadingIconTimer == 29, "5 timers");
    o.d.PCurrentAbility.Object = 0;

    // 6. flourish invalid, base shown: def b replaces a (a released but alive)
    g_flourish = 0; g_showBase = 1; g_baseDef = &b; b.m_count = 1; a.m_count = 2; reset(4, 0, &a);
    o.d.Update();
    check(o.d.PCurrentAbility.Object == &b && b.m_count == 2 && a.m_count == 1, "6 base");
    check(o.d.FlashTimer == 14 && o.d.ComboLoadingIconTimer == -1 && g_destroyed == 0, "6 timers");
    o.d.PCurrentAbility.Object = 0;

    // 7. flourish invalid, base not shown: null ability
    g_showBase = 0; reset(6, 6, 0);
    o.d.Update();
    check(g_clears == 1 && o.d.FlashTimer == -1, "7 none");

    // 8. flourish valid but null def; current b kept? no: assign null releases b
    g_flourish = 1; g_flourishDef = 0; b.m_count = 2; reset(2, 3, &b);
    o.d.Update();
    check(o.d.PCurrentAbility.Object == 0 && b.m_count == 1 && g_clears == 1, "8 null def");

    // 9. base path with a temp that drops to zero is not possible (getter holds a ref); positive timers count down
    g_flourish = 1; g_flourishDef = &a; a.m_count = 2; reset(3, 3, &a);
    o.d.Update();
    check(o.d.FlashTimer == 2 && o.d.ComboLoadingIconTimer == 2 && a.m_count == 2, "9 countdown");
    o.d.PCurrentAbility.Object = 0;

    if (!g_fail) printf("0064be45_TEST PASS\n");
    return g_fail;
}