// C2DBoxI::C2DBoxI(const C2DCoordI& centre, long radius) -- box of half-width `radius` around a point
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00434740, length 44 bytes. No callees.
// Layout from the PDB (C2DBoxI: TLX +0, TLY +4, BRX +8, BRY +0xc; C2DCoordI: X +0, Y +4).
// Member-initialiser order: TLX = c.X - r, TLY = c.Y - r, BRX = c.X + r, BRY = c.Y + r; `this`
// is moved to eax up front and returned (constructor), radius lives in edx, the centre in ecx.
struct C2DCoordI {
    long X;     // +0x00
    long Y;     // +0x04
};

struct C2DBoxI {
    long TLX;   // +0x00
    long TLY;   // +0x04
    long BRX;   // +0x08
    long BRY;   // +0x0c
    C2DBoxI(const C2DCoordI& centre, long radius);
};

C2DBoxI::C2DBoxI(const C2DCoordI& centre, long radius)
    : TLX(centre.X - radius)
    , TLY(centre.Y - radius)
    , BRX(centre.X + radius)
    , BRY(centre.Y + radius)
{
}