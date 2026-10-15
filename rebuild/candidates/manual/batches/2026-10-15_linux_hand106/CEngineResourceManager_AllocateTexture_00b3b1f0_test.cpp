#include <cstdio>
struct C2DExtentsI { long x, y; };
struct CPixelFormat { long FormatIndex; };
static long g_args[7];
class CTexture {
public:
    void* PD3DTexture; unsigned long ByteLengthAndSource;
    CTexture() : PD3DTexture(0), ByteLengthAndSource(0) { }
    bool Initialise(const C2DExtentsI& size, long levels, const CPixelFormat& format, unsigned long usage, int pool, bool dynamic, bool rt)
    { g_args[0] = size.x; g_args[1] = levels; g_args[2] = format.FormatIndex; g_args[3] = (long)usage; g_args[4] = pool; g_args[5] = dynamic; g_args[6] = rt; return true; }
};
struct CEngineResourceManager {
    CTexture* AllocateTexture(unsigned long, const C2DExtentsI& size, long levels, const CPixelFormat& format)
    {
        CTexture* texture = new CTexture;
        texture->Initialise(size, levels, format, 0, 1, false, false);
        return texture;
    }
};
int main()
{
    CEngineResourceManager m; C2DExtentsI e = { 64, 32 }; CPixelFormat f = { 7 };
    CTexture* t = m.AllocateTexture(99, e, 3, f);
    bool ok = t && t->PD3DTexture == 0 && t->ByteLengthAndSource == 0 && g_args[0] == 64 && g_args[1] == 3 && g_args[2] == 7 &&
              g_args[3] == 0 && g_args[4] == 1 && g_args[5] == 0 && g_args[6] == 0;
    delete t;
    if (!ok) { std::printf("00b3b1f0_TEST FAIL\n"); return 1; }
    std::printf("00b3b1f0_TEST PASS\n");
    return 0;
}
