// CTCLook::FrameUpdateOverriddenByParent() -- per-frame update while the parent overrides looking: a head state with bit 0 set re-centres the head unless bit 3 is also set; otherwise the head is forced to state 1 (OneFrameDelay on change, stop timer cancelled), re-centred and the eye angles zeroed; finally the body turn state is forced to 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753080, length 79 bytes.
// Callees: CTCLook::CentreHead @ 0x007528A0, CTCLook::ChangeEyeAngles(float, float) @ 0x00752200 (pushed 0, 0 = 0.0f, 0.0f).
// Layout (local model, no engine header): HeadTurnState +0x48, BodyTurnState +0x4c, TimeToStopLooking +0xb4,
// OneFrameDelay +0xb8 (PDB offsets minus 4 after the 12-byte retail vector TurningAnims at +0x8c).
// Shape: the constant 1 is CSE'd into ebx and shared by the bit test (`test bl,al`), both state compares and the byte store;
// the inline setters are the same change-tested forms as in StopLooking (0x00751B80).
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
    void CentreHead();                                       // 0x007528A0
    void ChangeEyeAngles(float angleXY, float angleYZ);      // 0x00752200
    void FrameUpdateOverriddenByParent();
};

void CTCLook::FrameUpdateOverriddenByParent()
{
    long state = HeadTurnState;
    if (state & 1) {
        if (!(state & 8))
            CentreHead();
    } else {
        ChangeHeadTurnState(1);
        TimeToStopLooking = -1;
        CentreHead();
        ChangeEyeAngles(0.0f, 0.0f);
    }
    SetBodyTurnState(1);
}