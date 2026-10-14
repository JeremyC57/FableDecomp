// CDisplayManager::GetAVisibleTelevisionScreenArea() const -- the whole back buffer as a float box at the origin
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BECB0, length 48 bytes.
// Callees: none. PDB CDisplayManager: C2DExtentsI BackBufferDimensions +396 (X +0x18c, Y +0x190).
// Shape: the box is built straight in the hidden result (`ret 4`, the pointer stays in eax): two immediate float
// zeros, then the two `fild` conversions of the integer extents stored through the x87 (Y loaded first, X stored first).
struct C2DBoxF {                       // 16-byte float box with a user constructor (hidden-pointer return)
    float TLX, TLY, BRX, BRY;
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};

struct CDisplayManager {
    unsigned char _pad[0x18c];         // +0x00 .. +0x18c: vptr, managers, surfaces, D3D objects, caps
    long BackBufferDimensions_X;       // +0x18c C2DExtentsI::X
    long BackBufferDimensions_Y;       // +0x190 C2DExtentsI::Y
    C2DBoxF GetAVisibleTelevisionScreenArea() const;
};

C2DBoxF CDisplayManager::GetAVisibleTelevisionScreenArea() const
{
    return C2DBoxF(0.0f, 0.0f, (float)BackBufferDimensions_X, (float)BackBufferDimensions_Y);
}