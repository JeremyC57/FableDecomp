// NCameraModes::CScriptModuleBase::GetViewPosInFocalPlane(const C3DVector& pos, const C3DVector& dir) -- pos + dir, or, with a live origin thing, the point where the ray from pos along dir meets the camera plane (through CoordOrigin, normal Forward x Up) when it lies beyond pos + dir
// Retail: Fable.exe (TLC, VC7.1) @ 0x00853320, `ret 0Ch` (hidden result pointer, pos, dir).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (twice, the inline IS_THING_ALIVE), C3DVector::AccurateNormalise @ 0x00A14440.
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

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    C3DVector& AccurateNormalise();                                // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
inline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

class CRightHandedSet { public:
    C3DVector Up;                                    // +0x00
    C3DVector Forward;                               // +0x0c
};

class CBaseObject;
class CThing { public:
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

class CScriptModuleBase { public:                           // vtable 0x01255F74
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
            normal.AccurateNormalise();
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