// NUISystem::CComponent::ChangePosition(const C2DVector& position, float time, long unused) -- set the target position; with a positive time start an interpolation from the current position, else jump there at once
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E9C0, 98 bytes (112 with padding). No callees.
//
// Shape: retail view of the Ego_r CComponent layout (as the landed CComponent rows 0x00530460 /
// 0x005309E0): the States map is 8 bytes smaller in retail, so Position sits at +0x34 (PDB +0x3c),
// TargetPosition +0x3c, InitialPosition +0x44, PositionTimeElapsed +0x98, PositionTime +0x9c.
// PositionTime is stored first as a raw dword (float copy through eax). The TargetPosition copy is
// written in EACH arm: VC hoists the common X store above the branch and keeps the Y store per arm
// (one copy before the `if` emits both words before the compare); `fcomp [0.0] ; test ah,41h ; jne` is
// `time > 0.0f` (0x0122DEDC is the 0.0f pool constant). With a time the current position becomes the
// initial position and the elapsed time restarts at 0; without one, initial and current position
// both snap to the argument. C2DVector is a plain two-float POD (integer moves). The third argument
// (`ret 0Ch`) is never read.
struct C2DVector {
    float X;
    float Y;
};

struct CComponent {
    unsigned char _pad[0x34];
    C2DVector Position;                     // +0x34
    C2DVector TargetPosition;               // +0x3c
    C2DVector InitialPosition;              // +0x44
    unsigned char _pad4c[0x98 - 0x4c];
    float PositionTimeElapsed;              // +0x98
    float PositionTime;                     // +0x9c
    void ChangePosition(const C2DVector& position, float time, long unused);
};

void CComponent::ChangePosition(const C2DVector& position, float time, long unused)
{
    PositionTime = time;
    if (time > 0.0f) {
        TargetPosition = position;
        InitialPosition = Position;
        PositionTimeElapsed = 0.0f;
    } else {
        TargetPosition = position;
        InitialPosition = position;
        Position = position;
    }
}
