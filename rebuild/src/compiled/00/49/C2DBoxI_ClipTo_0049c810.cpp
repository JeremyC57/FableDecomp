// C2DBoxI::ClipTo(const C2DBoxI& other) -- clamp each corner coordinate into the other box's range
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049C810, length 92 bytes. No callees.
// Layout from the PDB (TLX +0, TLY +4, BRX +8, BRY +0xc). Each coordinate is an
// `if (v < lo) v = lo; else if (v > hi) v = hi;` pair -- the stores are conditional (a ternary
// clamp would store unconditionally); VC lays the last pair out with the `>= lo` side at the end.
class C2DBoxI { public:
    long TLX;   // +0x00
    long TLY;   // +0x04
    long BRX;   // +0x08
    long BRY;   // +0x0c
    void ClipTo(const C2DBoxI& other);
};

void C2DBoxI::ClipTo(const C2DBoxI& other)
{
    if (TLX < other.TLX) TLX = other.TLX; else if (TLX > other.BRX) TLX = other.BRX;
    if (TLY < other.TLY) TLY = other.TLY; else if (TLY > other.BRY) TLY = other.BRY;
    if (BRX < other.TLX) BRX = other.TLX; else if (BRX > other.BRX) BRX = other.BRX;
    if (BRY < other.TLY) BRY = other.TLY; else if (BRY > other.BRY) BRY = other.BRY;
}