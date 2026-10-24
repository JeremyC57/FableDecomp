// NUISystem::CComponent::ChangeColour(const CRGBColour& colour, float time, bool unused) -- set the target colour; with a positive time start a fade from the current colour, else snap to it
// Leaf: ?ChangeColour@CComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052EC60, 95 bytes (96 with padding). No callees.
// Vtable 0x0122F5D4 (NUISystem::CSprite, inherited from CComponent) slot 38 -- the slot the
// ChangeColourDelta row 0x005305A0 calls. Same shape as the landed ChangePosition 0x0052E9C0 with a
// 4-byte CRGBColour: retail view of the Ego_r CComponent layout (States map 8 bytes smaller), so
// Colour sits at +0x84 (PDB +0x8c), TargetColour +0x88, InitialColour +0x8c, ColourTimeElapsed +0xa8,
// ColourTime +0xac. ColourTime is stored first as a raw dword; `fcomp [0.0f] ; test ah,41h ; jne` is
// `time > 0.0f`. The third argument (`ret 0Ch`) is never read.
struct CRGBColour {
    unsigned char B, G, R, A;
};

struct CComponent {
    unsigned char _pad[0x84];
    CRGBColour Colour;                      // +0x84
    CRGBColour TargetColour;                // +0x88
    CRGBColour InitialColour;               // +0x8c
    unsigned char _pad90[0xa8 - 0x90];
    float ColourTimeElapsed;                // +0xa8
    float ColourTime;                       // +0xac
    void ChangeColour(const CRGBColour& colour, float time, bool unused);
};

void CComponent::ChangeColour(const CRGBColour& colour, float time, bool unused)
{
    ColourTime = time;
    if (time > 0.0f) {
        TargetColour = colour;
        InitialColour = Colour;
        ColourTimeElapsed = 0.0f;
    } else {
        TargetColour = colour;
        InitialColour = colour;
        Colour = colour;
    }
}