// CWorldMap::ClipLinePointsToMapBox(const C2DBoxI& box, C3DVector& point, const C3DVector& towards) const -- when `point` lies outside the box, slide it along the line towards `towards` onto each box side plane it is beyond (x = 0, y = 0, x = width, y = height)
// Leaf: ?ClipLinePointsToMapBox@CWorldMap@
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local free operator- removed (the header member operator- is the same inline member-wise computation).
// Naming wave 18: g_InvSqrtMantissaTable is GFastOneOverSqrtTable (?GFastOneOverSqrtTable@@3PAKA); ChatGPT pass 10 audit, raw Ego_r public.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063BBC0, `ret 0xc`, esp frame; `this` unused.
// Callees: _ftol 0x00BFEA70, CPlane::Initialise(const C3DVector&, const C3DVector&, const C3DVector&) 0x00A42170,
//   CPlane::GetIntersectionWithLine(const C3DVector&, const C3DVector&, C3DVector*) const 0x00A422C0; fast inverse-sqrt
//   table 0x013CE558 (3.0f 0x0122DED4, 0.5f 0x0122F59C).
// Layout: C2DBoxI {TLX, TLY, BRX, BRY}; CPlane {Normal, Distance} (landed 0x00A422C0).
#include "fable_compiler.h"
extern unsigned long GFastOneOverSqrtTable[128];   // 0x013CE558
FABLE_FORCEINLINE float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}

#include "engine/C3DVector.h"

class C2DBoxI
{ public:
    long TLX;
    long TLY;
    long BRX;
    long BRY;
};

class CPlane
{ public:
    C3DVector Normal;
    float     Distance;
    bool Initialise(const C3DVector& a, const C3DVector& b, const C3DVector& c);                       // 0x00A42170
    bool GetIntersectionWithLine(const C3DVector& point, const C3DVector& direction, C3DVector* out) const; // 0x00A422C0
};

class CWorldMap
{
public:
    virtual ~CWorldMap();
    void ClipLinePointsToMapBox(const C2DBoxI& box, C3DVector& point, const C3DVector& towards) const;
};

void CWorldMap::ClipLinePointsToMapBox(const C2DBoxI& box, C3DVector& point, const C3DVector& towards) const
{
    long x = (long)point.X;
    long y = (long)point.Y;
    if (x >= box.TLX && x < box.BRX && y >= box.TLY && y < box.BRY)
        return;
    C3DVector start = point;
    C3DVector dir = towards - point;
    float inv = FastInvSqrt(dir.X * dir.X + dir.Y * dir.Y + dir.Z * dir.Z);
    dir.X *= inv;
    dir.Y *= inv;
    dir.Z *= inv;
    CPlane plane;
    if (point.X < 0.0f) {
        plane.Initialise(C3DVector(0.0f, 0.0f, 0.0f), C3DVector(0.0f, (float)(box.BRY - box.TLY), 0.0f), C3DVector(0.0f, 0.0f, 1.0f));
        plane.GetIntersectionWithLine(start, dir, &point);
    }
    if (point.Y < 0.0f) {
        plane.Initialise(C3DVector(0.0f, 0.0f, 0.0f), C3DVector((float)(box.BRX - box.TLX), 0.0f, 0.0f), C3DVector(0.0f, 0.0f, 1.0f));
        plane.GetIntersectionWithLine(start, dir, &point);
    }
    if ((float)(box.BRX - box.TLX) < point.X) {
        plane.Initialise(C3DVector((float)(box.BRX - box.TLX), 0.0f, 0.0f), C3DVector((float)(box.BRX - box.TLX), (float)(box.BRY - box.TLY), 0.0f), C3DVector((float)(box.BRX - box.TLX), 0.0f, 1.0f));
        plane.GetIntersectionWithLine(start, dir, &point);
    }
    if ((float)(box.BRY - box.TLY) < point.Y) {
        plane.Initialise(C3DVector(0.0f, (float)(box.BRY - box.TLY), 0.0f), C3DVector((float)(box.BRX - box.TLX), (float)(box.BRY - box.TLY), 0.0f), C3DVector(0.0f, (float)(box.BRY - box.TLY), 1.0f));
        plane.GetIntersectionWithLine(start, dir, &point);
    }
}