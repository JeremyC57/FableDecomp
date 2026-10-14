// CDisplayManager::GetVVisibleTelevisionScreenArea() const -- the whole virtual screen as a C2DBoxF: top-left (0, 0), bottom-right (virtual width, virtual height) read from two double globals
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BECE0, length 35 bytes.
// Callees: none. Globals: the virtual-screen extents at 0x013961E8 (width) and 0x013961F0 (height), doubles in .data, each
// narrowed to float on the store. Shape: `this` is unused (`ret 4` pops only the hidden result pointer); the two doubles are
// loaded before any store (x87 stack: height first, width on top), the two zero floats are immediate dword stores and the
// stores interleave by the scheduler. C2DBoxF needs a user constructor to come back through the hidden pointer.
extern double g_VirtualScreenWidth_013961e8;    // 0x013961E8 (65536.0 in the shipped binary)
extern double g_VirtualScreenHeight_013961f0;   // 0x013961F0 (65536.0 in the shipped binary)

struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};

struct CDisplayManager {
    C2DBoxF GetVVisibleTelevisionScreenArea() const;
};

C2DBoxF CDisplayManager::GetVVisibleTelevisionScreenArea() const
{
    return C2DBoxF(0.0f, 0.0f, (float)g_VirtualScreenWidth_013961e8, (float)g_VirtualScreenHeight_013961f0);
}