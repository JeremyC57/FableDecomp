// NCameraModes::CScriptModuleBase::GetViewPosInFocalPlane(const C3DVector& pos, const C3DVector& dir) -- pos + dir, or, with a live origin thing, the point where the ray from pos along dir meets the camera plane (through CoordOrigin, normal Forward x Up) when it lies beyond pos + dir
// Grade: DIFFER(438v458) -- register/x87 allocation: retail keeps this in ebx (pos esi, dir edi) and copies only Up.X/Up.Y and CoordOrigin into frame words (0x30 frame), loading Forward straight from the member; ours puts this in esi and copies a whole CRightHandedSet (0x3c frame) or forwards everything; cross/dot/sum operand order is canonical (source order has no effect)
// Leaf: GetViewPosInFocalPlane
#include <math.h>
#pragma intrinsic(fabs)
extern const float g_epsilon_0129ba3c;               // 0x0129BA3C (1e-4f)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
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
        CRightHandedSet axis = CoordAxis;
        C3DVector normal = CrossProduct(axis.Forward, axis.Up);
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
