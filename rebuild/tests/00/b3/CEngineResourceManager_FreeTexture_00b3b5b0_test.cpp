#include <cstdio>
static int g_uninit = 0;
class CTexture {
public:
    void* PD3DTexture; unsigned long ByteLengthAndSource;
    ~CTexture() { Uninitialise(); }
    void Uninitialise() { ++g_uninit; PD3DTexture = 0; }
};
class CEngineResourceManager { public: void FreeTexture(CTexture* texture) { delete texture; } };
int main()
{
    CEngineResourceManager m;
    m.FreeTexture(0);
    if (g_uninit != 0) { std::printf("00b3b5b0_TEST FAIL\n"); return 1; }
    m.FreeTexture(new CTexture);
    if (g_uninit != 1) { std::printf("00b3b5b0_TEST FAIL\n"); return 1; }
    std::printf("00b3b5b0_TEST PASS\n");
    return 0;
}