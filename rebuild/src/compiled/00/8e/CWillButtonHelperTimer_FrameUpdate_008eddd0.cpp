// CWillButtonHelperTimer::FrameUpdate() -- per-frame will-button charge timer: bail if the creature is gone or dead; for an AI caster count up until its target level (or while it holds the button), for the player keep counting while the ability's button (or quick-access event) is still held, raising the accumulated level each time the held time passes the next level's timing; a finished player release clears the ability inventory's quick-access state
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EDDD0, 531 bytes (+ int3 padding), plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0
//   and non-const 0x0040F020 (interface 0x6F CTCInventoryAbilities = presence word 3 bit 15, `test ch,ch ; jns`);
//   CTCInventoryAbilities::GetButtonAbilityIsSetTo 0x005EDF33, IsAbilityQuickAccessEventInQueue 0x005EE99D and the empty
//   0x005EDCEF (descriptive name ClearQuickAccessState: a bare `ret`); CThing::GetPlayer 0x004C7A30, CPlayer::GetPlayerNumber
//   0x00487510, CMainGameComponent::GetGamePlayerInterface 0x00416359, CGamePlayerInterface::IsAssignableSpecialMoveButtonInQueue
//   0x00445E90; g_pMainGameComponent 0x013B86A0.
// Layout: Ego_r.pdb CWillButtonHelperTimer (56) with the retail 12-byte STLport vector: ButtonHeldDownTimer +0, WillAccumulatedLevel
//   +4, PCreature +8, MyAbility +0x10, CurrentState +0x14, WillPowerUpAccumulatedTimings +0x18, MaxNumberLevels +0x24,
//   CurMaxLevels +0x28, WillIncrementedFlag +0x2c, UpdateAccLevel +0x2d, IsAICasting +0x2e, IsAIHoldingDown +0x2f, LevelToCast +0x30.
//   CTCInventoryAbilities' bool at +0x142 is the quick-access-pending flag (named descriptively).
// Shape: the inventory lookup through a const thing (const LowerBound, component pre-nulled in edi -- declared before the
//   creature read so the xor sits ahead of GetPItem), the clear through the
//   non-const one; the main-game-component global is read through an inline accessor so it loads before the GetPlayer chain; the two level-up blocks are written
//   out separately (the AI one also checks the level is non-negative).
class CBaseObject;
class CTCBase;
class CPlayer {
public:
    long GetPlayerNumber();                         // 0x00487510
};
class CGamePlayerInterface {
public:
    bool IsAssignableSpecialMoveButtonInQueue(long button, long eventType, long playerNumber) const;   // 0x00445E90
};
class CMainGameComponent {
public:
    CGamePlayerInterface* GetGamePlayerInterface(); // 0x00416359
};
extern CMainGameComponent* g_pMainGameComponent;    // 0x013B86A0
inline CMainGameComponent* GetMainGameComponent() { return g_pMainGameComponent; }

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                          // +0x91 (bit 0: dead / being removed)
    CPlayer* GetPlayer();                           // 0x004C7A30
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

class CTCInventoryAbilities {
public:
    unsigned char _pad[0x142];
    bool QuickAccessPending;                        // +0x142
    bool GetButtonAbilityIsSetTo(long ability, long* button);   // 0x005EDF33
    bool IsAbilityQuickAccessEventInQueue(long ability);        // 0x005EE99D
    void ClearQuickAccessState();                   // 0x005EDCEF
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                  // 0x00A01B50
    CBaseObject* PItem;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
    T* Get() const { return (T*)GetPItem(); }
};

class CWillButtonHelperTimer {
public:
    long ButtonHeldDownTimer;                       // +0x00
    long WillAccumulatedLevel;                      // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08
    long MyAbility;                                 // +0x10 EHeroAbility
    long CurrentState;                              // +0x14 EButtonHeldState
    float* TimingsStart;                            // +0x18 vector<float> WillPowerUpAccumulatedTimings
    float* TimingsFinish;                           // +0x1c
    float* TimingsEnd;                              // +0x20
    float MaxNumberLevels;                          // +0x24
    float CurMaxLevels;                             // +0x28
    bool WillIncrementedFlag;                       // +0x2c
    bool UpdateAccLevel;                            // +0x2d
    bool IsAICasting;                               // +0x2e
    bool IsAIHoldingDown;                           // +0x2f
    long LevelToCast;                               // +0x30
    void FrameUpdate();
};

void CWillButtonHelperTimer::FrameUpdate()
{
    WillIncrementedFlag = false;
    if (!PCreature.Get())
        return;
    if (PCreature->Flags91 & 1)
        return;
    if (IsAICasting) {
        if (CurrentState == 0) {
            if (!IsAIHoldingDown && WillAccumulatedLevel >= LevelToCast) {
                CurrentState = 2;
            } else {
                ++ButtonHeldDownTimer;
                if (WillAccumulatedLevel >= 0 && WillAccumulatedLevel < CurMaxLevels
                    && ButtonHeldDownTimer > TimingsStart[WillAccumulatedLevel] && UpdateAccLevel) {
                    WillIncrementedFlag = true;
                    ++WillAccumulatedLevel;
                }
            }
        } else {
            CurrentState = CurrentState == 1 ? 2 : 3;
        }
    } else {
        if (CurrentState == 0) {
            CTCInventoryAbilities* abilities = 0;
            const CThing* creature = PCreature.Get();
            if (creature->HasTC(0x6f))
                abilities = (CTCInventoryAbilities*)creature->GetTC(0x6f);
            long button = -1;
            if ((!abilities->QuickAccessPending && abilities->GetButtonAbilityIsSetTo(MyAbility, &button)
                 && GetMainGameComponent()->GetGamePlayerInterface()->IsAssignableSpecialMoveButtonInQueue(button, 2, PCreature->GetPlayer()->GetPlayerNumber()))
                || (abilities->QuickAccessPending && abilities->IsAbilityQuickAccessEventInQueue(MyAbility))) {
                ++ButtonHeldDownTimer;
                if (WillAccumulatedLevel < CurMaxLevels
                    && ButtonHeldDownTimer > TimingsStart[WillAccumulatedLevel] && UpdateAccLevel) {
                    WillIncrementedFlag = true;
                    ++WillAccumulatedLevel;
                }
            } else {
                CurrentState = 2;
            }
        } else {
            CurrentState = CurrentState == 1 ? 2 : 3;
        }
    }
    if (!IsAICasting && CurrentState == 2) {
        CThing* creature = PCreature.Get();
        if (creature->HasTC(0x6f))
            ((CTCInventoryAbilities*)creature->GetTC(0x6f))->ClearQuickAccessState();
    }
}