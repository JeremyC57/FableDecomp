// CActionInputControl::GetDirection() -- quantise the control direction of a movement action into 4 directions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040ED10, length 144 bytes.
// Globals: the shared float epsilon @ 0x0129BA3C (1e-4f) and the 0.0f constant @ 0x0122DEDC.
//
// Shape: a direction counts only when its squared length exceeds epsilon squared -- the epsilon is a global
// float read twice (`fld [eps] ; fmul [eps]`, no folding), the components stay on the x87 stack for the
// squares -- and only for game actions 0x3C and 0x72. Then +X -> 4, -X -> 3, +Y -> 1, -Y -> 2 (each
// component re-read per test: `> 0` is `test ah,41h ; jne`, `< 0` is `test ah,5 ; jp`), else 0. No engine
// header exists; members carry the PDB names (GameAction +0x00 .. ControlDirection +0x14).

extern const float g_floatEpsilon;              // 0x0129BA3C (1e-4f)
extern const float g_floatZero;                 // 0x0122DEDC (0.0f)

struct C2DVector {
    float X, Y;
};

struct CActionInputControl {
    long GameAction;                            // +0x00 EGameAction
    long ControllerType;                        // +0x04 EControllerType
    long KeyboardKey;                           // +0x08 EInputKey
    long XboxButton;                            // +0x0c EXboxControllerButton
    long MouseButton;                           // +0x10 EMouseButtonControl
    C2DVector ControlDirection;                 // +0x14
    long GetDirection() const;
};

long CActionInputControl::GetDirection() const
{
    if (ControlDirection.X * ControlDirection.X + ControlDirection.Y * ControlDirection.Y
            > g_floatEpsilon * g_floatEpsilon
        && (GameAction == 0x3C || GameAction == 0x72)) {
        if (ControlDirection.X > g_floatZero)
            return 4;
        if (ControlDirection.X < g_floatZero)
            return 3;
        if (ControlDirection.Y > g_floatZero)
            return 1;
        if (ControlDirection.Y < g_floatZero)
            return 2;
    }
    return 0;
}