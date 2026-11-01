// Test for CEnginePrimitiveManagerMeshBase::GetBumpTexture 0x00BA95D0: bank index 0 gives the full bank's blank bump
// texture; a composite surface map that resolves the index wins; otherwise the frame lookup with the block-on-load flags.
#include <cstdio>
struct Texture { int id; };
static Texture g_blank = { 1 }, g_composed = { 2 }, g_frameTex = { 3 };
struct Frame { bool loaded; unsigned long seenFlags; const Texture* GetTexture(unsigned long, unsigned long f) { seenFlags = f; return loaded ? &g_frameTex : 0; } };
static Frame g_frame = { true, 0 };
static bool g_blockOnLoad = false;
static const Texture* GetComposedTexture(long& index, int* surfaces) { return (*surfaces == index) ? &g_composed : 0; }
static const Texture* GetBumpTexture(long bumpIndex, int* surfaces, unsigned long mipmap, bool& preloading, bool force)
{
    long index = bumpIndex;
    if (index == 0) return &g_blank;
    if (surfaces) { const Texture* composed = GetComposedTexture(index, surfaces); if (composed) return composed; }
    Frame* frame = (index == 1) ? &g_frame : 0;
    unsigned long flags = 0;
    if (g_blockOnLoad || force) flags = 0x11;
    const Texture* texture;
    if (frame && (texture = frame->GetTexture(mipmap, flags)) != 0) return texture;
    preloading = true;
    return 0;
}
int main() {
    bool pre = false; int surf = 1;
    bool ok = GetBumpTexture(0, &surf, 0, pre, false) == &g_blank;
    ok = ok && GetBumpTexture(1, &surf, 0, pre, false) == &g_composed;
    ok = ok && GetBumpTexture(1, 0, 0, pre, false) == &g_frameTex && g_frame.seenFlags == 0 && !pre;
    ok = ok && GetBumpTexture(1, 0, 0, pre, true) == &g_frameTex && g_frame.seenFlags == 0x11;
    g_frame.loaded = false; ok = ok && GetBumpTexture(1, 0, 0, pre, false) == 0 && pre;
    if (ok) printf("00ba95d0_TEST PASS\n"); else printf("00ba95d0_TEST FAIL\n");
    return ok ? 0 : 1;
}