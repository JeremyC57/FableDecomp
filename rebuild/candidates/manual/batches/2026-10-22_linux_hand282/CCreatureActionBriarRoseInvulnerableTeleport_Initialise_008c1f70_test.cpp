// Behaviour test for 0x008C1F70 CCreatureAction_BriarRoseInvulnerableTeleport::Initialise: runs the reconstructed body
// over stub things, components and defs; checks the animation name set before the base Initialise, the teleport-intangible
// mode pushed only for a living enemy (owner = the creature, enemy = PEnemy), the TELEPORT_POP effect at the creature's
// position, the TELEPORT_AWAY_WANK sound played with the default CSoundInit, the def-class reference counting (balanced),
// the sub-def releases (Destroy on the last reference), and the missing-component / missing-def cases.
#include <cstdio>
#include <cstring>

#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif

static int g_failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++g_failures; } } while (0)

struct CTCBase { };
struct C3DVector { float X, Y, Z; };
struct CCharString {
    char Text[64];
    CCharString(const char* text, long length = -1) { (void)length; std::strncpy(Text, text, 63); Text[63] = 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry  entries[8];
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type)
            ++it;
        return it;
    }
};
struct CDefBase {
    int  Destroyed;
    long RefCount;
    CDefBase() : Destroyed(0), RefCount(1) { }
    virtual ~CDefBase() { }
    virtual void Destroy() { ++Destroyed; }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CSpecialEffectsDef : CDefBase {
    long PopEffect;                         // -1: no TELEPORT_POP entry
    bool CalcAndLookup(const CCharString& name, long* value)
    {
        if (std::strcmp(name.Text, "TELEPORT_POP") != 0 || PopEffect < 0)
            return false;
        *value = PopEffect;
        return true;
    }
};
struct CSoundMap {
    long WankSound;
    bool HasWank;
    const long* GetPSound(const CCharString& name) const
    {
        if (!HasWank || std::strcmp(name.Text, "TELEPORT_AWAY_WANK") != 0)
            return 0;
        return &WankSound;
    }
};
struct CEntitySoundDef : CDefBase {
    CSoundMap SoundMap;
};
struct CDefClassBase : CDefBase {
    CSpecialEffectsDef* Effects;
    CEntitySoundDef*    Sounds;
    int                 MaxRefSeen;
    CDefClassBase() : Effects(0), Sounds(0), MaxRefSeen(0) { }
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
    {
        if (RefCount > MaxRefSeen) MaxRefSeen = RefCount;
        if (!Effects) return false;
        ++Effects->RefCount;                // the out pointer owns one reference
        out->Object = Effects;
        return true;
    }
    bool GetSubDef(CDefPointer<CEntitySoundDef>* out)
    {
        if (RefCount > MaxRefSeen) MaxRefSeen = RefCount;
        if (!Sounds) return false;
        ++Sounds->RefCount;
        out->Object = Sounds;
        return true;
    }
};
struct CDefClassReference {
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->RefCount; }
    ~CDefClassReference() { if (m_p) { if (--m_p->RefCount == 0) m_p->Destroy(); } }
    CDefClassBase* operator->() const { return m_p; }
};
class CSoundInit {
public:
    float Volume, Pitch, Pan;
    long NoPlays, PlaybackType;
    unsigned long GainFaderBits, PitchFaderBits;
    long OverrideID;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), GainFaderBits(0), PitchFaderBits(0), OverrideID(-1)
    {
        PlaybackType = 1;
    }
};
struct CTCSoundPlayer : CTCBase {
    int        Calls;
    long       Sound;
    CSoundInit Init;
    long       A, B;
    CTCSoundPlayer() : Calls(0), Sound(0), A(-7), B(-7) { }
    unsigned long PlaySound(long sound, const CSoundInit& init, long a, long b)
    {
        ++Calls; Sound = sound; Init = init; A = a; B = b;
        return 0;
    }
};
struct CThing {
    unsigned long  m_presentBits[9];
    CInterfaceMap  m_interfaces;
    CDefClassBase* m_pDef;
    unsigned char  m_flags91;
    C3DVector      Position;
    CThing() : m_pDef(0), m_flags91(0)
    {
        std::memset(m_presentBits, 0, sizeof(m_presentBits));
        m_interfaces.first = m_interfaces.last = m_interfaces.entries;
        Position.X = 1.0f; Position.Y = 2.0f; Position.Z = 3.0f;
    }
    void AddInterface(long type, CTCBase* component)    // keep entries sorted by type
    {
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        CInterfaceMap::SEntry* it = m_interfaces.last++;
        while (it != m_interfaces.first && (it - 1)->type > type) { *it = *(it - 1); --it; }
        it->type = type;
        it->component = component;
    }
    const C3DVector& GetPosition() const { return Position; }
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
struct CThingCreatureBase : CThing { };

static int       g_effectCalls = 0;
static long      g_effect = 0;
static C3DVector g_effectPos;
static bool      g_effectAttached = true;
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached)
{
    ++g_effectCalls; g_effect = effect; g_effectPos = position; g_effectAttached = attached;
    return 0;
}

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    operator CThing*() const { return GetPItem(); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
    CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem(); }
};
struct CModeBase { virtual ~CModeBase() { } };
template <class T> struct CCountedPointer {
    T* m_pObject;
    CCountedPointer(T* object) : m_pObject(object) { }
};
struct CModeBriarRoseTeleportIntangible : CModeBase {
    CThing*             Owner;
    CThingCreatureBase* Enemy;
    CModeBriarRoseTeleportIntangible(CThing* owner, CThingCreatureBase* enemy) : Owner(owner), Enemy(enemy) { }
};
struct CTCCreatureModeManager : CTCBase {
    int        Calls;
    CModeBase* Last;
    CTCCreatureModeManager() : Calls(0), Last(0) { }
    ~CTCCreatureModeManager() { delete Last; }
    bool AddMode(CCountedPointer<CModeBase> mode) { ++Calls; delete Last; Last = mode.m_pObject; return true; }
};

static int  g_order = 0;
static int  g_animOrder = 0, g_initOrder = 0;
static char g_anim[64];
class CCombatActionBase {
public:
    CBaseIntelligentPointer PCreature;
    virtual ~CCombatActionBase() { }
    virtual void Initialise() { g_initOrder = ++g_order; }
    void SetAnimationName(const CCharString& name) { g_animOrder = ++g_order; std::strcpy(g_anim, name.Text); }
};
class CCreatureAction_BriarRoseInvulnerableTeleport : public CCombatActionBase {
public:
    CIntelligentPointerCreature PEnemy;
    virtual void Initialise();
};

// ---- reconstructed body (src/008c1f70.cpp) ----
void CCreatureAction_BriarRoseInvulnerableTeleport::Initialise()
{
    CTCSoundPlayer* soundPlayer;
    {
        CCharString name("INVULNERABLE_OUT_OF");
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31) && PEnemy.GetPItem() && !(PEnemy.GetPItem()->m_flags91 & 1))
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeBriarRoseTeleportIntangible(PCreature, PEnemy.GetPItem())));
    CDefPointer<CSpecialEffectsDef> effects;
    long effect = 0;
    if (CDefClassReference(PCreature.GetPItem()->m_pDef)->GetSubDef(&effects) && effects->CalcAndLookup("TELEPORT_POP", &effect))
        CreateSpecialEffect(effect, PCreature.GetPItem()->GetPosition(), false);
    CDefPointer<CEntitySoundDef> sounds;
    if (PCreature.GetPItem()->GetInterface(&soundPlayer, 0x10) && CDefClassReference(PCreature.GetPItem()->m_pDef)->GetSubDef(&sounds)) {
        const long* sound = sounds->SoundMap.GetPSound("TELEPORT_AWAY_WANK");
        if (sound)
            soundPlayer->PlaySound(*sound, CSoundInit(), 0, 0);
    }
}
// ---- end body ----

static void Reset()
{
    g_order = g_animOrder = g_initOrder = 0;
    g_anim[0] = 0;
    g_effectCalls = 0; g_effect = 0; g_effectAttached = true;
}

int main()
{
    // 1: everything present, living enemy.
    {
        Reset();
        CThingCreatureBase creature, enemy;
        CTCCreatureModeManager modes;
        CTCSoundPlayer player;
        CDefClassBase defClass;
        CSpecialEffectsDef fx; fx.PopEffect = 42;
        CEntitySoundDef snd; snd.SoundMap.HasWank = true; snd.SoundMap.WankSound = 77;
        defClass.Effects = &fx; defClass.Sounds = &snd;
        fx.RefCount = 1; snd.RefCount = 1;
        creature.m_pDef = &defClass;
        creature.AddInterface(0x31, &modes);
        creature.AddInterface(0x10, &player);
        creature.Position.X = 5.0f; creature.Position.Y = 6.0f; creature.Position.Z = 7.0f;
        CCreatureAction_BriarRoseInvulnerableTeleport action;
        action.PCreature.m_pItem = &creature;
        action.PEnemy.m_pItem = &enemy;
        action.Initialise();
        CHECK(std::strcmp(g_anim, "INVULNERABLE_OUT_OF") == 0);
        CHECK(g_animOrder == 1 && g_initOrder == 2);
        CHECK(modes.Calls == 1);
        CModeBriarRoseTeleportIntangible* mode = (CModeBriarRoseTeleportIntangible*)modes.Last;
        CHECK(mode && mode->Owner == &creature && mode->Enemy == &enemy);
        CHECK(g_effectCalls == 1 && g_effect == 42 && !g_effectAttached);
        CHECK(g_effectPos.X == 5.0f && g_effectPos.Y == 6.0f && g_effectPos.Z == 7.0f);
        CHECK(player.Calls == 1 && player.Sound == 77 && player.A == 0 && player.B == 0);
        CHECK(player.Init.Volume == 1.0f && player.Init.Pitch == 1.0f && player.Init.Pan == 0.0f);
        CHECK(player.Init.NoPlays == 1 && player.Init.PlaybackType == 1 && player.Init.GainFaderBits == 0);
        CHECK(player.Init.PitchFaderBits == 0 && player.Init.OverrideID == -1);
        CHECK(defClass.RefCount == 1 && defClass.MaxRefSeen == 2 && defClass.Destroyed == 0);
        CHECK(fx.RefCount == 1 && fx.Destroyed == 0 && snd.RefCount == 1 && snd.Destroyed == 0);
    }
    // 2: dead enemy, sub-defs only referenced by the lookups (released to zero -> Destroy), no TELEPORT_POP effect.
    {
        Reset();
        CThingCreatureBase creature, enemy;
        enemy.m_flags91 = 1;
        CTCCreatureModeManager modes;
        CTCSoundPlayer player;
        CDefClassBase defClass;
        CSpecialEffectsDef fx; fx.PopEffect = -1; fx.RefCount = 0;
        CEntitySoundDef snd; snd.SoundMap.HasWank = false; snd.RefCount = 0;
        defClass.Effects = &fx; defClass.Sounds = &snd;
        creature.m_pDef = &defClass;
        creature.AddInterface(0x10, &player);
        creature.AddInterface(0x31, &modes);
        CCreatureAction_BriarRoseInvulnerableTeleport action;
        action.PCreature.m_pItem = &creature;
        action.PEnemy.m_pItem = &enemy;
        action.Initialise();
        CHECK(modes.Calls == 0);
        CHECK(g_effectCalls == 0);
        CHECK(player.Calls == 0);
        CHECK(fx.Destroyed == 1 && snd.Destroyed == 1);
        CHECK(defClass.RefCount == 1 && defClass.Destroyed == 0);
    }
    // 3: no enemy, no mode manager, no sound player; effect def present.
    {
        Reset();
        CThingCreatureBase creature;
        CDefClassBase defClass;
        CSpecialEffectsDef fx; fx.PopEffect = 9; fx.RefCount = 1;
        CEntitySoundDef snd; snd.SoundMap.HasWank = true; snd.SoundMap.WankSound = 3; snd.RefCount = 1;
        defClass.Effects = &fx; defClass.Sounds = &snd;
        creature.m_pDef = &defClass;
        CCreatureAction_BriarRoseInvulnerableTeleport action;
        action.PCreature.m_pItem = &creature;
        action.PEnemy.m_pItem = 0;
        action.Initialise();
        CHECK(g_animOrder == 1 && g_initOrder == 2);
        CHECK(g_effectCalls == 1 && g_effect == 9);
        CHECK(snd.RefCount == 1 && snd.Destroyed == 0 && fx.RefCount == 1);
        CHECK(defClass.RefCount == 1);
    }
    // 4: mode manager but no enemy; no sub-defs at all; sound player present but no sound def.
    {
        Reset();
        CThingCreatureBase creature;
        CTCCreatureModeManager modes;
        CTCSoundPlayer player;
        CDefClassBase defClass;
        creature.m_pDef = &defClass;
        creature.AddInterface(0x31, &modes);
        creature.AddInterface(0x10, &player);
        CCreatureAction_BriarRoseInvulnerableTeleport action;
        action.PCreature.m_pItem = &creature;
        action.PEnemy.m_pItem = 0;
        action.Initialise();
        CHECK(modes.Calls == 0 && g_effectCalls == 0 && player.Calls == 0);
        CHECK(defClass.RefCount == 1 && defClass.Destroyed == 0);
    }
    if (g_failures == 0)
        std::printf("008c1f70_TEST PASS\n");
    return g_failures ? 1 : 0;
}
