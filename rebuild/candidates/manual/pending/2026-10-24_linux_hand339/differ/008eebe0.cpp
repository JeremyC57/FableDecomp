// Mass::BackplaneFriction(float friction, float dt) -- friction against the backplane: shrinks the linear velocity's length by `friction` (to zero when it is smaller) and the angular velocity's magnitude by friction / dt (to zero when smaller)
// Grade: DIFFER(129v131) -- only the dead-spin cleanup in the angular "1 - angular/spin" arm: ours fstp st(1), retail fxch st(1) ; fstp st(0); the linear part and everything else match
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EEBE0, 131 bytes, `ret 8`. Layout from struct_layouts_egor.tsv (Mass: Vel +0xC, AVel +0x1C); name from FableWin.pdb ?BackplaneFriction@Mass@@QAEXMM@Z.
// Tried: if/else scale local, ternary, inline FrictionScale helper (by value / const-ref friction, >-first, early-return, default-0 local), fabs re-read, separate angular scale, explicit tail-duplicated arms.
// Leaf: ?BackplaneFriction@Mass
#include <math.h>
struct C2DVector {
    float X;                                 // +0x00
    float Y;                                 // +0x04
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
    C2DVector& operator*=(float s) { X *= s; Y *= s; return *this; }
};

struct Mass {                                // vtable 0x01288230, 0x24 bytes
    C2DVector Pos;                           // +0x04
    C2DVector Vel;                           // +0x0C
    float IMass;                             // +0x14
    float APos;                              // +0x18
    float AVel;                              // +0x1C
    float AIMass;                            // +0x20
    virtual ~Mass() { }
    void BackplaneFriction(float friction, float dt);
};

static inline float FrictionScale(float magnitude, float friction)
{
    return magnitude <= friction ? 0.0f : 1.0f - friction / magnitude;
}

void Mass::BackplaneFriction(float friction, float dt)
{
    Vel *= FrictionScale(Vel.GetLength(), friction);
    AVel *= FrictionScale((float)fabs(AVel), friction / dt);
}
