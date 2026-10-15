// CTCLook::StopLooking(long timeToStop) -- force the body turn state to 1; with no delay, also switch the head turn state to 1 (OneFrameDelay on change) and cancel TimeToStopLooking (-1), otherwise just arm TimeToStopLooking
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751B80, length 59 bytes. No callees.
// Layout (local model, no engine header): HeadTurnState +0x48, BodyTurnState +0x4c, TimeToStopLooking +0xb4,
// OneFrameDelay +0xb8 (PDB offsets minus 4 after the 12-byte retail vector TurningAnims at +0x8c).
// Shape: the constant 1 lives in eax for both turn-state compares and the OneFrameDelay byte store (`mov [..],al`), so
// both setters are inline members with the change test (`if (state != value) state = value;`); the -1 store
// follows the head-state change (unlike SetHeadTurnState 0x007517B0, which stores it first), so it is a separate statement.
struct CTCLook {
    unsigned char _pad_0x00[0x48];
    long          HeadTurnState;                 // +0x48
    long          BodyTurnState;                 // +0x4c
    unsigned char _pad_0x50[0xb4 - 0x50];
    long          TimeToStopLooking;             // +0xb4 (PDB +0xb8)
    bool          OneFrameDelay;                 // +0xb8 (PDB +0xbc)
    void ChangeHeadTurnState(long state)
    {
        if (HeadTurnState != state) {
            HeadTurnState = state;
            OneFrameDelay = true;
        }
    }
    void SetBodyTurnState(long state)
    {
        if (BodyTurnState != state)
            BodyTurnState = state;
    }
    void StopLooking(long timeToStop);
};

void CTCLook::StopLooking(long timeToStop)
{
    SetBodyTurnState(1);
    if (timeToStop == 0) {
        ChangeHeadTurnState(1);
        TimeToStopLooking = -1;
    } else
        TimeToStopLooking = timeToStop;
}
