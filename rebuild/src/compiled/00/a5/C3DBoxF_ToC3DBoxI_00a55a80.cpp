// C3DBoxF::ToC3DBoxI() const -- the six float extents of the box truncated to longs into a C3DBoxI returned through the hidden pointer
// Leaf: ?ToC3DBoxI@C3DBoxF@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A55A80, 111 bytes, `ret 4` (PDB ?ToC3DBoxI@C3DBoxF@@QBE?AVC3DBoxI@@XZ).
// Callees: __ftol2 @ 0x00BFEA70 (six times). The 3D sibling of the landed C2DBoxF::ToIntBox 0x00A55A30 just before it.
// Label: the manifest's `MakePath` is a propagation.
// Layout: Ego_r.pdb C3DBoxF / C3DBoxI derive from C2DBoxF / C2DBoxI (TLX, TLY, BRX, BRY at +0..+0xc) and add TLZ +0x10,
//   BRZ +0x14. No engine headers.
// Shape: `return C3DBoxI((long)TLX, (long)TLY, (long)TLZ, (long)BRX, (long)BRY, (long)BRZ);` -- the PDB constructor
//   ??0C3DBoxI@@QAE@JJJJJJ@Z takes the top-left corner then the bottom-right one, so the right-to-left argument evaluation
//   converts BRZ, BRY, BRX, TLZ, TLY, TLX (retail's `fld` order +0x14, +0xc, +8, +0x10, +4, +0); every result is stored
//   into the hidden result after the last conversion.
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;                            // +0x00 .. +0x0c
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};

class C3DBoxI : public C2DBoxI {
public:
    long TLZ;                                           // +0x10
    long BRZ;                                           // +0x14
    C3DBoxI(long tlx, long tly, long tlz, long brx, long bry, long brz) : C2DBoxI(tlx, tly, brx, bry), TLZ(tlz), BRZ(brz) { }
    C3DBoxI(const C3DBoxI& other) : C2DBoxI(other), TLZ(other.TLZ), BRZ(other.BRZ) { }
};

class C2DBoxF {
public:
    float TLX, TLY, BRX, BRY;                           // +0x00 .. +0x0c
};

class C3DBoxF : public C2DBoxF {
public:
    float TLZ;                                          // +0x10
    float BRZ;                                          // +0x14
    C3DBoxI ToC3DBoxI() const;
};

C3DBoxI C3DBoxF::ToC3DBoxI() const
{
    return C3DBoxI((long)TLX, (long)TLY, (long)TLZ, (long)BRX, (long)BRY, (long)BRZ);
}