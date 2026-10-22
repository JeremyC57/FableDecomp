// Behaviour test for 0x008597A0 CCombatActionBase::ProcessStrikeEvents: runs the reconstructed body over stubbed callees;
// checks the new-strike-event path (damage, LastWeaponUsedInStrike, aug fire), the ended-event and no-strike clears (alive and
// dead weapons), and the three timed flags (interrupt, reset combo with the combat component, movement interrupt).
#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif

long g_FramesPerSecond = 15;

struct CTCBase { };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};

static int g_fired = 0, g_ended = 0, g_resets = 0;
struct CTCObjectAugmentations : CTCBase {
    void FireCombatAugEffect() { ++g_fired; }
    void EndStrikeAction() { ++g_ended; }
};
struct CTCCombat : CTCBase {
    void ResetComboChainPos() { ++g_resets; }
};

struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long ThingTypeFlags;
    unsigned char Dead : 1;
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
};
static int g_ipCtors = 0;
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* item) { m_pItem = item; ++g_ipCtors; }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { m_pItem = other.m_pItem; return *this; }
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer& thing) { return thing.GetPItem() && !thing.GetPItem()->Dead; }

struct CStrikeEvent { long StrikeEventIndex; };
struct CStrikeEventList {
    struct SNode { SNode* next; SNode* prev; };
    SNode* head;
    bool HasEvents() const { return head->next != head; }
};

static int g_ownerCalls = 0;
struct CAnimationOwner;
struct CCreatureActionBase {
    bool m_active;
    long m_animDelay;
    virtual ~CCreatureActionBase() { }
    virtual long GetAnimDelayTime() const { return m_animDelay; }
    virtual bool IsStrikeEventActive() const { return m_active; }
    CBaseIntelligentPointer PCreature;
    long FramesRunning;
    CAnimationOwner* GetPCreatureAnimationOwner() const { ++g_ownerCalls; return 0; }
};

static CStrikeEvent* g_firstEvent = 0;
static CThing* g_weaponUsed = 0;
static int g_damage = 0;
static float g_damageFraction = 0;
struct CCombatActionBase : CCreatureActionBase {
    CStrikeEventList StrikeEvents;
    float StrikeEventInterruptTime;
    float StrikeEventResetComboTime;
    long  MovementEventInterruptFrame;
    bool  HasStrikeEventInterrupt;
    bool  HasStrikeEventResetCombo;
    bool  HasMovementEventInterrupt;
    bool  StrikeEventInterruptActive;
    bool  MovementEventInterruptActive;
    long  ActiveStrikeEventIndex;
    CIntelligentPointer LastWeaponUsedInStrike;
    CStrikeEvent* GetFirstActiveStrikeEvent() { return g_firstEvent; }
    CThing* GetWeaponUsedInStrikeAction() { return g_weaponUsed; }
    void PerformPossibleDamageAction(float fraction) { ++g_damage; g_damageFraction = fraction; }
    void ProcessStrikeEvents();
};

void CCombatActionBase::ProcessStrikeEvents()
{
    long framesPerSecond = g_FramesPerSecond;
    GetPCreatureAnimationOwner();
    if (StrikeEvents.HasEvents() && IsStrikeEventActive()) {
        PerformPossibleDamageAction(0.25f);
        CStrikeEvent* strikeEvent = GetFirstActiveStrikeEvent();
        if (strikeEvent) {
            if (strikeEvent->StrikeEventIndex != ActiveStrikeEventIndex) {
                ActiveStrikeEventIndex = strikeEvent->StrikeEventIndex;
                if (PCreature.GetPItem()->ThingTypeFlags & 0x20) {
                    CThing* weapon = GetWeaponUsedInStrikeAction();
                    if (weapon) {
                        LastWeaponUsedInStrike = weapon;
                        CTCObjectAugmentations* augmentations;
                        if (weapon->GetInterface(&augmentations, 0xbf))
                            augmentations->FireCombatAugEffect();
                    }
                }
            }
        } else if (ActiveStrikeEventIndex >= 0) {
            if (IS_THING_ALIVE(LastWeaponUsedInStrike)) {
                CTCObjectAugmentations* augmentations;
                if (LastWeaponUsedInStrike.GetPItem()->GetInterface(&augmentations, 0xbf))
                    augmentations->EndStrikeAction();
            }
            ActiveStrikeEventIndex = -1;
        }
    } else if (ActiveStrikeEventIndex >= 0) {
        if (LastWeaponUsedInStrike.GetPItem() && !LastWeaponUsedInStrike.GetPItem()->Dead) {
            CTCObjectAugmentations* augmentations;
            if (LastWeaponUsedInStrike.GetPItem()->GetInterface(&augmentations, 0xbf))
                augmentations->EndStrikeAction();
        }
        ActiveStrikeEventIndex = -1;
    }
    if (HasStrikeEventInterrupt && !StrikeEventInterruptActive &&
        FramesRunning > (long)(GetAnimDelayTime() + framesPerSecond * StrikeEventInterruptTime))
        StrikeEventInterruptActive = true;
    if (HasStrikeEventResetCombo &&
        FramesRunning > (long)(GetAnimDelayTime() + framesPerSecond * StrikeEventResetComboTime)) {
        HasStrikeEventResetCombo = false;
        CTCCombat* combat;
        PCreature.GetPItem()->GetInterface(&combat, 0x30);
        combat->ResetComboChainPos();
    }
    if (HasMovementEventInterrupt && !MovementEventInterruptActive && MovementEventInterruptFrame == FramesRunning)
        MovementEventInterruptActive = true;
}

static void Clear(CThing& t)
{
    for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_interfaces.first = t.m_interfaces.last = 0;
    t.ThingTypeFlags = 0; t.Dead = 0;
}

int main()
{
    bool ok = true;
    CTCObjectAugmentations aug; CTCCombat combat;
    CInterfaceMap::SEntry weaponMap[2];
    weaponMap[0].type = 0xbf; weaponMap[0].component = &aug;
    weaponMap[1].type = 0; weaponMap[1].component = 0;
    CThing weapon; Clear(weapon);
    weapon.m_presentBits[0xbf >> 5] = 1UL << (0xbf & 31);
    weapon.m_interfaces.first = weaponMap; weapon.m_interfaces.last = weaponMap + 1;
    CInterfaceMap::SEntry creatureMap[2];
    creatureMap[0].type = 0x30; creatureMap[0].component = &combat;
    CThing creature; Clear(creature);
    creature.m_presentBits[0x30 >> 5] = 1UL << (0x30 & 31);
    creature.m_interfaces.first = creatureMap; creature.m_interfaces.last = creatureMap + 1;
    creature.ThingTypeFlags = 0x20;

    CStrikeEventList::SNode head, node;
    head.next = &node; head.prev = &node; node.next = &head; node.prev = &head;
    CStrikeEvent event; event.StrikeEventIndex = 3;

    CCombatActionBase action;
    action.m_active = true; action.m_animDelay = 10;
    action.PCreature.m_pItem = &creature;
    action.FramesRunning = 0;
    action.StrikeEvents.head = &head;
    action.StrikeEventInterruptTime = 2.0f;   // 10 + 30 = 40
    action.StrikeEventResetComboTime = 3.0f;  // 10 + 45 = 55
    action.MovementEventInterruptFrame = 7;
    action.HasStrikeEventInterrupt = true; action.HasStrikeEventResetCombo = true; action.HasMovementEventInterrupt = true;
    action.StrikeEventInterruptActive = false; action.MovementEventInterruptActive = false;
    action.ActiveStrikeEventIndex = -1;

    // 1: a new strike event fires the weapon's aug effect
    g_firstEvent = &event; g_weaponUsed = &weapon;
    action.ProcessStrikeEvents();
    ok = ok && g_ownerCalls == 1 && g_damage == 1 && g_damageFraction == 0.25f;
    ok = ok && action.ActiveStrikeEventIndex == 3 && action.LastWeaponUsedInStrike.GetPItem() == &weapon && g_ipCtors == 1;
    ok = ok && g_fired == 1 && g_ended == 0 && g_resets == 0;
    ok = ok && !action.StrikeEventInterruptActive && !action.MovementEventInterruptActive && action.HasStrikeEventResetCombo;

    // 2: the same event again does nothing new
    action.ProcessStrikeEvents();
    ok = ok && g_damage == 2 && g_fired == 1 && g_ipCtors == 1 && action.ActiveStrikeEventIndex == 3;

    // 3: the event ended (no first active event): clear the alive weapon's aug effect
    g_firstEvent = 0;
    action.ProcessStrikeEvents();
    ok = ok && g_ended == 1 && action.ActiveStrikeEventIndex == -1;
    action.ProcessStrikeEvents();                  // index already -1: no second clear
    ok = ok && g_ended == 1;

    // 4: no strike active, index set, dead weapon: index reset without a clear
    action.m_active = false; action.ActiveStrikeEventIndex = 5; weapon.Dead = 1;
    action.ProcessStrikeEvents();
    ok = ok && g_ended == 1 && action.ActiveStrikeEventIndex == -1 && g_damage == 4;
    // 4b: no strike active, alive weapon: inline path clears
    action.ActiveStrikeEventIndex = 5; weapon.Dead = 0;
    action.ProcessStrikeEvents();
    ok = ok && g_ended == 2 && action.ActiveStrikeEventIndex == -1;
    // 4c: empty list counts as no strike
    head.next = &head; action.m_active = true; action.ActiveStrikeEventIndex = 2;
    action.ProcessStrikeEvents();
    ok = ok && g_ended == 3 && action.ActiveStrikeEventIndex == -1 && g_damage == 4;

    // 5: a creature without flag 0x20 does not touch the weapon
    head.next = &node; g_firstEvent = &event; creature.ThingTypeFlags = 0; action.LastWeaponUsedInStrike.m_pItem = 0;
    action.ProcessStrikeEvents();
    ok = ok && action.ActiveStrikeEventIndex == 3 && action.LastWeaponUsedInStrike.GetPItem() == 0 && g_fired == 1;

    // 6: timed flags
    action.FramesRunning = 40;                     // not past 40
    action.ProcessStrikeEvents();
    ok = ok && !action.StrikeEventInterruptActive && action.HasStrikeEventResetCombo;
    action.FramesRunning = 41;
    action.ProcessStrikeEvents();
    ok = ok && action.StrikeEventInterruptActive && action.HasStrikeEventResetCombo && g_resets == 0;
    action.FramesRunning = 56;
    action.ProcessStrikeEvents();
    ok = ok && !action.HasStrikeEventResetCombo && g_resets == 1;
    action.ProcessStrikeEvents();
    ok = ok && g_resets == 1 && !action.MovementEventInterruptActive;
    action.FramesRunning = 7;
    action.ProcessStrikeEvents();
    ok = ok && action.MovementEventInterruptActive;
    action.HasStrikeEventInterrupt = false; action.StrikeEventInterruptActive = false; action.FramesRunning = 100;
    action.ProcessStrikeEvents();
    ok = ok && !action.StrikeEventInterruptActive;

    if (!ok) { std::printf("008597a0_TEST FAIL\n"); return 1; }
    std::printf("008597a0_TEST PASS\n");
    return 0;
}
