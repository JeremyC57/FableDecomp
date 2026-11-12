// Behaviour test for CSurface::CopyFromMemory (0x009F4390): path selection (signed conversion / direct copy / D3DX) and the
// D3DX rectangles, through a copy of the model with recording stand-ins for the callees.
#include <cstdio>
struct Box { long TLX, TLY, BRX, BRY; };
struct Rect { long left, top, right, bottom; };
struct Format { long index; bool isSigned; };
static const unsigned long kD3DFormats[4] = { 21, 22, 60, 63 };
struct Surface {
    unsigned long width, height; Format format;
    bool canDo;
    int path; Rect dstR, srcR; bool hasDst, hasSrc; unsigned long d3dFormat, filter; long pitch;
    Format GetPixelFormat() const { return format; }
    unsigned long GetWidth() const { return width; }
    unsigned long GetHeight() const { return height; }
    void CopyFromMemory(const Box* destRect, const void* source, const Box* sourceRect, const Format& sourceFormat,
                        long sourcePitch, unsigned long colourKey)
    {
        long destWidth = destRect ? destRect->BRX - destRect->TLX : (long)GetWidth();
        long destHeight = destRect ? destRect->BRY - destRect->TLY : (long)GetHeight();
        long sourceWidth = sourceRect ? sourceRect->BRX - sourceRect->TLX : destWidth;
        long sourceHeight = sourceRect ? sourceRect->BRY - sourceRect->TLY : destHeight;
        if ((GetPixelFormat().isSigned || sourceFormat.isSigned) && !(GetPixelFormat().isSigned && sourceFormat.isSigned) && canDo) {
            path = 1; pitch = sourcePitch; return;
        }
        if (sourceFormat.index == GetPixelFormat().index && sourceWidth == destWidth && sourceHeight == destHeight) {
            path = 2; pitch = sourcePitch; return;
        }
        Rect* pDest = 0; Rect* pSource = 0; Rect dest; Rect src;
        if (destRect) { dest.top = destRect->TLY; dest.bottom = destRect->BRY; dest.left = destRect->TLX; dest.right = destRect->BRX; pDest = &dest; }
        if (sourceRect) { src.top = sourceRect->TLY; src.bottom = sourceRect->BRY; src.left = sourceRect->TLX; src.right = sourceRect->BRX; pSource = &src; }
        path = 3; hasDst = pDest != 0; hasSrc = pSource != 0;
        if (pDest) dstR = *pDest;
        if (pSource) srcR = *pSource;
        d3dFormat = kD3DFormats[sourceFormat.index]; pitch = sourcePitch; filter = colourKey;
    }
};
int main()
{
    int fail = 0;
    Surface s = { 64, 32, { 0, false }, false };
    Format same = { 0, false }, other = { 2, false }, sgn = { 1, true };
    Box full = { 0, 0, 64, 32 }, part = { 4, 2, 20, 10 }, part2 = { 1, 1, 17, 9 };
    s.CopyFromMemory(0, 0, 0, same, 256, 0xffffffff);           if (s.path != 2 || s.pitch != 256) fail = 1;
    s.CopyFromMemory(&full, 0, 0, same, 256, 0);                if (s.path != 2) fail = 2;
    s.CopyFromMemory(&part, 0, &part2, same, 64, 0);            if (s.path != 2) fail = 3;
    s.CopyFromMemory(&part, 0, &full, same, 64, 0);             if (s.path != 3 || s.d3dFormat != 21) fail = 9;
    s.CopyFromMemory(&part, 0, 0, other, 64, 7);                if (s.path != 3 || !s.hasDst || s.hasSrc || s.dstR.left != 4 || s.dstR.bottom != 10 || s.filter != 7) fail = 4;
    s.CopyFromMemory(0, 0, &part, other, 64, 5);                if (s.path != 3 || s.hasDst || !s.hasSrc || s.srcR.right != 20 || s.srcR.top != 2 || s.d3dFormat != 60) fail = 5;
    s.CopyFromMemory(0, 0, 0, sgn, 64, 0);                      if (s.path != 3 || s.d3dFormat != 22) fail = 6;
    s.canDo = true;
    s.CopyFromMemory(0, 0, 0, sgn, 128, 0);                     if (s.path != 1 || s.pitch != 128) fail = 7;
    s.format = sgn;
    s.CopyFromMemory(0, 0, 0, sgn, 64, 0);                      if (s.path != 2) fail = 8;
    if (fail) { std::printf("009f4390_TEST FAIL\n"); return 1; }
    std::printf("009f4390_TEST PASS\n");
    return 0;
}