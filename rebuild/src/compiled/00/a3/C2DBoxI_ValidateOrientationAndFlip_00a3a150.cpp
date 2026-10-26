// C2DBoxI::ValidateOrientationAndFlip() -- swap the corners so TL <= BR on each axis
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3A150, length 48 bytes. No callees.
// Layout from the PDB (TLX +0, TLY +4, BRX +8, BRY +0xc). Two `if (BR < TL) Swap(TL, BR)`
// blocks through an inline by-reference swap; VC keeps the X pair in registers and re-reads BRY
// in the Y swap.
template <class T>
inline void Swap(T& a, T& b)
{
    T t = a;
    a = b;
    b = t;
}

class C2DBoxI { public:
    long TLX;   // +0x00
    long TLY;   // +0x04
    long BRX;   // +0x08
    long BRY;   // +0x0c
    void ValidateOrientationAndFlip();
};

void C2DBoxI::ValidateOrientationAndFlip()
{
    if (BRX < TLX)
        Swap(TLX, BRX);
    if (BRY < TLY)
        Swap(TLY, BRY);
}