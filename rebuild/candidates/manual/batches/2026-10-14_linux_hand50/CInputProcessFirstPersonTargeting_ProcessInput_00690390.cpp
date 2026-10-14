// CInputProcessFirstPersonTargeting::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- a held button event that is not game action 10 ends the player's mode 0x16 and marks the input handled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00690390, length 54 bytes. Vtable 0x01237C98 slot 4.
// Callees: CInputProcessGameBase::IsEventButtonHeld(const CInputEvent&) @ 0x00687BD0, CInputProcessGameBase::IsEventGameAction(long action, const CInputEvent&)
// @ 0x00687CF0 (forwards to CGamePlayerInterface::IsEventGameAction @ 0x00445BD0 through +0x24), CPlayer::RemoveMode(long) @ 0x00634190
// (MyPlayer +0x18), CProcessedInput::SetHandled @ 0x00A0D390 (NoGameEvents = 0, Type = 2).
// Shape: the event reference rides in edi for both predicate calls, nested ifs share the single exit, `ret 8`.
struct CInputEvent;
struct CProcessedInput {
    void SetHandled();                              // 0x00A0D390
};
struct CPlayer {
    void RemoveMode(long mode);                     // 0x00634190
};
struct CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
    virtual void v00();
    unsigned char _pad04[0x10];                     // +0x04 CAInputProcess
    void*         World;                            // +0x14
    CPlayer*      MyPlayer;                         // +0x18
    void*         DefinitionManager;                // +0x1c
    void*         Component;                        // +0x20
    void*         PGamePlayerInterface;             // +0x24
    bool IsEventButtonHeld(const CInputEvent& event);                  // 0x00687BD0
    bool IsEventGameAction(long action, const CInputEvent& event);     // 0x00687CF0
};
struct CInputProcessFirstPersonTargeting : CInputProcessGameBase {   // vtable 0x01237C98
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessFirstPersonTargeting::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (IsEventButtonHeld(event)) {
        if (!IsEventGameAction(10, event)) {
            MyPlayer->RemoveMode(0x16);
            processed.SetHandled();
        }
    }
}
