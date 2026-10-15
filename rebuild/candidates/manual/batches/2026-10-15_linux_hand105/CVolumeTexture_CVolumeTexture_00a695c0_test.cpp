#include <cstdio>
static int g_base = 0;
struct IDirect3DVolumeTexture9;
struct CBaseClass { CBaseClass() { ++g_base; } virtual void v00() { } virtual ~CBaseClass() { } };
struct CVolumeTexture : CBaseClass {
    IDirect3DVolumeTexture9* PD3DVolumeTexture; long ByteLength; void* PData;
    CVolumeTexture() : PD3DVolumeTexture(0), PData(0) { }
    virtual void v00() { }
};
int main()
{
    CVolumeTexture t;
    if (g_base != 1 || t.PD3DVolumeTexture != 0 || t.PData != 0) { std::printf("00a695c0_TEST FAIL\n"); return 1; }
    std::printf("00a695c0_TEST PASS\n");
    return 0;
}
