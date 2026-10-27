// C2DBoxF::GetTRPos() const -- the box's top-right corner (BRX, TLY)
// Leaf: ?GetTRPos@C2DBoxF@@QBE?AVC2DVector@@XZ
// Retail: Fable.exe @ 0x00643970, 18 bytes, thiscall `ret 4` (hidden C2DVector result pointer, returned in eax).
// Identity (probable, FableWin.pdb order + body): FableWin.pdb (the PC build, which keeps the inline accessors out of line)
// lists ?GetTRPos@C2DBoxF@@QBE?AVC2DVector@@XZ next to SetTLPos(const C2DVector&), SetWidth(float), SetHeight(float); retail
// has those three bodies directly before this one (0x00643930 copies a C2DVector into TLX/TLY, 0x00643950 stores TLX + w
// into BRX, 0x00643960 TLY + h into BRY). This one builds a C2DVector from BRX (+8) and TLY (+4): the top-right corner.
// No direct caller (a COMDAT copy; the abs hit at 0x005BB0A0 is the rel32 of a call, not a reference).
// Callees: none.
// Shape: TLY loaded through the FPU, BRX copied as a dword (the C2DVector(float, float) constructor inlined into the result).
// Layout: Ego_r.pdb C2DBoxF {TLX, TLY, BRX, BRY} (16 bytes); C2DVector {X, Y}.
class C2DVector {
public:
    float X;
    float Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class C2DBoxF {
public:
    float TLX;                                              // +0x00
    float TLY;                                              // +0x04
    float BRX;                                              // +0x08
    float BRY;                                              // +0x0C
    C2DVector GetTRPos() const;
};

C2DVector C2DBoxF::GetTRPos() const
{
    return C2DVector(BRX, TLY);
}