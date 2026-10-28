// Grade: RELOCATION_MATCH with cl 13.10.3077 (RTM); the DDK 13.10.4035 build emits a different /GS prologue for this body.
// CInputProcessDigging::ProcessInput(const CInputEvent& event, CProcessedInput& input) -- when the digging action button (game action 3) is pressed and the hero thing has component kind 0xc6, queue game event 0x1a carrying the long 0x17 and the byte 1
// Leaf: ?ProcessInput@CInputProcessDigging@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00690C70, 162 bytes, `ret 8`.
// Callees: CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90 and IsEventGameAction @ 0x00687CF0 (the landed
//   models' names), CPlayer::GetPControlledCreature @ 0x00487DC0, CMemoryBuffer::Copy @ 0x009F1760 (AddLong / AddUBYTE inlined),
//   CProcessedInput::AddGameEvent(const CGameEvent&) @ 0x00A0D340, __security_check_cookie @ 0x00BFE9F9.
// Layout: engine/CInputProcessGameBase.h (MyPlayer +0x18), modelled locally as in the landed 0x0068EA60 because the
//   class carries the virtual ProcessInput; CGameEvent (Ego_r.pdb, 40 bytes) as in the landed 0x0068E3C0; the hero
//   thing's component bitset +0x20 (kind 0xc6 = byte +0x38 bit 6).
// Shape: one `&&` test chain sharing the cookie-check exit; the event built in the frame (the /GS cookie above it), the
//   two zero bytes from one `xor al,al`, AddLong / AddUBYTE homing their by-value argument in the dead `event` slot.
class CInputEvent;
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { Copy(sizeof(value), &value); }
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    __forceinline void AddGameEvent(long type, long data, unsigned char flag)
    {
        CGameEvent gameEvent(type);
        gameEvent.AddLong(data);
        gameEvent.AddUBYTE(flag);
        AddGameEvent(gameEvent);
    }
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                     // +0x20: which component kinds the thing holds
    bool HasTC(long kind) const
    {
        if (m_tcMask[kind >> 5] & (1UL << (kind & 31)))
            return true;
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                        // 0x00487DC0
};
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
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
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
class CInputProcessDigging : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& input);
};

void CInputProcessDigging::ProcessInput(const CInputEvent& event, CProcessedInput& input)
{
    if (IsEventButtonPressed(event) && IsEventGameAction(3, event) && MyPlayer->GetPControlledCreature()->HasTC(0xc6)) {
        input.AddGameEvent(0x1a, 0x17, 1);
    }
}