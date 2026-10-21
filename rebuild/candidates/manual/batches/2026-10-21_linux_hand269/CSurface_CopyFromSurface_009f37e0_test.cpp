// Behaviour test for 0x009F37E0 CSurface::CopyFromSurface: a signed destination fed from an unsigned source (when the
// conversion is possible) goes through a temporary A8R8G8B8 image (recursive copy with the default filter, then the
// signed conversion from the locked temporary); every other case is one D3DX copy with the boxes turned into RECTs.
#include <cstdio>

struct Box { long left, top, right, bottom; };
struct Rect { long left, top, right, bottom; };

static int g_d3dx = 0, g_convert = 0, g_recursive = 0, g_tempReleased = 0;
static Rect g_lastDst, g_lastSrc;
static bool g_hadDst = false, g_hadSrc = false;
static unsigned long g_lastFilter = 0;

struct Surface {
    bool isSigned;
    bool canConvert;
    bool isTemp;
    Surface(bool s, bool c, bool t) : isSigned(s), canConvert(c), isTemp(t) {}
    ~Surface() { if (isTemp) ++g_tempReleased; }
    void CopyFromSurface(const Box* dstBox, const Surface& source, const Box* srcBox, unsigned long filter)
    {
        if (isTemp) ++g_recursive;
        if (isSigned && !source.isSigned && canConvert) {
            Surface temp(false, false, true);
            temp.CopyFromSurface(dstBox, source, srcBox, 0xffffffff);
            ++g_convert;
            return;
        }
        Rect dst, src;
        Rect* pDst = 0;
        Rect* pSrc = 0;
        if (dstBox) { dst.top = dstBox->top; dst.bottom = dstBox->bottom; dst.left = dstBox->left; dst.right = dstBox->right; pDst = &dst; }
        if (srcBox) { src.top = srcBox->top; src.bottom = srcBox->bottom; src.left = srcBox->left; src.right = srcBox->right; pSrc = &src; }
        ++g_d3dx;
        g_hadDst = pDst != 0; g_hadSrc = pSrc != 0;
        if (pDst) g_lastDst = *pDst;
        if (pSrc) g_lastSrc = *pSrc;
        g_lastFilter = filter;
    }
};

int main()
{
    int ok = 1;
    Surface plain(false, false, false), unsignedSrc(false, false, false), signedDst(true, true, false);
    Box a = { 1, 2, 3, 4 };
    plain.CopyFromSurface(&a, unsignedSrc, 0, 7);
    ok &= g_d3dx == 1 && g_hadDst && !g_hadSrc && g_lastDst.left == 1 && g_lastDst.top == 2 && g_lastDst.right == 3
        && g_lastDst.bottom == 4 && g_lastFilter == 7;
    signedDst.CopyFromSurface(0, unsignedSrc, &a, 5);
    ok &= g_convert == 1 && g_recursive == 1 && g_d3dx == 2 && g_lastFilter == 0xffffffff && g_hadSrc && !g_hadDst
        && g_tempReleased == 1;
    Surface signedNoConv(true, false, false);
    signedNoConv.CopyFromSurface(0, unsignedSrc, 0, 3);
    ok &= g_convert == 1 && g_d3dx == 3 && g_lastFilter == 3;
    Surface signedSrc(true, true, false);
    signedDst.CopyFromSurface(0, signedSrc, 0, 9);
    ok &= g_convert == 1 && g_d3dx == 4 && g_lastFilter == 9;
    if (ok) std::printf("009f37e0_TEST PASS\n"); else std::printf("009f37e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
