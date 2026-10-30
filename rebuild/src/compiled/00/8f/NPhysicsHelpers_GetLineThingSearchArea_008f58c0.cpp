// NPhysicsHelpers::GetLineThingSearchArea(const C3DVector&, const C3DVector&, float) -- integer box around the segment's midpoint, half-size = segment length + 2 * radius
// Leaf: ?GetLineThingSearchArea@
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the PDB ctor C2DBoxI(const C2DCoordI&, long) written as two corner copies
//   of the centre adjusted in place (tl -= r, br += r) and stored BR first gives retail's copy-then-add tail.
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
    {
        C2DCoordI tl = centre;
        C2DCoordI br = centre;
        tl.X -= radius;
        tl.Y -= radius;
        br.X += radius;
        br.Y += radius;
        BRX = br.X;
        BRY = br.Y;
        TLX = tl.X;
        TLY = tl.Y;
    }
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