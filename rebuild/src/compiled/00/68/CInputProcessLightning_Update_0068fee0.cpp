// CInputProcessLightning::Update(CProcessedInput& processed) -- unless the controlled creature is alive and holds component kind 0x94, queue game event 0x41 (stop the lightning spell)
// Grade: DIFFER(103v98) -- /GS lane gap: `processed` shadowed into esi at entry (`push esi ; mov esi,[esp+34h]`); retail reads the argument slot right before AddGameEvent; all other bytes (cl/dl reuse for the event bytes) match
// Leaf: Update
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068FEE0, length 98 bytes.
// Callees: CPlayer::GetControlledCreature @ 0x00487DD0 (MyPlayer +0x18), CProcessedInput::AddGameEvent @ 0x00A0D340,
// __security_check_cookie @ 0x00BFE9F9.
// Model: CGameEvent as the landed CTCInventoryBase::AddCloseInventoryEvent 0x005BC527 (Ego_r.pdb, 40 bytes); the thing's
// presence bitset at +0x20 (kind 0x94 = bit 20 of the word at +0x30) and dead bit +0x91 as the landed
// CGameScriptInterface rows. The input-process base as engine/CInputProcessGameBase.h (engine/CInputProcessLightning.h
// flattens it).
// Shape: the dead-bit mask 1 and the zero of the null test are reused (`cl`, `dl`) for the event's Valid / EndPos /
// Replacement bytes.
class CInputEvent;
struct CMemoryBuffer {
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
};
struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[0x51];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
class CPlayer {
public:
    CThing_Retail* GetControlledCreature();        // 0x00487DD0
};
struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90 (kind 0x13)
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0 (kind 0x15)
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
class CInputProcessLightning : public CInputProcessGameBase {
public:
    virtual void Update(CProcessedInput& processed);
};

void CInputProcessLightning::Update(CProcessedInput& processed)
{
    CThing_Retail* creature = MyPlayer->GetControlledCreature();
    if (creature == 0 || creature->m_dead || !creature->HasTC(0x94)) {
        CGameEvent gameEvent(0x41);
        processed.AddGameEvent(gameEvent);
    }
}