// COrbitState::Initialise(const COrbitSettings& settings, unsigned long* seed) -- start an orbit: randomised start angle and rotate-speed offset (each +-its random range), zero target/next positions, then step the angle back by two frames of rotation
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B161A0, length 105 bytes (padded to 112).
// Callees: GFFloatRandom(float range, unsigned long* seed) @ 0x004978A0 (__fastcall: seed in ecx, range on the stack), twice.
// Globals: 1.0f @ 0x0122DED8.
// Layout: PDB COrbitState (0x44): NextAngle +0, RotateSpeedRandomValue +4, FromPosition +8, ToPosition +0x14,
// NextPosition +0x20, FromTangent +0x2c, ToTangent +0x38; COrbitSettings (0x30): RotateSpeed +0x20, RotateStart +0x24,
// RotateSpeedRandom +0x28, RotateStartRandom +0x2c.
// Shape: the start angle is copied as a dword then accumulated; each position is cleared by an inline
// C3DVector::Zero() holding a chained `X = Y = Z = 0` (stores run from the highest address down through one zero
// register); written inline in the body the zero stores are scheduled around the second fstp.
struct C3DVector {
    float X, Y, Z;
    void Zero() { X = Y = Z = 0.0f; }
};
struct COrbitSettings {
    long  Type;                             // +0x00
    bool  Enabled;                          // +0x04
    float Radius;                           // +0x08
    float Expand;                           // +0x0c
    bool  CycleFlag;                        // +0x10
    float CycleTime;                        // +0x14
    float SqueezeScale;                     // +0x18
    float SqueezeAngle;                     // +0x1c
    float RotateSpeed;                      // +0x20
    float RotateStart;                      // +0x24
    float RotateSpeedRandom;                // +0x28
    float RotateStartRandom;                // +0x2c
};

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct COrbitState {
    float     NextAngle;                    // +0x00
    float     RotateSpeedRandomValue;       // +0x04
    C3DVector FromPosition;                 // +0x08
    C3DVector ToPosition;                   // +0x14
    C3DVector NextPosition;                 // +0x20
    C3DVector FromTangent;                  // +0x2c
    C3DVector ToTangent;                    // +0x38
    void Initialise(const COrbitSettings& settings, unsigned long* seed);
};

void COrbitState::Initialise(const COrbitSettings& settings, unsigned long* seed)
{
    NextAngle = settings.RotateStart;
    NextAngle += (GFFloatRandom(2.0f, seed) - 1.0f) * settings.RotateStartRandom;
    RotateSpeedRandomValue = (GFFloatRandom(2.0f, seed) - 1.0f) * settings.RotateSpeedRandom;
    ToPosition.Zero();
    NextPosition.Zero();
    NextAngle -= (settings.RotateSpeed + RotateSpeedRandomValue) * 2.0f;
}