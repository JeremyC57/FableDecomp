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
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
#include <math.h>
#pragma intrinsic(fabs)
extern const float DELTA_NOTIONAL_ZERO;           // 0x0129BA3C (1e-4f)

#include "engine/C3DVector.h"
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
        if (normal.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
            normal.AccurateNormalise();
            float denominator = DotProduct(normal, dir);
            if (fabs(denominator) > DELTA_NOTIONAL_ZERO) {
                float t = DotProduct(origin - pos, normal) / denominator;
                if (t > 1.0f - DELTA_NOTIONAL_ZERO)
                    result = pos + dir * t;
            }
        }
    }
    return result;
}

}