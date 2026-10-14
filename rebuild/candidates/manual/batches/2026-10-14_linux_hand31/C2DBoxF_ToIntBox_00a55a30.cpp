// C2DBoxF::ToIntBox() const -- the four float edges truncated to longs into a C2DBoxI returned through the hidden pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A55A30, length 65 bytes.
// Callees: __ftol2 @ 0x00BFEA70 (four times). Shape: the constructor's four arguments are evaluated right to left (BRY first, kept in edi, then BRX in ebx,
// TLY in ebp, TLX last in eax) and stored into the hidden result after the last conversion; `ret 4` pops the hidden pointer. The chunk label
// "CEditWorldMap::GetPenArea" is propagated (`this` is a box of four floats, not the editor map); the box types follow the PDB (C2DBoxF / C2DBoxI: TLX, TLY, BRX, BRY).
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};

struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxI ToIntBox() const;
};

C2DBoxI C2DBoxF::ToIntBox() const
{
    return C2DBoxI((long)TLX, (long)TLY, (long)BRX, (long)BRY);
}
