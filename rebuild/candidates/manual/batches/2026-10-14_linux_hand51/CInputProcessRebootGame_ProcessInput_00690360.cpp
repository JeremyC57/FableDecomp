// CInputProcessRebootGame::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- for an event of kind 0x15 (0x00A03B40), consume it (0x00A03B90) and request the reboot through the processed input (0x00A0D390)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00690360, length 48 bytes.
// Leaf: ?ProcessInput@CInputProcessRebootGame@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// The callee labels ("CWorld::DrawGetEnvironment", "GetCanItemBeQuickAssigned", "CWomanToAttract::Init") are propagated. Shape: the event in esi, `cmp eax,15h`, `ret 8`.
struct CInputEvent {
    long GetKind() const;                          // 0x00A03B40
    void MarkAsUsed() const;                       // 0x00A03B90
};
struct CProcessedInput {
    void RequestReboot();                          // 0x00A0D390
};
struct CInputProcessRebootGame {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessRebootGame::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (event.GetKind() == 0x15) {
        event.MarkAsUsed();
        processed.RequestReboot();
    }
}
