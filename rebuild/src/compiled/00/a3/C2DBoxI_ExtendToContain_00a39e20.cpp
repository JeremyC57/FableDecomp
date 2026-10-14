// C2DBoxI::ExtendToContain(const C2DCoordI& p) -- grow the box so the point is inside
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A39E20, length 51 bytes. No callees.
// Layout from the PDB (C2DBoxI: TLX +0, TLY +4, BRX +8, BRY +0xc; C2DCoordI: X +0, Y +4).
// The argument is a point (only +0/+4 are read), not a box as the manifest's parameter list says.
// Four guarded stores, member on the left of each compare (`cmp [this+n],reg`); p.X is re-read
// between the TLX and BRX tests, p.Y between TLY and BRY.
struct C2DCoordI {
    long X;     // +0x00
    long Y;     // +0x04
};

struct C2DBoxI {
    long TLX;   // +0x00
    long TLY;   // +0x04
    long BRX;   // +0x08
    long BRY;   // +0x0c
    void ExtendToContain(const C2DCoordI& p);
};

void C2DBoxI::ExtendToContain(const C2DCoordI& p)
{
    if (TLX > p.X) TLX = p.X;
    if (BRX < p.X) BRX = p.X;
    if (TLY > p.Y) TLY = p.Y;
    if (BRY < p.Y) BRY = p.Y;
}