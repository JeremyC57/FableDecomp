// NPhysicsHelpers::GetLineThingSearchArea(const C3DVector&, const C3DVector&, float) -- integer box around the segment's midpoint, half-size = segment length + 2 * radius
// Leaf: ?GetLineThingSearchArea@
// Grade: DIFFER(164v163) -- tail only: retail computes both TL corners into copies (`mov ecx,edi ; mov edx,ebx ; add
//   edi,eax ; sub ecx,eax ; sub edx,eax ; add ebx,eax`) and stores BRX, BRY, TLX, TLY; the lane emits `sub ecx,eax ; mov
//   [esi],ecx ; lea ecx,[eax+edi]` (TL first). Everything before (midpoint X/Y via 0.5f, the line vector, the three _ftol
//   calls, the Z, Y, X sum of squares, radius + radius) matches. Tried: 4-long ctor (164), C2DBoxI(C2DCoordI, C2DCoordI)
//   from two temporaries (161), named corner locals (161), PDB ctor C2DBoxI(const C2DCoordI&, long) (164, this file).
// Retail 0x008F58C0, 0xA3 bytes, __fastcall (hidden C2DBoxI result in ecx, lineStart in edx, lineEnd and radius on the
//   stack, `ret 8`). Manifest name matches the PDB name set
//   (`?GetLineThingSearchArea@NPhysicsHelpers@@YI?AVC2DBoxI@@ABVC3DVector@@0M@Z`).
// Callees: _ftol2 @ 0x00BFEA70 (three times); 0.5f @ 0x0122F59C.
// Layout: C2DBoxI = TLX, TLY, BRX, BRY (longs, struct_layouts_egor.tsv); PDB ctors include (long x4), (C2DCoordI,
//   C2DCoordI), (C2DCoordI, long) and a user copy ctor.
// Next idea: a named `C2DBoxI box(centre, halfSize); return box;` through the PDB's user copy ctor, or a ctor body that
//   assigns BRX/BRY before TLX/TLY.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const
    {
        C3DVector result;
        result.X = X - v.X;
        result.Y = Y - v.Y;
        result.Z = Z - v.Z;
        return result;
    }
    C3DVector operator*(float f) const
    {
        C3DVector result;
        result.X = X * f;
        result.Y = Y * f;
        result.Z = Z * f;
        return result;
    }
    float GetLength() const
    {
        return (float)sqrt(X * X + Y * Y + Z * Z);
    }
};

class C2DCoordI {
public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};

class C2DBoxI {
public:
    long TLX, TLY;                              // +0x00 / +0x04
    long BRX, BRY;                              // +0x08 / +0x0c
    C2DBoxI(const C2DCoordI& centre, long radius)
        : TLX(centre.X - radius), TLY(centre.Y - radius), BRX(centre.X + radius), BRY(centre.Y + radius) {}
};

namespace NPhysicsHelpers {
C2DBoxI __fastcall GetLineThingSearchArea(const C3DVector& lineStart, const C3DVector& lineEnd, float radius)
{
    C3DVector centre = (lineStart + lineEnd) * 0.5f;
    C3DVector line = lineEnd - lineStart;
    C2DCoordI centreCoord((long)centre.X, (long)centre.Y);
    return C2DBoxI(centreCoord, (long)(line.GetLength() + radius * 2.0f));
}
}
