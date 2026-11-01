// Test for CEnginePrimitiveManagerMeshBase::GetDiffuseTexture 0x00BA9360: bank index 0 gives the blank diffuse texture; a
// composite surface map that resolves the index wins; otherwise the frame lookup (bool out-param form) with the flags.
#include <cstdio>
struct Texture { int id; };
static Texture g_blank = { 1 }, g_composed = { 2 }, g_frameTex = { 3 };
struct Frame {
    bool loaded; unsigned long seenFlags;
    bool GetTexture(unsigned long, unsigned long f, const Texture*& t) { seenFlags = f; if (!loaded) return false; t = &g_frameTex; return true; }
};
static Frame g_frame = { true, 0 };
static bool g_blockOnLoad = false;
static const Texture* GetComposedTexture(long& index, int* surfaces) { return (*surfaces == index) ? &g_composed : 0; }
static const Texture* GetDiffuseTexture(long diffuseIndex, int* surfaces, unsigned long mipmap, bool& preloading, bool force)
{
    long index = diffuseIndex;
    if (index == 0) return &g_blank;
    if (surfaces) { const Texture* composed = GetComposedTexture(index, surfaces); if (composed) return composed; }
    Frame* frame = (index == 1) ? &g_frame : 0;
    unsigned long flags = 0;
    if (g_blockOnLoad || force) flags = 0x11;
    const Texture* texture;
    if (frame && frame->GetTexture(mipmap, flags, texture)) return texture;
    preloading = true;
    return 0;
}
int main() {
    bool pre = false; int surf = 2;
    bool ok = GetDiffuseTexture(0, &surf, 0, pre, false) == &g_blank;
    ok = ok && GetDiffuseTexture(2, &surf, 0, pre, false) == &g_composed;
    ok = ok && GetDiffuseTexture(1, &surf, 0, pre, false) == &g_frameTex && g_frame.seenFlags == 0 && !pre;
    g_blockOnLoad = true; ok = ok && GetDiffuseTexture(1, 0, 0, pre, false) == &g_frameTex && g_frame.seenFlags == 0x11;
    g_frame.loaded = false; ok = ok && GetDiffuseTexture(1, 0, 0, pre, false) == 0 && pre;
    if (ok) printf("00ba9360_TEST PASS\n"); else printf("00ba9360_TEST FAIL\n");
    return ok ? 0 : 1;
}