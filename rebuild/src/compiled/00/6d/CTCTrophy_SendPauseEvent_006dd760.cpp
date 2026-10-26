// CTCTrophy::SendPauseEvent(bool pause) -- queue game event 9 (pause) for the current player carrying the pause flag byte
// Leaf: SendPauseEvent
// Flags: /GS
// Retail: Fable.exe @ 0x006DD760, 117 bytes (`ret 4`, /GS cookie 0x0139C8A8). Caller: CTCTrophy vtable slot 26 0x006DF460.
//         Byte-identical body: CTCWife::SendPauseEvent 0x006CEBF0.
// Callees: CTCBase::PeekWorld() const 0x00686A80; CWorld::GetCurrentPlayer() 0x0049D9D0 (player number byte at +0x28);
//          CMemoryBuffer::Copy(unsigned long, const void*) 0x009F1760; CMainGameComponent::AddGameEvent(const CGameEvent&)
//          0x0041649C on g_pMainGameComponent 0x013B86A0; __security_check_cookie 0x00BFE9F9.
// Model: CGameEvent as the landed CInputProcessQuickAccessItems::CancelQuickAccessCharge 0x0068E3C0 (Ego_r.pdb, 40 bytes
//        over an empty CMemoryBuffer; the ctor's dead `Player = -1` store is kept).
// Shape: the flag byte is normalised IN the `pause` argument slot (`test ; setne ; mov [arg],cl`) and its address passed
//        to Copy: an inline AddBool(bool&) that normalises and copies its referent. By-value inline helpers (AddUBYTE(unsigned
//        char), AddBool(bool), AddBool(const bool&) with a `? true : false` temporary) all get their own frame slot.
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
    void AddBool(bool& value) { value = value ? true : false; Copy(1, &value); }
};
class CMainGameComponent { public:
    void AddGameEvent(const CGameEvent& event);    // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

class CPlayer { public:
    unsigned char _pad_0x00[0x28];
    signed char   m_playerNumber;                  // +0x28
};
class CWorld { public:
    CPlayer* GetCurrentPlayer() const;             // 0x0049D9D0
};

class CTCBase
{
public:
    virtual void v00();
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};

class CTCTrophy : public CTCBase { public:
    void SendPauseEvent(bool pause);
};

void CTCTrophy::SendPauseEvent(bool pause)
{
    CGameEvent gameEvent(9);
    gameEvent.Player = PeekWorld().GetCurrentPlayer()->m_playerNumber;
    gameEvent.AddBool(pause);
    g_pMainGameComponent->AddGameEvent(gameEvent);
}