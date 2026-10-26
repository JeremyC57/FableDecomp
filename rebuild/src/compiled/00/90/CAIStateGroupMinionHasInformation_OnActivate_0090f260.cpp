// CAIStateGroup_MinionHasInformation::OnActivate() -- reset the wave/wander frame stamps and record the
// activation frame.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F260, length 34 bytes (+ int3 padding).
// Callee: GFGetCurrentFrame @ 0x0049D870 (__fastcall, no arguments).
// Members are the PDB names from rebuild/include/engine/CAIStateGroup_MinionHasInformation.h (LastFrameWaved
// +0x14, LastFrameWandered +0x18, FrameInitialised +0x1c, WavesWithoutCalling +0x20); the header flattens the
// base into pads and has no vtable, so the class is modelled locally with the same offsets.
long __fastcall GFGetCurrentFrame();        // 0x0049D870

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];               // +0x04..+0x13
};

struct CAIStateGroup_MinionHasInformation : CAIStateGroupBase {
    long LastFrameWaved;                    // +0x14
    long LastFrameWandered;                 // +0x18
    long FrameInitialised;                  // +0x1c
    long WavesWithoutCalling;               // +0x20
    virtual void OnActivate();
};

void CAIStateGroup_MinionHasInformation::OnActivate()
{
    LastFrameWaved = 0;
    LastFrameWandered = 0;
    FrameInitialised = GFGetCurrentFrame();
    WavesWithoutCalling = 0;
}