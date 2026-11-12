// CActionInputControl::GetDirection() -- quantise the control direction of a movement action into 4 directions
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040ED10, length 144 bytes.
// Globals: the shared float epsilon @ 0x0129BA3C (1e-4f) and the 0.0f constant @ 0x0122DEDC.
//
// Shape: a direction counts only when its squared length exceeds epsilon squared -- the epsilon is a global
// float read twice (`fld [eps] ; fmul [eps]`, no folding), the components stay on the x87 stack for the
// squares -- and only for game actions 0x3C and 0x72. Then +X -> 4, -X -> 3, +Y -> 1, -Y -> 2 (each
// component re-read per test: `> 0` is `test ah,41h ; jne`, `< 0` is `test ah,5 ; jp`), else 0. No engine
// header exists; members carry the PDB names (GameAction +0x00 .. ControlDirection +0x14).

extern const float DELTA_NOTIONAL_ZERO;       // 0x0129BA3C (1e-4f)

class C2DVector { public:
    float X, Y;
};

class CActionInputControl { public:
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
            > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO
        && (GameAction == 0x3C || GameAction == 0x72)) {
        if (ControlDirection.X > 0.0f)
            return 4;
        if (ControlDirection.X < 0.0f)
            return 3;
        if (ControlDirection.Y > 0.0f)
            return 1;
        if (ControlDirection.Y < 0.0f)
            return 2;
    }
    return 0;
}