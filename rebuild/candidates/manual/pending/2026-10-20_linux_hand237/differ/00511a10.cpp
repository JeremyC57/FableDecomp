// Grade: DIFFER(241v252) -- lane gap: the radius is rounded up by a bare dword fistp ceil helper in an ebp frame (inline asm in the original, asm_bake family); plain C++ goes through ceil/__ftol2
// CThingFilter_OverlappingArea::operator()(CThing* thing) const -- true when the thing's bounding centre lies in the filter area grown by the thing's radius (rounded up)
// Leaf: operator()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00511A10, length 250 bytes, `ret 4`, ebp frame.
// (A combined-filter instantiation: the first filter @ 0x006A7C60 on this+0, then the area test with the area at this+4.)
// Callees: CTCPhysicsBase::GetBoundingInfo(C3DVector*, float*) @ 0x006AF640, __ftol2 @ 0x00BFEA70 (six times).
// Shape: the radius is rounded up by a bare `fistp` helper (fld r ; fadd 0.5 ; fistp ; fild ; fsub 1.0 ; compare the
// float bits ; dec) -- an inline-asm CeilToInt in the original (asm_bake family, like 0x0052F440): the ebp frame and the
// dword fistp cannot come from plain C++. This source states the logic with a C ceil.
#include <math.h>
struct C3DVector { float X, Y, Z; };
struct C2DBoxI { long X0, Y0, X1, Y1; };
class CTCPhysicsBase {
public:
    void GetBoundingInfo(C3DVector* centre, float* radius);    // 0x006AF640
};
class CThing;
class CThingFilter {
public:
    bool operator()(CThing* thing) const;                      // 0x006A7C60
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                                 // +0x60
};
class CThingFilter_OverlappingArea {
public:
    CThingFilter*  First;                                      // +0x00
    const C2DBoxI* Area;                                       // +0x04
    bool operator()(CThing* thing) const;
};

bool CThingFilter_OverlappingArea::operator()(CThing* thing) const
{
    if (!(*First)(thing) || !thing->PhysicsTC)
        return false;
    C3DVector centre;
    float radius;
    thing->PhysicsTC->GetBoundingInfo(&centre, &radius);
    float grow = (float)(long)ceil(radius);
    C2DBoxI box = *Area;
    long maxX = (long)(box.X1 + grow);
    long minX = (long)(box.X0 - grow);
    long minY = (long)(box.Y0 - grow);
    long maxY = (long)(box.Y1 + grow);
    long x = (long)centre.X;
    long y = (long)centre.Y;
    return x >= minX && x < maxX && y >= minY && y < maxY;
}
