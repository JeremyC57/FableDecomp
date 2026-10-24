// Behaviour test for 0x0085F2B0 NCreatureMode::IsLyingDown (chunk 402): the reconstructed body (copied from src/0085f2b0.cpp)
// run over a working model of the thing, its interfaces and the creature-mode manager.
#include <cstdio>
#if !defined(_MSC_VER)
#define __fastcall
#endif

// Working model: interface bits + a sorted interface table with the retail end/hit lookup, a creature-mode manager
// that records RemoveMode calls, configurable creature actions and stand-ins for the out-of-line callees.
class CTCBase {
public:
    virtual ~CTCBase() { }
};
class CModeBase {
public:
    long Type;
    CModeBase() : Type(0) { }
    long GetType() const { return Type; }
};
class CTCCreatureModeManager : public CTCBase {
public:
    CModeBase     Current;
    unsigned long Active[16];
    int           ActiveCount;
    unsigned long Removed[16];
    int           RemovedCount;
    CTCCreatureModeManager() : ActiveCount(0), RemovedCount(0) { }
    const CModeBase& GetCurrentMode() const { return Current; }
    bool IsModeActive(unsigned long mode) const
    {
        for (int i = 0; i < ActiveCount; ++i)
            if (Active[i] == mode)
                return true;
        return false;
    }
    void RemoveMode(unsigned long mode)
    {
        if (RemovedCount < 16)
            Removed[RemovedCount++] = mode;
    }
};
class CCreatureActionBase {
public:
    bool PushableByHero, AlwaysPushableBySelfAvoidance, EvadeAllHits, CauseRecoil, PreventingLookTurning;
    CCreatureActionBase()
        : PushableByHero(true), AlwaysPushableBySelfAvoidance(false), EvadeAllHits(false), CauseRecoil(false),
          PreventingLookTurning(false) { }
    bool IsPhysicsPushableByHero() const { return PushableByHero; }
    bool IsAlwaysPhysicsPushableBySelfAvoidance() const { return AlwaysPushableBySelfAvoidance; }
    bool IsToEvadeAllHits() const { return EvadeAllHits; }
    bool IsToCauseRecoil() const { return CauseRecoil; }
    bool IsPreventingLookTurning() const { return PreventingLookTurning; }
};
class CPlayer {
public:
    bool ZTargeting;
    bool MainPlayer;
    bool FirstPersonTargeting;
    CPlayer() : ZTargeting(false), MainPlayer(false), FirstPersonTargeting(false) { }
};
class CPlayerMethods : public CPlayer {
public:
    bool IsMainPlayer() const { return MainPlayer; }
    bool IsFirstPersonTargeting() const { return FirstPersonTargeting; }
};
enum ETargetingType { TARGETING_TYPE_NONE, TARGETING_TYPE_A, TARGETING_TYPE_B, TARGETING_TYPE_C, TARGETING_TYPE_D, TARGETING_TYPE_E };
struct CTCInterfaceEntry {
    long     type;
    CTCBase* component;
};
class CThing {
public:
    unsigned int      m_tcPresent[9];
    CTCInterfaceEntry m_entries[17];
    int               m_count;
    unsigned char     ThingTypeTag;
    unsigned long     ThingTypeFlags;
    bool              Dead;
    unsigned char     PhysicalFlags;
    bool              Player;
    CPlayerMethods    PlayerRecord;
    CThing() : m_count(0), ThingTypeTag(0), ThingTypeFlags(0), Dead(false), PhysicalFlags(0), Player(false)
    {
        for (int i = 0; i < 9; ++i)
            m_tcPresent[i] = 0;
        m_entries[0].type = 0;
        m_entries[0].component = 0;
    }
    virtual ~CThing() { }
    void AddTC(long type, CTCBase* component)
    {
        int i = m_count;
        while (i > 0 && m_entries[i - 1].type > type) {
            m_entries[i] = m_entries[i - 1];
            --i;
        }
        m_entries[i].type = type;
        m_entries[i].component = component;
        ++m_count;
        m_entries[m_count].type = 0;
        m_entries[m_count].component = 0;
        m_tcPresent[type >> 5] |= 1u << (type & 31);
    }
    unsigned int HasTC(long type) const { return m_tcPresent[type >> 5] & (1u << (type & 31)); }
    CTCBase* GetTC(long type) const
    {
        const CTCInterfaceEntry* it = m_entries;
        const CTCInterfaceEntry* last = m_entries + m_count;
        while (it != last && it->type < type)
            ++it;
        return (it == last || type < it->type ? last : it)->component;
    }
    template<class T> bool GetInterface(T** ppOut, long type) const
    {
        if (HasTC(type)) {
            *ppOut = (T*)GetTC(type);
            return true;
        }
        return false;
    }
    const CTCCreatureModeManager* GetModeManager() const { return (const CTCCreatureModeManager*)GetTC(0x31); }
    CTCCreatureModeManager* GetModeManager() { return (CTCCreatureModeManager*)GetTC(0x31); }
    bool IsPlayer() const { return Player; }
    CPlayer& GetPlayer() { return PlayerRecord; }
};
class CThingCreatureBase : public CThing {
public:
    CCreatureActionBase* PCurrentAction;
    long                 MovementState;
    bool                 PerformingAnyAction;
    CThingCreatureBase() : PCurrentAction(0), MovementState(0), PerformingAnyAction(false)
    {
        ThingTypeTag = 2;
        ThingTypeFlags = 8 | 2;
    }
    bool IsPerformingAnyAction() const { return PerformingAnyAction; }
};
class CTCCarrying : public CTCBase {
public:
    CThing* Primary;
    CTCCarrying() : Primary(0) { }
    CThing* GetThingInPrimarySlot() const { return Primary; }
};
class CTCWeapon : public CTCBase {
public:
    const CThing* UsableBy;
    CTCWeapon() : UsableBy(0) { }
    bool IsUsableBy(const CThing& thing) const { return &thing == UsableBy; }
};
class CPlayerManager {
public:
    const CThing* MainPlayer;
    CPlayerManager() : MainPlayer(0) { }
    bool IsThingMainPlayer(const CThing& thing) const { return &thing == MainPlayer; }
};
class CMainGameComponent {
public:
    CPlayerManager* PlayerManager;
};
CPlayerManager      gPlayerManager;
CMainGameComponent  gMainGameComponent = { &gPlayerManager };
CMainGameComponent* g_pMainGameComponent = &gMainGameComponent;
bool g_SprintEnabled = true;
bool g_bHeroRunOption = true;
class CTCHeroSpecialMovement : public CTCBase {
public:
    bool Available;
    CTCHeroSpecialMovement() : Available(true) { }
    bool IsAvailableForSprint() const { return Available; }
};
class CTCTargeted : public CTCBase {
public:
    unsigned long TargetingFlags;
    CTCTargeted() : TargetingFlags(0) { }
};
class CTCTargetedMethods : public CTCTargeted {
public:
    void SetTargetingFlags(unsigned long flags) { TargetingFlags = flags; }
};
class CTCAIScratchpad : public CTCBase {
public:
    bool PhysicsPushable;
    CTCAIScratchpad() : PhysicsPushable(true) { }
};
bool gLightningable = true;
class CTCWillResponse {
public:
    static bool __fastcall IsThingLightningable(const CThing* thing) { return thing != 0 && gLightningable; }
};

namespace NCreatureMode {
bool __fastcall IsRolling(const CThing& thing);
bool __fastcall IsToDeflectAllHits(const CThing& thing);
bool __fastcall IsCarryingTooHeavyWeapon(const CThing& thing);
bool __fastcall IsKnockedDown(const CThing& thing);
bool __fastcall IsStabbableByHero(const CThing& thing);
bool __fastcall IsIncapacitated(const CThing& thing);
bool __fastcall IsUnderground(const CThing& thing);
bool __fastcall IsAbleToUseConversationAnimations(const CThing& thing);
bool __fastcall IsLookTurningOverridenByParent(const CThing& thing);
bool __fastcall IsLyingDown(const CThing& thing);
bool __fastcall IsSitting(const CThing& thing);
bool __fastcall IsFaceHitterOnHit(const CThing& thing);
bool __fastcall IsAbleToMove(const CThing& thing);
bool __fastcall IsAvailableForHeroSprint(const CThing& thing);
bool __fastcall IsAvailableForHeroRun(const CThing& thing);
void __fastcall RemoveMovementBlockingModes(CThing& thing);
void __fastcall RemoveAnimationBlockingModes(CThing& thing);
unsigned long __fastcall GetOldTargetFlagsAndReplaceWith(CThing& thing, ETargetingType type);
void __fastcall RevertOldTargetFlags(CThing& thing, ETargetingType flags);
bool __fastcall IsLightningable(const CThing& thing);
bool __fastcall IsAbleToBlock(const CThing& thing);
bool __fastcall IsAbleToUseTurningAnimations(const CThing& thing);
bool __fastcall IsPhysicsPushableByHero(const CThing& thing);
bool __fastcall IsPhysicsPushableBySelfAvoidance(const CThing& thing);
}
using namespace NCreatureMode;

// A creature with a mode manager in mode `mode` (plus two unrelated interfaces around 0x31 for the lookup).
static CTCBase gOther;
static CTCCreatureModeManager* GiveModes(CThing& thing, long mode)
{
    CTCCreatureModeManager* modes = new CTCCreatureModeManager;
    modes->Current.Type = mode;
    thing.AddTC(0x30, &gOther);
    thing.AddTC(0x31, modes);
    thing.AddTC(0x32, &gOther);
    return modes;
}
static bool ModeCheck(bool (__fastcall *fn)(const CThing&), long mode)
{
    CThingCreatureBase creature;
    GiveModes(creature, mode);
    return fn(creature);
}
static int gFailures = 0;
static void Check(bool ok, const char* what)
{
    if (!ok) {
        printf("  check failed: %s\n", what);
        ++gFailures;
    }
}

// body under test
bool __fastcall NCreatureMode::IsLyingDown(const CThing& thing)
{
    if (thing.HasTC(0x31)) {
        long mode = thing.GetModeManager()->GetCurrentMode().GetType();
        if (mode == 0x0d || mode == 0x1f || mode == 0x1d)
            return true;
    }
    return false;
}

int main()
{
    Check(ModeCheck(IsLyingDown, 0xd) == true, "IsLyingDown mode 0xd");
    Check(ModeCheck(IsLyingDown, 0x1f) == true, "IsLyingDown mode 0x1f");
    Check(ModeCheck(IsLyingDown, 0x1d) == true, "IsLyingDown mode 0x1d");
    Check(ModeCheck(IsLyingDown, 0x1) == false, "IsLyingDown mode 0x1");
    Check(ModeCheck(IsLyingDown, 0x8) == false, "IsLyingDown mode 0x8");
    Check(ModeCheck(IsLyingDown, 0x43) == false, "IsLyingDown mode 0x43");
    { CThingCreatureBase bare; Check(IsLyingDown(bare) == false, "IsLyingDown without a mode manager"); }
    if (gFailures == 0) {
        printf("0085f2b0_TEST PASS\n");
        return 0;
    }
    printf("0085f2b0_TEST FAIL\n");
    return 1;
}