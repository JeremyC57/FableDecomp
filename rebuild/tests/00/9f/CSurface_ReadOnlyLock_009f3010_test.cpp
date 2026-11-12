#include <cstdio>
// behaviour test: CSurface::ReadOnlyLock -- lock the whole surface with flags | D3DLOCK_READONLY (0x10); on success return the
// surface extents with the lock's pitch and bits, on failure an all-zero locked rect
struct Rect { long left, top, right, bottom; };
struct LockedRect { int Pitch; void* pBits; };
struct Extents { long X, Y; };
struct SurfaceLockedRect { Extents Dimensions; long Pitch; unsigned char* PMem; };
static unsigned char g_bits[64];
struct FakeSurface {
    unsigned int Width, Height; long Result; unsigned long LastFlags; Rect LastRect;
    long GetDesc(unsigned int* w, unsigned int* h) { *w = Width; *h = Height; return 0; }
    long LockRect(LockedRect* out, const Rect* r, unsigned long flags)
    {
        LastFlags = flags; LastRect = *r;
        if (Result < 0) return Result;
        out->Pitch = (int)(Width * 4); out->pBits = g_bits; return 0;
    }
};
static SurfaceLockedRect ReadOnlyLock(FakeSurface& s, unsigned long flags)
{
    unsigned int w, h; s.GetDesc(&w, &h);
    Extents dimensions; dimensions.X = (long)w; dimensions.Y = (long)h;
    Rect rect; rect.top = 0; rect.left = 0; rect.bottom = dimensions.Y; rect.right = dimensions.X;
    LockedRect locked;
    SurfaceLockedRect result;
    if (s.LockRect(&locked, &rect, flags | 0x10) >= 0) {
        result.Dimensions = dimensions; result.Pitch = locked.Pitch; result.PMem = (unsigned char*)locked.pBits; return result;
    }
    result.Pitch = 0; result.PMem = 0; result.Dimensions.X = 0; result.Dimensions.Y = 0; return result;
}
int main()
{
    FakeSurface s; s.Width = 16; s.Height = 8; s.Result = 0;
    SurfaceLockedRect r = ReadOnlyLock(s, 0x2000);
    bool ok = s.LastFlags == 0x2010 && s.LastRect.left == 0 && s.LastRect.top == 0 && s.LastRect.right == 16 &&
              s.LastRect.bottom == 8 && r.Dimensions.X == 16 && r.Dimensions.Y == 8 && r.Pitch == 64 && r.PMem == g_bits;
    s.Result = -1;
    r = ReadOnlyLock(s, 0);
    ok = ok && s.LastFlags == 0x10 && r.Dimensions.X == 0 && r.Dimensions.Y == 0 && r.Pitch == 0 && r.PMem == 0;
    std::printf(ok ? "009f3010_TEST PASS\n" : "009f3010_TEST FAIL\n");
    return ok ? 0 : 1;
}