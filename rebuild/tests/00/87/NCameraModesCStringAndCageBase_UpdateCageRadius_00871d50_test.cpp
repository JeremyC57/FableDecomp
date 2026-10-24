// Behaviour test for 0x00871D50 NCameraModes::CStringAndCageBase::UpdateCageRadius(C3DVector&) const: a position inside
// the cage radius (2D distance from the observed thing) is pushed out to the radius along the same XY direction (Z set to
// the centre's Z) and true is returned; a position on or outside the radius is untouched and false is returned.
#include <stdio.h>
#include <math.h>
struct C3DVector {
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

static C3DVector g_centre(10.0f, 20.0f, 7.0f);
static CThingGameObject g_thing;
const C3DVector& CThing::GetPosition() const { return g_centre; }
void NCameraModes::CBase::v00() { }
const CThingGameObject* NCameraModes::CBase::GetObservedThing() const { return &g_thing; }

static int Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-4f; }

int main()
{
    int ok = 1;
    NCameraModes::CStringAndCageBase cam;
    cam.CurrentCageRadius = 5.0f;
    C3DVector pos(13.0f, 20.0f, 99.0f);             // 3 away on +X: pushed to 5
    if (!cam.UpdateCageRadius(pos)) ok = 0;
    if (!Near(pos.X, 15.0f) || !Near(pos.Y, 20.0f) || !Near(pos.Z, 7.0f)) ok = 0;
    C3DVector pos2(10.0f, 18.0f, 1.0f);             // 2 away on -Y
    if (!cam.UpdateCageRadius(pos2)) ok = 0;
    if (!Near(pos2.X, 10.0f) || !Near(pos2.Y, 15.0f)) ok = 0;
    C3DVector pos3(13.0f, 24.0f, 1.0f);             // exactly 5 away: untouched
    if (cam.UpdateCageRadius(pos3)) ok = 0;
    if (pos3.X != 13.0f || pos3.Y != 24.0f || pos3.Z != 1.0f) ok = 0;
    C3DVector pos4(30.0f, 20.0f, 1.0f);             // far: untouched
    if (cam.UpdateCageRadius(pos4) || pos4.X != 30.0f) ok = 0;
    printf("00871d50_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}