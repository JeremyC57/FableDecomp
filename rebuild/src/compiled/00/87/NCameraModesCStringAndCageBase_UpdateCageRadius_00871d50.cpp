// NCameraModes::CStringAndCageBase::UpdateCageRadius(C3DVector&) const -- when the camera position is inside the cage radius (2D) around the observed thing, push it out onto the radius and return true
// Leaf: ?UpdateCageRadius@CStringAndCageBase@NCameraModes@@QBE_NAAVC3DVector@@@Z
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- squared XY distance summed in statement order (`distSq = offset.X * offset.X; distSq += offset.Y * offset.Y;`) so VC7.1 squares the spilled X first as retail
// Retail 0x00871D50, 0xE3 bytes, __fastcall `ret 4`, bool in al. PDB (Ego_r):
// ?UpdateCageRadius@CStringAndCageBase@NCameraModes@@ABE_NAAVC3DVector@@@Z (private const). Caller: the landed
// CStringAndCageBase::Update @ 0x008728C0 (`if (!UpdateStringLength(goTo)) UpdateCageRadius(goTo);` -- it names the pair
// UpdatePositionWithVelocity 0x008722D0 / UpdatePosition 0x00871D50, descriptively).
// Callees: NCameraModes::CBase::GetObservedThing() const @ 0x0084F1B0, CThing::GetPosition() const @ 0x004C73D0.
// Pools: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Layout (retail = Ego_r - 0x24 for this class, as the landed UpdateZVars 0x00872050): CurrentCageRadius +0xB8.
// Shape: the observed thing's position is copied into a C3DVector local through a memberwise copy constructor (X/Y stay
// on the x87 stack, Z is copied as a dword); the offset is a C3DVector local (pos - centre in X/Y, Z 0.0f) so its X
// spills to its own frame slot and the 2D length takes the dead `pos` argument slot (two float locals swap those
// homes and add a frame word); `pos = offset * CurrentCageRadius / dist + centre` through the inline operators
// (operator/ as `inv = 1.0f / s` times each member; operator+ as `X + r.X`); `dist < CurrentCageRadius` gives `test ah,5 ; jp`.
// Tried: float dx/dy locals (a), the sum written Y-first (b, same as a), the C3DVector offset (c, best), an inline
// C3DVector::GetMagnitude2D() (d, 217 bytes).
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator+(const C3DVector& r) const { return C3DVector(X + r.X, Y + r.Y, Z + r.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator/(float s) const { float inv = 1.0f / s; return C3DVector(X * inv, Y * inv, Z * inv); }
};
class CThing {
public:
    const C3DVector& GetPosition() const;                  // 0x004C73D0
};
class CThingGameObject : public CThing { };
namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    unsigned char _pad04[0xa4 - 0x04];
    const CThingGameObject* GetObservedThing() const;      // 0x0084F1B0
};
class CStringAndCageBase : public CBase {
public:
    float DesiredHeightOffset;                             // +0xa4
    float DesiredStringLength;                             // +0xa8
    float DesiredCageRadius;                               // +0xac
    float CurrentHeightOffset;                             // +0xb0
    float CurrentStringLength;                             // +0xb4
    float CurrentCageRadius;                               // +0xb8
    bool UpdateCageRadius(C3DVector& pos) const;
};
bool CStringAndCageBase::UpdateCageRadius(C3DVector& pos) const
{
    C3DVector centre = GetObservedThing()->GetPosition();
    C3DVector offset(pos.X - centre.X, pos.Y - centre.Y, 0.0f);
    float distSq = offset.X * offset.X;
    distSq += offset.Y * offset.Y;
    float dist = (float)sqrt(distSq);
    if (dist < CurrentCageRadius) {
        pos = offset * CurrentCageRadius / dist + centre;
        return true;
    }
    return false;
}
}