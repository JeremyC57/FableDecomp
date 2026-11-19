// CTCHeroCentre::FrameUpdate() -- HEAD DRAFT (pending-event block of 1491 bytes): when the post flag (+0x30) is set and
// Grade: DIFFER(192v1491) 434 diff lines (triage-only 1v1491/461): head draft of the pending game-event block (pause / world flag guards, TC 0x16 test, event 0x1A AddData + AddGameEvent) under /GS; position update not drafted (retry wave 2 sec2c)
// neither the global pause byte nor the world's +0xA4 flag blocks it, clear the flag and, when the hero has TC 0x16, post
// game event 0x1A carrying this component and the main player's number; the hero-centre position update (0x0077D693..) is
// not drafted, nor retail's `and esp,-8` frame.
// Leaf: FrameUpdate
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077D5B0, 1491 bytes, /GS cookie 0x0139C8A8 checked per return.
class CThing { public: unsigned char _pad00[0x20]; unsigned long UsingComponents[9]; };
class CThingPlayerCreature : public CThing { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
class CPlayerManager { public: const CPlayer& GetMainPlayer() const; long GetMainPlayerNumber() const; };   // 0x00449960 / 0x00449700
class CWorld { public: unsigned char _pad00[0xa4]; bool HeroCentreBlocked; };
struct CGlobalState { unsigned char _pad00[0xfd]; bool Paused; };
extern CGlobalState* g_pGlobalState;                                    // 0x013B8790
class CGameEvent {
public:
    long Type; unsigned char Priority; unsigned char _pad05[3]; long DataSize; unsigned char Data[0x18];
    unsigned char Player; bool Flag29; bool Flag2A; bool Flag2B;
    void AddData(long size, const void* data);                          // 0x009F1760
};
class CMainGameComponent { public: void AddGameEvent(const CGameEvent& event); };   // 0x0041649C
extern CMainGameComponent* g_pMainGameComponent;                        // 0x013B86A0
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;
protected:
    CWorld* PeekWorld() const;                                          // 0x00686A80
    const CPlayerManager& PeekPlayerManager() const;                    // 0x00686D50
};
class CTCHeroCentre : public CTCBase {
public:
    unsigned char _pad08[0x30 - 0x08];
    bool PostPending;                                                   // +0x30
    void FrameUpdate();
};
void CTCHeroCentre::FrameUpdate()
{
    CTCHeroCentre* self = this;
    if (PostPending) {
        if (g_pGlobalState->Paused)
            return;
        if (PeekWorld()->HeroCentreBlocked)
            return;
        PostPending = false;
        if (!(PeekPlayerManager().GetMainPlayer().GetPControlledCreature()->UsingComponents[0] & 0x400000))
            return;
        long kind = 0xb;
        CGameEvent event;
        event.Type = 0x1a;
        event.Priority = 0xff;
        event.Flag29 = false;
        event.Flag2A = true;
        event.Flag2B = false;
        event.AddData(4, &self);
        event.Player = (unsigned char)PeekPlayerManager().GetMainPlayerNumber();
        g_pMainGameComponent->AddGameEvent(event);
        (void)kind;
        return;
    }
}
