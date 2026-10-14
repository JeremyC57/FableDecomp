// C2DBoxF::ContainsPoint(const C2DVector& p) const -- half-open float box test
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00535CD0, length 66 bytes. No callees.
// Layout from the PDB (C2DBoxF: TLX +0, TLY +4, BRX +8, BRY +0xc; C2DVector: X +0, Y +4).
// Four ordered x87 compares, the point loaded first each time (`fld [p] ; fcomp [member]`):
// p.X >= TLX (`test ah,1 ; jne` fails on C0), p.X < BRX (`test ah,5 ; jp`), then the same for Y.
// `mov eax,1` / `xor eax,eax` is a long result, not a bool (that would be `mov al,1`).
struct C2DVector {
    float X;    // +0x00
    float Y;    // +0x04
};

struct C2DBoxF {
    float TLX;  // +0x00
    float TLY;  // +0x04
    float BRX;  // +0x08
    float BRY;  // +0x0c
    long ContainsPoint(const C2DVector& p) const;
};

long C2DBoxF::ContainsPoint(const C2DVector& p) const
{
    if (p.X >= TLX && p.X < BRX && p.Y >= TLY && p.Y < BRY)
        return 1;
    return 0;
}