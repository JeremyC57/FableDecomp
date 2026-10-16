#include <cstdio>
// Behaviour model of CVolumeTexture::ReleaseTexture: the memory counters drop by the texture's size only when the
// AddRef/Release probe shows this object holds the last outside reference.
static unsigned int g_graphics = 1000, g_volume = 500;
struct Tex { int refs; unsigned int AddRef() { return ++refs; } unsigned int Release() { return --refs; } };
struct VolumeTexture {
    Tex* tex; long ByteLength; void* PData;
    unsigned int GetMemorySize() const { unsigned int s = 0; if (tex) s = (PData ? 64u : (unsigned int)ByteLength) + 4; return s; }
    void ReleaseTexture()
    {
        if (tex) {
            tex->AddRef();
            if (tex->Release() == 2) { g_graphics -= GetMemorySize(); g_volume -= GetMemorySize(); }
            tex->Release();
            tex = 0;
        }
    }
};
int main()
{
    Tex t; t.refs = 2;
    VolumeTexture v; v.tex = &t; v.ByteLength = 96; v.PData = 0;
    v.ReleaseTexture();
    if (g_graphics != 900 || g_volume != 400 || v.tex || t.refs != 1) { std::printf("00a67f60_TEST FAIL\n"); return 1; }
    Tex u; u.refs = 3; v.tex = &u;
    v.ReleaseTexture();
    if (g_graphics != 900 || u.refs != 2) { std::printf("00a67f60_TEST FAIL\n"); return 1; }
    v.ReleaseTexture();
    std::printf("00a67f60_TEST PASS\n");
    return 0;
}