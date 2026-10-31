// CEngineEnvironment::WorldFrameUpdate(bool force) -- refresh the wind: new normalised direction (previous one when the new is shorter than 0.1), speed clamped to 0.4, previous/next wind and speed shifted, then a new wind-skew sample 0.75*H0 + 0.9*(H0-H1) + wind*speed pushed onto the 64-entry history
// Grade: DIFFER(475v487) -- the history part is one expression and the wind term is added member by member (`skew.X +=
//   wind.X * speed` ...): diff 46 -> 21 lines; left: retail computes all three wind*speed products first (Z product
//   spilled to [esp+38h]) and then adds them into the partial in place, keeping the partial Z on the x87 stack; ours
//   interleaves product and add per member. Tried this pass: a named gust vector added member by member (both operand
//   orders, 495), a three-argument C3DVector of the sums (495)
// Retail: Fable.exe @ 0x00B8CCF0, 487 bytes, `ret 4`. Callees: env provider slot 1 / env slot 1 (wind record), C3DVector::Normalise 0x00A14440. this = engine/CEngineEnvironment.h (offsets agree with Ego_r.pdb).
#include <math.h>
#pragma intrinsic(sqrt)
#include "engine/CEngineEnvironment.h"
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    void Normalise();                              // 0x00A14440
};
struct SWindRecord {
    unsigned char _pad[0x14];
    C3DVector     Direction;                       // +0x14
    float         Strength;                        // +0x20
};
struct CEnvironment {
    virtual void v00();
    virtual SWindRecord* GetWind();                // slot 1
};
struct CEnvironmentProvider {
    virtual void v00();
    virtual CEnvironment* GetCurrentEnvironment(); // slot 1
};
struct SEngineState_01436e8c {
    unsigned char         _pad[0x198];
    CEnvironmentProvider* m_pEnvironmentProvider;  // +0x198
};
extern SEngineState_01436e8c* g_pEngineState_01436e8c;   // 0x01436E8C

static __forceinline C3DVector* WindSkewHistory(CEngineEnvironment* self) { return (C3DVector*)self->WindSkewHistory; }
static __forceinline C3DVector& NextWindVector(CEngineEnvironment* self) { return *(C3DVector*)&self->NextWindVector_X; }
static __forceinline C3DVector& PreviousWindVector(CEngineEnvironment* self) { return *(C3DVector*)&self->PreviousWindVector_X; }

void __fastcall WorldFrameUpdate(CEngineEnvironment* self, int, bool force)
{
    if (!force && !self->EnvironmentalEffectsAlwaysUpdate)
        return;
    CEnvironment* environment = g_pEngineState_01436e8c->m_pEnvironmentProvider->GetCurrentEnvironment();
    C3DVector wind = environment->GetWind()->Direction;
    if (wind.GetLength() < 0.1f)
        wind = PreviousWindVector(self);
    wind.Normalise();
    float speed = environment->GetWind()->Strength;
    if (!(speed < 0.4f))
        speed = 0.4f;
    PreviousWindVector(self) = NextWindVector(self);
    NextWindVector(self) = wind;
    self->PreviousWindSpeed = self->NextWindSpeed;
    self->NextWindSpeed = speed;
    C3DVector skew = WindSkewHistory(self)[0] * 0.75f + (WindSkewHistory(self)[0] - WindSkewHistory(self)[1]) * 0.9f;
    skew.X += wind.X * speed;
    skew.Y += wind.Y * speed;
    skew.Z += wind.Z * speed;
    for (int i = 63; i > 0; --i)
        WindSkewHistory(self)[i] = WindSkewHistory(self)[i - 1];
    WindSkewHistory(self)[0] = skew;
}
