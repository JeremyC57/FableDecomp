// CCombatWheel::ReassignAttacker(CWorld*, CCombatantInfo*, CCombatantInfo*, const C3DVector*) -- give the attacker a fresh preferred attack dolly: the flat direction from the defender to the given position, or to the attacker turned by a random 0.25..0.75 turn; when it is not degenerate it is normalised and stored with a five-second timer
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E80F0, 383 bytes (385 incl. a padding byte). `ret 10h` (`this` unused).
// Callees: CThing::GetPosition 0x004C73D0, CWorld's random-seed accessor 0x0049D810 (catalogued
//          CVertexBufferWin32::DoSizeof), `__fastcall` GFFloatRandom(float, unsigned long*) 0x004978A0,
//          C3DVector::Rotate(const C3DVector&, float) 0x00A13C60, C3DVector::Normalise 0x00A14440 (catalogued
//          GetScaled), _ftol2 0x00BFEA70. Globals: 1e-4f 0x0129BA3C (read as a variable), 0.25f 0x0123078C,
//          5.0f 0x01261D64, the integer frame rate 0x01375550.
//
// Shape: each arm assigns the difference through the implicit operator= (the dead Z copy is kept before the
// `Z = 0` store); the squared length is `X*X + Y*Y + Z*Z` (VC7.1 emits Y first; the else arm folds Z = 0);
// the rotation axis is a C3DVector(0, 0, 1) temporary in the subtraction's dead slots.
// Layout: CCombatantInfo / CAttackingCombatantInfo of the landed copy ctor 0x006F4990: Parent +0,
// PreferredAttackDollySet +0x10, PreferredAttackDollyTimer +0x14, PreferredAttackDollyVector +0x18.
struct C2DVector {
    float X;
    float Y;
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    void Normalise();                                   // 0x00A14440
    void Rotate(const C3DVector& axis, float angle);    // 0x00A13C60
};
extern float g_Epsilon;                     // 0x0129BA3C (1e-4f)
extern long g_framesPerSecond;              // 0x01375550
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct CThingCreatureBase {
    const C3DVector& GetPosition() const;   // 0x004C73D0 (CThing::GetPosition)
};
struct CWorld {
    unsigned long* GetRandomSeed();         // 0x0049D810
};
struct CAttackingCombatantInfo {
    CThingCreatureBase* Attacked;           // +0x00
    long AttackerNumber;                    // +0x04
    long CombatWheelRingIndex;              // +0x08
    bool PreferredAttackDollySet;           // +0x0c
    long PreferredAttackDollyTimer;         // +0x10
    C2DVector PreferredAttackDollyVector;   // +0x14
    bool UsingCombatWheel;                  // +0x1c
};
struct CCombatantInfo {
    CThingCreatureBase* Parent;             // +0x00
    CAttackingCombatantInfo AttackerInfo;   // +0x04
};
class CCombatWheel {
public:
    void ReassignAttacker(CWorld* world, CCombatantInfo* attacker, CCombatantInfo* defender, const C3DVector* position);
};

void CCombatWheel::ReassignAttacker(CWorld* world, CCombatantInfo* attacker, CCombatantInfo* defender, const C3DVector* position)
{
    C3DVector dolly;
    if (position) {
        dolly = *position - defender->Parent->GetPosition();
        dolly.Z = 0.0f;
    } else {
        dolly = attacker->Parent->GetPosition() - defender->Parent->GetPosition();
        dolly.Z = 0.0f;
        if (dolly.GetLengthSquared() > g_Epsilon * g_Epsilon)
            dolly.Rotate(C3DVector(0.0f, 0.0f, 1.0f), GFFloatRandom(0.5f, world->GetRandomSeed()) + 0.25f);
    }
    if (dolly.GetLengthSquared() > g_Epsilon * g_Epsilon) {
        dolly.Normalise();
        attacker->AttackerInfo.PreferredAttackDollySet = true;
        attacker->AttackerInfo.PreferredAttackDollyTimer = (long)(g_framesPerSecond * 5.0f);
        attacker->AttackerInfo.PreferredAttackDollyVector.X = dolly.X;
        attacker->AttackerInfo.PreferredAttackDollyVector.Y = dolly.Y;
    }
}
