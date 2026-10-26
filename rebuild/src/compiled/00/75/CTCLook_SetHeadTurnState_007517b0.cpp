// CTCLook::SetHeadTurnState(long state) -- cancel the stop-looking timer (TimeToStopLooking = -1) and, when the head turn state changes, store it and raise OneFrameDelay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007517B0, length 32 bytes. No callees.
// Layout: PDB CTCLook (0xd4) with the retail 12-byte std::vector TurningAnims at +0x8c, so every member after it sits
// 4 bytes below its PDB offset: HeadTurnState +0x48, TimeToStopLooking +0xb4, OneFrameDelay +0xb8. No engine header exists
// for CTCLook, so the members this body touches are modelled locally. Shape: the -1 store is scheduled between the compare
// and its branch; the change test is the usual `if (member != value) { member = value; flag = true; }`.
class CTCLook { public:
    unsigned char _pad_0x00[0x48];
    long          HeadTurnState;                 // +0x48
    long          BodyTurnState;                 // +0x4c
    unsigned char _pad_0x50[0xb4 - 0x50];
    long          TimeToStopLooking;             // +0xb4 (PDB +0xb8)
    bool          OneFrameDelay;                 // +0xb8 (PDB +0xbc)
    void SetHeadTurnState(long state);
};

void CTCLook::SetHeadTurnState(long state)
{
    TimeToStopLooking = -1;
    if (HeadTurnState != state) {
        HeadTurnState = state;
        OneFrameDelay = true;
    }
}