// Behaviour test for 00853320: src/00853320.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. NCameraModes::CScriptModuleBase::GetViewPosInFocalPlane(const C3DVector& pos, const C3DVector& dir)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NCameraModes::CScriptModuleBase::GetViewPosInFocalPlane(const C3DVector& pos, const C3DVector& dir) -- pos + dir, or, with a live origin thing, the point where the ray from pos along dir meets the camera plane (through CoordOrigin, normal Forward x Up) when it lies beyond pos + dir
// Retail: Fable.exe (TLC, VC7.1) @ 0x00853320, `ret 0Ch` (hidden result pointer, pos, dir).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (twice, the inline IS_THING_ALIVE), C3DVector::Normalise 0x00A14440.
// Layout: CScriptModuleBase PCamPoint +0x04, PThingOrigin +0x0c, CoordOrigin +0x14, CoordAxis +0x20 (CRightHandedSet: Up +0,
//   Forward +0xc, Ego_r.pdb); the camera module class is modelled locally (no shared header).
// Shape: the origin and the two axes are copied into locals in the order origin, forward, up (VC7.1 then keeps Forward
//   and Up.Z on the x87 stack and homes only Up.X/Up.Y and the origin in frame words, frame 0x30; a whole CRightHandedSet
//   copy or member references give other spills); the vector operators are __forceinline (left to the compiler they are
//   emitted out of line here).
// Leaf: GetViewPosInFocalPlane
#include <math.h>
#pragma intrinsic(fabs)
extern const float g_epsilon_0129ba3c;               // 0x0129BA3C (1e-4f)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    void Normalise();                                // 0x00A14440
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
inline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

struct CRightHandedSet {
    C3DVector Up;                                    // +0x00
    C3DVector Forward;                               // +0x0c
};

struct CBaseObject;
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                        // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    virtual void v00();
};
#define IS_THING_ALIVE(p) ((p).GetPItem() && !((const CThing*)(p).GetPItem())->m_dead)

namespace NCameraModes {

struct CScriptModuleBase {                           // vtable 0x01255F74
    virtual void v00();
    CIntelligentPointer_CThing PCamPoint;            // +0x04
    CIntelligentPointer_CThing PThingOrigin;         // +0x0c
    C3DVector                  CoordOrigin;          // +0x14
    CRightHandedSet            CoordAxis;            // +0x20
    C3DVector GetViewPosInFocalPlane(const C3DVector& pos, const C3DVector& dir);
};

C3DVector CScriptModuleBase::GetViewPosInFocalPlane(const C3DVector& pos, const C3DVector& dir)
{
    C3DVector result = pos + dir;
    if (IS_THING_ALIVE(PThingOrigin)) {
        C3DVector origin = CoordOrigin;
        C3DVector forward = CoordAxis.Forward;
        C3DVector up = CoordAxis.Up;
        C3DVector normal = CrossProduct(forward, up);
        if (normal.GetLengthSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c) {
            normal.Normalise();
            float denominator = DotProduct(normal, dir);
            if (fabs(denominator) > g_epsilon_0129ba3c) {
                float t = DotProduct(origin - pos, normal) / denominator;
                if (t > 1.0f - g_epsilon_0129ba3c)
                    result = pos + dir * t;
            }
        }
    }
    return result;
}

}

// ---- stubs and driver ----
#include <math.h>
extern const float g_epsilon_0129ba3c = 0.0001f;
static int g_normCalls = 0;
void C3DVector::Normalise() { ++g_normCalls; float l = sqrtf(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; }
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointer_CThing::v00() {}
void NCameraModes::CScriptModuleBase::v00() {}
static int g_pitemCalls = 0;
CBaseObject* CBaseIntelligentPointer::GetPItem() const { ++g_pitemCalls; return (CBaseObject*)m_pItem; }
static bool Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-4f; }
static bool NearV(const C3DVector& v, float x, float y, float z) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z); }
static void SetAxes(NCameraModes::CScriptModuleBase& m, float ux, float uy, float uz, float fx, float fy, float fz)
{
    m.CoordAxis.Up.X = ux; m.CoordAxis.Up.Y = uy; m.CoordAxis.Up.Z = uz;
    m.CoordAxis.Forward.X = fx; m.CoordAxis.Forward.Y = fy; m.CoordAxis.Forward.Z = fz;
}
int main()
{
    static unsigned char thingBytes[0x100];
    CThing* thing = (CThing*)thingBytes;
    NCameraModes::CScriptModuleBase m;
    m.PCamPoint.m_pItem = 0;
    m.PThingOrigin.m_pItem = 0;
    m.CoordOrigin = C3DVector(0.0f, 0.0f, 10.0f);
    SetAxes(m, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);       // normal = Forward x Up = +Z, plane z = 10
    C3DVector pos(1.0f, 2.0f, 0.0f), dir(0.0f, 0.0f, 2.0f);

    // 1: no origin thing -> pos + dir, nothing else evaluated
    g_pitemCalls = g_normCalls = 0;
    C3DVector r = m.GetViewPosInFocalPlane(pos, dir);
    CHECK(NearV(r, 1.0f, 2.0f, 2.0f) && g_pitemCalls == 1 && g_normCalls == 0);

    // 2: dead origin thing -> pos + dir
    m.PThingOrigin.m_pItem = thing; thing->m_dead = 1;
    g_pitemCalls = g_normCalls = 0;
    r = m.GetViewPosInFocalPlane(pos, dir);
    CHECK(NearV(r, 1.0f, 2.0f, 2.0f) && g_pitemCalls == 2 && g_normCalls == 0);

    // 3: live origin, plane beyond pos + dir -> the ray meets z = 10 at t = 5
    thing->m_dead = 0;
    g_pitemCalls = g_normCalls = 0;
    r = m.GetViewPosInFocalPlane(pos, dir);
    CHECK(NearV(r, 1.0f, 2.0f, 10.0f) && g_normCalls == 1);

    // 4: plane closer than pos + dir (t = 0.5 <= 1 - eps) -> pos + dir
    m.CoordOrigin = C3DVector(0.0f, 0.0f, 1.0f);
    r = m.GetViewPosInFocalPlane(pos, dir);
    CHECK(NearV(r, 1.0f, 2.0f, 2.0f));

    // 5: ray parallel to the plane (denominator 0) -> pos + dir
    C3DVector side(3.0f, 0.0f, 0.0f);
    m.CoordOrigin = C3DVector(0.0f, 0.0f, 10.0f);
    r = m.GetViewPosInFocalPlane(pos, side);
    CHECK(NearV(r, 4.0f, 2.0f, 0.0f));

    // 6: degenerate axes (parallel, zero normal) -> pos + dir, no normalise
    SetAxes(m, 1.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f);
    g_normCalls = 0;
    r = m.GetViewPosInFocalPlane(pos, dir);
    CHECK(NearV(r, 1.0f, 2.0f, 2.0f) && g_normCalls == 0);

    // 7: tilted plane: Up = +Y, Forward = (1,0,1)/sqrt2 -> normal along (1,0,-1); origin (5,0,0); ray from (0,0,0) along (1,0,0)
    //    plane x - z = 5 -> t = 5
    SetAxes(m, 0.0f, 1.0f, 0.0f, 0.70710678f, 0.0f, 0.70710678f);
    m.CoordOrigin = C3DVector(5.0f, 0.0f, 0.0f);
    r = m.GetViewPosInFocalPlane(C3DVector(0.0f, 0.0f, 0.0f), C3DVector(1.0f, 0.0f, 0.0f));
    CHECK(NearV(r, 5.0f, 0.0f, 0.0f));

    if (fails == 0) printf("00853320_TEST PASS\n");
    return fails;
}
