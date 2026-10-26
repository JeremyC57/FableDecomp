// CTCCombat::UpdateCombatTimers() -- per-frame combat bookkeeping: track action/strike state, run the combo and handedness grace countdowns, the unarmed idle timer, the block-counter timer, every combat ability's timers, and the block hold/blocking counters
// Leaf: UpdateCombatTimers
// Retail 0x007339E0, 400 bytes, `ret`.
// Callees: CThingCreatureBase::IsPerformingAnyAction 0x006622A0, the current action's slot 15 (+0x3C, is-strike),
// CThingCreatureBase::SetLastActionHandedness 0x006619F0, the block-hold expiry 0x007338A0 (labelled
// CAIStateGroup_MinionReactToNoise::Cleanup), __ftol2 0x00BFEA70; globals: the unarmed idle frame count 0x013BAD40, the
// frames-per-second long 0x01375550.
// Layout: engine/CTCCombat.h agrees up to ParentCreature (+0x24); after the 12-byte retail vectors retail sits 4/8 lower:
// BlockCounterTimer +0x50, PerformingAction +0x60, PerformingStrikeAction +0x61, Blocking +0x63, BlockHoldTimer +0x64,
// BlockingNumFrames +0x68, CombatAbilities (STLport multimap header) +0x70 - so a local model is used. The creature's
// current action +0x100 and last-action handedness +0x138; the ability node value: data +0x14 (cooldown seconds +0x0C),
// Timer +0x1C, Cooldown +0x20.
// Shape: the two state bools and the previous values live in frame bytes; the abilities are walked with STLport's inline
// `_M_increment` on the node pointer, end re-read from the header each pass; the ability data pointer is a named local read at
// the top of each pass (retail loads it before the timer test).
extern long g_UnarmedCombatIdleFrames;                     // 0x013BAD40
extern long g_framesPerSecond;                             // 0x01375550
class CCombatActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14();
    virtual bool IsStrikeAction();                         // slot 15 (+0x3c)
};
class CThingCreatureBase { public:
    unsigned char      _pad000[0x100];
    CCombatActionBase* CurrentAction;                      // +0x100
    unsigned char      _pad104[0x138 - 0x104];
    long               LastActionHandedness;               // +0x138
    bool IsPerformingAnyAction();                          // 0x006622A0
    void SetLastActionHandedness(long handedness);         // 0x006619F0
};
class CCombatAbilityData { public:
    unsigned char _pad00[0x0c];
    float         CooldownSeconds;                         // +0x0c
};
struct SAbilityNode {                                      // multimap node: color, parent, left, right, value
    long               Color;
    SAbilityNode*      Parent;                             // +0x04
    SAbilityNode*      Left;                               // +0x08
    SAbilityNode*      Right;                              // +0x0c
    long               Type;                               // +0x10
    CCombatAbilityData* Data;                              // +0x14 (CCountedPointer)
    void*              DataInfo;                           // +0x18
    long               Timer;                              // +0x1c
    long               Cooldown;                           // +0x20
};
static inline SAbilityNode* _M_increment(SAbilityNode* node)
{
    if (node->Right != 0) {
        node = node->Right;
        while (node->Left != 0)
            node = node->Left;
    } else {
        SAbilityNode* y = node->Parent;
        while (node == y->Right) {
            node = y;
            y = y->Parent;
        }
        if (node->Right != y)
            node = y;
    }
    return node;
}
class CTCCombat {
public:
    void*               __vftable;                         // +0x00
    unsigned char       _pad04[0x0c - 0x04];
    long                ComboChainPos;                     // +0x0c
    long                ComboChainID;                      // +0x10
    long                ComboChainGracePeriodCountdown;    // +0x14
    long                HandednessGracePeriodCountdown;    // +0x18
    long                UnarmedCombatIdleCountdown;        // +0x1c
    void*               CombatManager;                     // +0x20
    CThingCreatureBase* ParentCreature;                    // +0x24
    unsigned char       _pad28[0x50 - 0x28];
    long                BlockCounterTimer;                 // +0x50
    unsigned char       _pad54[0x60 - 0x54];
    bool                PerformingAction;                  // +0x60
    bool                PerformingStrikeAction;            // +0x61
    bool                StartingToBlock;                   // +0x62
    bool                Blocking;                          // +0x63
    long                BlockHoldTimer;                    // +0x64
    long                BlockingNumFrames;                 // +0x68
    long                UnsheathingWeaponType;             // +0x6c
    SAbilityNode*       CombatAbilities;                   // +0x70 (multimap header node)
    void OnBlockHoldExpired();                             // 0x007338A0
    void UpdateCombatTimers();
};

void CTCCombat::UpdateCombatTimers()
{
    bool performing = ParentCreature->IsPerformingAnyAction();
    bool strike = performing ? ParentCreature->CurrentAction->IsStrikeAction() : false;
    bool wasPerforming = PerformingAction;
    bool wasStrike = PerformingStrikeAction;
    if (!strike) {
        if (wasStrike)
            ComboChainGracePeriodCountdown = HandednessGracePeriodCountdown = 3;
        if (ComboChainGracePeriodCountdown)
            --ComboChainGracePeriodCountdown;
        else if (ComboChainPos != -777 || ComboChainID != -999) {
            ComboChainPos = -777;
            ComboChainID = -999;
        }
        if (HandednessGracePeriodCountdown)
            --HandednessGracePeriodCountdown;
        else if (ParentCreature->LastActionHandedness)
            ParentCreature->SetLastActionHandedness(0);
    }
    PerformingAction = performing;
    PerformingStrikeAction = strike;
    if (wasPerforming && wasStrike)
        UnarmedCombatIdleCountdown = g_UnarmedCombatIdleFrames;
    else if (UnarmedCombatIdleCountdown)
        --UnarmedCombatIdleCountdown;
    if (BlockCounterTimer >= 0)
        --BlockCounterTimer;
    for (SAbilityNode* node = CombatAbilities->Left; node != CombatAbilities; node = _M_increment(node)) {
        CCombatAbilityData* data = node->Data;
        if (node->Timer >= 0)
            --node->Timer;
        if (node->Cooldown >= 0 && --node->Cooldown == -1)
            node->Timer = (long)(g_framesPerSecond * data->CooldownSeconds);
    }
    if (BlockHoldTimer == 0)
        OnBlockHoldExpired();
    if (BlockHoldTimer >= 0)
        --BlockHoldTimer;
    if (Blocking)
        ++BlockingNumFrames;
    else if (BlockingNumFrames > 0)
        BlockingNumFrames = 0;
}