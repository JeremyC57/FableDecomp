// CInputProcessGameBase::IsGameActionEventInQueue(long action, long buttonEventType, float* pTime) -- forward to the player interface (+0x24) with this player's number (CPlayer +0x228) inserted as the third argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687D80, length 37 bytes (+ int3 padding).
// Callees: CPlayer::GetPlayerNumber @ 0x00487510 (`mov eax,[ecx+228h]`; the label "CWorld::DrawGetEnvironment" is propagated),
// CGamePlayerInterface::IsGameActionEventInQueue(long, long, long, float*) @ 0x00445CB0.
// Layout: engine/CInputProcessGameBase.h (MyPlayer +0x18, PGamePlayerInterface +0x24). Shape: the arguments are pushed right to left --
// the time pointer first, then the player number call, then the two words reloaded from their slots; the bool rides through al, `ret 0Ch`.
struct CPlayer {
    long GetPlayerNumber();                         // 0x00487510
};
struct CGamePlayerInterface {
    bool IsGameActionEventInQueue(long action, long buttonEventType, long playerNumber, float* pTime);   // 0x00445CB0
};
struct CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
    virtual void v00();
    unsigned char         _pad04[0x10];             // +0x04 CAInputProcess
    void*                 World;                    // +0x14
    CPlayer*              MyPlayer;                 // +0x18
    void*                 DefinitionManager;        // +0x1c
    void*                 Component;                // +0x20
    CGamePlayerInterface* PGamePlayerInterface;     // +0x24
    bool IsGameActionEventInQueue(long action, long buttonEventType, float* pTime);
};

bool CInputProcessGameBase::IsGameActionEventInQueue(long action, long buttonEventType, float* pTime)
{
    return PGamePlayerInterface->IsGameActionEventInQueue(action, buttonEventType, MyPlayer->GetPlayerNumber(), pTime);
}
