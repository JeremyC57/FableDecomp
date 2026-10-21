// Behaviour test for 00782390: src/00782390.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCBoastingPosition::OnBoastAccepted(CThingCreatureBase& hero)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCBoastingPosition::OnBoastAccepted(CThingCreatureBase& hero) -- the hero's boast at this position was accepted: if the podium, the position and the hero are all live and the hero can pay the cost, tell the podium, burst the orb effect, play the boast animation, switch the position off and make its thing untargetable
// Leaf: OnBoastAccepted
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00782390, 464 bytes (`ret 4`), no EH frame.
//         Ego_r.pdb: `?OnBoastAccepted@CTCBoastingPosition@@QAEXAAVCThingCreatureBase@@@Z`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020
//          and its const copy @ 0x004365B0, CTCBoastingPodium::OnHeroBoast(long, CThingCreatureBase&) @ 0x00813C50,
//          IS_THING_ALIVE<CIntelligentPointer<...>> __fastcall @ 0x004CC340 (out-of-line copy), CThing::Kill(bool) @
//          0x004C9B80, CTCDParticleEmitter::Create(const CCharString&, const C3DVector&, bool) __fastcall @ 0x006E0970,
//          CCharString @ 0x0099EBF0 / 0x0099EAE0 ("PODIUM_ORB_EXPLODE_01" @ 0x0126B2F0, "ST_BOAST" @ 0x0124DCE8),
//          CCreatureAction_PlayAnimation ctor @ 0x008424F0, CThingCreatureBase::SetCurrentAction @ 0x006644F0,
//          ~CCreatureActionBase @ 0x00693EF0.
// Shape: IS_THING_ALIVE is one inline __fastcall template (pointer test, then the dead bit in a nested `if`); VC7.1
//        expands it for the podium pointer and the hero pointer and calls the out-of-line copy for the particle effect
//        (inline-once band). The affordability test is a bool local (`setge al ; test al,al`) set inside the PeekTC arm;
//        the podium component is a zero-initialised out pointer filled by the GetTC(T**) template (`xor edi,edi` first);
//        both literals are named CCharStrings (built before the other arguments), the CSoundPair a temporary.
// Layout: CTCBoastingPosition (Ego_r.pdb, retail offsets equal): ParentThing +4, PBoastingPodium +0x0c, OrbPosition +0x14,
//         PositionActivated +0x20, BoastIndex +0x24, Cost +0x28, BoastOn +0x2c, HeroInsideRadius +0x2d, PParticleEffect
//         +0x2e (CBaseIntelligentPointer is byte-packed). Retail CThing: interface bitset +0x20, CVectorMap +0x44, dead flag
//         +0x91 bit 0. CTCHeroStats (interface 4) Money retail +0x3c (PDB +0x44: map and vector 4 bytes shorter);
//         CTCBoastingPodium = interface 0x70; CTCTargeted (interface 9) Targetable +0x10.
class CTCBase;
class CBaseObject;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct C3DVector { float X, Y, Z; };

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*>: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];               // +0x20
    CInterfaceMap Components;                       // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    void Kill(bool removeFromWorld);                // 0x004C9B80
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (T*)it->component;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (const T*)it->component;
        return true;
    }
};

class CCreatureActionBase {
public:
    unsigned char _pad[0xb4];                       // retail CCreatureAction_PlayAnimation is 0xb4 bytes
    ~CCreatureActionBase();                         // 0x00693EF0
};

class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCreatureAction_PlayAnimation : public CCreatureActionBase {
public:
    CCreatureAction_PlayAnimation(CThingCreatureBase& creature, const CCharString& animation, const CSoundPair& sound,
                                  bool b1, bool b2, bool b3, long l1, long l2);   // 0x008424F0
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                           // +0x04
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
#pragma pack(pop)
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    operator T*() const { return (T*)GetPItem(); }
    T* operator->() const { return (T*)GetPItem(); }
};

template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (thing) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCHeroStats {                                // interface 4
public:
    enum { INTERFACE_TYPE = 4 };
    unsigned char _pad00[0x3c];
    long Money;                                     // +0x3c
};
class CTCTargeted {                                 // interface 9
public:
    enum { INTERFACE_TYPE = 9 };
    unsigned char _pad00[0x10];
    bool Targetable;                                // +0x10
};
class CTCBoastingPodium {                           // interface 0x70
public:
    enum { INTERFACE_TYPE = 0x70 };
    void OnHeroBoast(long boastIndex, CThingCreatureBase& hero);   // 0x00813C50
};
class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(const CCharString& name, const C3DVector& position, bool b);   // 0x006E0970
};

class CTCBoastingPosition {
public:
    void* __vftable;                                // +0x00 (CTCBase)
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    unsigned char _pad0a[2];
    CIntelligentPointer<CThing> PBoastingPodium;    // +0x0c
    C3DVector OrbPosition;                          // +0x14
    bool PositionActivated;                         // +0x20
    long BoastIndex;                                // +0x24
    long Cost;                                      // +0x28
    bool BoastOn;                                   // +0x2c
    bool HeroInsideRadius;                          // +0x2d
    CIntelligentPointer<CThing> PParticleEffect;    // +0x2e
    void OnBoastAccepted(CThingCreatureBase& hero);
};

void CTCBoastingPosition::OnBoastAccepted(CThingCreatureBase& hero)
{
    if (!IS_THING_ALIVE(PBoastingPodium) || !PositionActivated || !HeroInsideRadius || !BoastOn)
        return;
    CThingCreatureBase* pHero = &hero;
    if (!IS_THING_ALIVE(pHero))
        return;
    const CTCHeroStats* stats;
    bool canAfford = true;
    if (hero.PeekTC(&stats))
        canAfford = stats->Money >= Cost;
    if (!canAfford)
        return;
    CTCBoastingPodium* podium = 0;
    PBoastingPodium->GetTC(&podium);
    podium->OnHeroBoast(BoastIndex, hero);
    if (IS_THING_ALIVE(PParticleEffect)) {
        PParticleEffect->Kill(true);
        CCharString name("PODIUM_ORB_EXPLODE_01", -1);
        CTCDParticleEmitter::Create(name, OrbPosition, false);
    }
    {
        CCharString animation("ST_BOAST", -1);
        hero.SetCurrentAction(CCreatureAction_PlayAnimation(hero, animation, CSoundPair(), false, true, false, 0, 0));
    }
    BoastOn = false;
    CThing* parent = ParentThing;
    CTCTargeted* targeted;
    if (parent->GetTC(&targeted))
        targeted->Targetable = false;
}

// ---- stubs and driver ----
// stubs: record every observable call
static int g_strCtor = 0, g_strDtor = 0, g_kills = 0, g_killArgTrue = 0, g_boasts = 0, g_creates = 0, g_actions = 0;
static int g_actionDtor = 0, g_setAction = 0;
static CThing* g_killed = 0; static CTCBoastingPodium* g_boastPodium = 0; static long g_boastIndex = -1;
static CThingCreatureBase* g_boastHero = 0; static const char* g_createName = 0; static const C3DVector* g_createPos = 0;
static bool g_createFlag = true; static const char* g_animName = 0; static bool g_actionArgsOk = false;
static const CCreatureActionBase* g_builtAction = 0; static const CCreatureActionBase* g_setArg = 0; static CThingCreatureBase* g_setOn = 0;
CCharString::CCharString(const char* text, long length) { ++g_strCtor; PStringData = (void*)text; CHECK(length == -1); }
CCharString::~CCharString() { ++g_strDtor; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{ const SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
void CThing::Kill(bool removeFromWorld) { ++g_kills; g_killed = this; if (removeFromWorld) ++g_killArgTrue; }
CCreatureActionBase::~CCreatureActionBase() { ++g_actionDtor; CHECK(this == g_builtAction); }
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& action) { ++g_setAction; g_setArg = &action; g_setOn = this; return true; }
CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase& creature, const CCharString& animation,
    const CSoundPair& sound, bool b1, bool b2, bool b3, long l1, long l2)
{
    ++g_actions; g_builtAction = this; g_animName = (const char*)animation.PStringData;
    g_actionArgsOk = &creature == g_boastHero && sound.m_first == 0 && sound.m_second == 0 && !b1 && b2 && !b3 && l1 == 0 && l2 == 0;
}
void CBaseIntelligentPointer::v00() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CTCBoastingPodium::OnHeroBoast(long boastIndex, CThingCreatureBase& hero) { ++g_boasts; g_boastPodium = this; g_boastIndex = boastIndex; g_boastHero = &hero; }
CThing* __fastcall CTCDParticleEmitter::Create(const CCharString& name, const C3DVector& position, bool b)
{ ++g_creates; g_createName = (const char*)name.PStringData; g_createPos = &position; g_createFlag = b; return 0; }

static CThingCreatureBase g_hero;
static CThing g_podiumThing, g_parent, g_effect;
static CTCHeroStats g_stats; static CTCTargeted g_targeted; static unsigned char g_podiumComp[4];
static CInterfaceMap::SEntry g_heroMap[1], g_podiumMap[2], g_parentMap[1];
static void Reset()
{
    g_strCtor = g_strDtor = g_kills = g_killArgTrue = g_boasts = g_creates = g_actions = g_actionDtor = g_setAction = 0;
    g_killed = 0; g_boastPodium = 0; g_boastIndex = -1; g_boastHero = 0; g_createName = 0; g_createPos = 0; g_createFlag = true;
    g_animName = 0; g_actionArgsOk = false; g_setArg = 0; g_setOn = 0; g_targeted.Targetable = true;
}
static void Setup(CTCBoastingPosition& p)
{
    p.ParentThing = &g_parent; p.PBoastingPodium.m_pItem = (CBaseObject*)&g_podiumThing; p.PParticleEffect.m_pItem = (CBaseObject*)&g_effect;
    p.PositionActivated = true; p.HeroInsideRadius = true; p.BoastOn = true; p.BoastIndex = 3; p.Cost = 50;
    g_podiumThing.m_flags91 = 0; g_hero.m_flags91 = 0; g_effect.m_flags91 = 0; g_stats.Money = 50;
    g_hero.UsingComponents[0] = 1UL << 4; g_parent.UsingComponents[0] = 1UL << 9; g_podiumThing.UsingComponents[3] = 1UL << 16;
}
static bool Untouched(const CTCBoastingPosition& p) { return g_boasts == 0 && g_actions == 0 && g_kills == 0 && g_creates == 0 && p.BoastOn && g_targeted.Targetable; }
int main()
{
    static CTCBoastingPosition p;
    g_heroMap[0].type = 4; g_heroMap[0].component = (CTCBase*)&g_stats;
    g_hero.Components.first = g_heroMap; g_hero.Components.last = g_heroMap + 1;
    g_podiumMap[0].type = 0x10; g_podiumMap[1].type = 0x70; g_podiumMap[1].component = (CTCBase*)g_podiumComp;
    g_podiumThing.Components.first = g_podiumMap; g_podiumThing.Components.last = g_podiumMap + 2;
    g_parentMap[0].type = 9; g_parentMap[0].component = (CTCBase*)&g_targeted;
    g_parent.Components.first = g_parentMap; g_parent.Components.last = g_parentMap + 1;

    // full accept: podium told, orb burst, boast animation, position off, parent untargetable
    Setup(p); Reset();
    p.OnBoastAccepted(g_hero);
    CHECK(g_boasts == 1 && g_boastPodium == (CTCBoastingPodium*)g_podiumComp && g_boastIndex == 3 && g_boastHero == &g_hero);
    CHECK(g_kills == 1 && g_killed == &g_effect && g_killArgTrue == 1);
    CHECK(g_creates == 1 && g_createName && strcmp(g_createName, "PODIUM_ORB_EXPLODE_01") == 0 && g_createPos == &p.OrbPosition && !g_createFlag);
    CHECK(g_actions == 1 && g_actionArgsOk && g_animName && strcmp(g_animName, "ST_BOAST") == 0);
    CHECK(g_setAction == 1 && g_setArg == g_builtAction && g_setOn == &g_hero && g_actionDtor == 1);
    CHECK(g_strCtor == 2 && g_strDtor == 2 && !p.BoastOn && !g_targeted.Targetable);

    // dead or missing particle effect: no burst; parent without TC 9 keeps its flag
    Setup(p); Reset(); g_effect.m_flags91 = 1; g_parent.UsingComponents[0] = 0;
    p.OnBoastAccepted(g_hero);
    CHECK(g_boasts == 1 && g_kills == 0 && g_creates == 0 && g_actions == 1 && g_strCtor == 1 && !p.BoastOn && g_targeted.Targetable);
    Setup(p); Reset(); p.PParticleEffect.m_pItem = 0;
    p.OnBoastAccepted(g_hero);
    CHECK(g_boasts == 1 && g_kills == 0 && g_creates == 0 && g_actions == 1);

    // hero without stats always affords; too poor a hero does nothing
    Setup(p); Reset(); g_hero.UsingComponents[0] = 0; g_stats.Money = 0;
    p.OnBoastAccepted(g_hero); CHECK(g_boasts == 1 && g_actions == 1);
    Setup(p); Reset(); g_stats.Money = 49;
    p.OnBoastAccepted(g_hero); CHECK(Untouched(p));

    // any failed precondition does nothing
    Setup(p); Reset(); p.PBoastingPodium.m_pItem = 0; p.OnBoastAccepted(g_hero); CHECK(Untouched(p));
    Setup(p); Reset(); g_podiumThing.m_flags91 = 1; p.OnBoastAccepted(g_hero); CHECK(Untouched(p));
    Setup(p); Reset(); p.PositionActivated = false; p.OnBoastAccepted(g_hero); CHECK(Untouched(p));
    Setup(p); Reset(); p.HeroInsideRadius = false; p.OnBoastAccepted(g_hero); CHECK(Untouched(p));
    Setup(p); Reset(); p.BoastOn = false; p.OnBoastAccepted(g_hero); CHECK(g_boasts == 0 && g_actions == 0 && g_targeted.Targetable);
    Setup(p); Reset(); g_hero.m_flags91 = 1; p.OnBoastAccepted(g_hero); CHECK(Untouched(p));
    if (fails == 0) printf("00782390_TEST PASS\n");
    return fails;
}
