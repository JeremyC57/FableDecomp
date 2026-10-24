// C2DBoxF::C2DBoxF(const C2DVector& topLeft, const C2DExtentsF& extents) -- box from its top-left corner and its size
// Leaf: ??0C2DBoxF@@QAE@ABVC2DVector@@ABVC2DExtentsF@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041A7E0, length 39 bytes (row padded to 48), `ret 8`.
// Name: Ego_r.pdb `??0C2DBoxF@@QAE@ABVC2DVector@@ABVC2DExtentsF@@@Z`. The catalog's "operator+" is a propagated label:
// the body is a thiscall constructor (`this` in ecx returned in eax, two reference arguments) storing all four
// floats of the 16-byte box; callers 0x006489B5, 0x0065086E, 0x00650A97, 0x00650F35.
// Layout: Ego_r.pdb C2DBoxF {TLX, TLY, BRX, BRY}, C2DVector {X, Y}, C2DExtentsF {X, Y} (no engine/ headers).
// Shape: TLX/TLY are plain dword copies; BRX = TLX source + width (`fld tl.X ; fadd ext.X`), BRY loads the
// height first (`fld ext.Y ; fadd tl.Y`), i.e. the second sum is written extents-first.
class C2DVector {
public:
    float X, Y;
};
class C2DExtentsF {
public:
    float X, Y;
};
class C2DBoxF {
public:
    C2DBoxF(const C2DVector& topLeft, const C2DExtentsF& extents);
    float TLX, TLY;                                      // +0x00
    float BRX, BRY;                                      // +0x08
};
C2DBoxF::C2DBoxF(const C2DVector& topLeft, const C2DExtentsF& extents)
    : TLX(topLeft.X), TLY(topLeft.Y), BRX(topLeft.X + extents.X), BRY(extents.Y + topLeft.Y)
{
}