// Test for CEnginePrimitiveManagerMeshBase::Get2DAlphaMaskTexture 0x00BA9A70: bank index 0 gives the resource manager's
// saturated texture; otherwise the bank's frame is asked with the block-on-load flags (0x11 when the renderer blocks or
// `force`), and a missing texture sets `preloading` and returns null.
#include <cstdio>
struct Texture { int id; };
static Texture g_sat = { 99 };
static Texture g_frameTex = { 7 };
struct Frame {
    bool loaded; unsigned long seenFlags;
    const Texture* GetTexture(unsigned long, unsigned long flags) { seenFlags = flags; return loaded ? &g_frameTex : 0; }
};
static bool g_blockOnLoad = false;
static Frame g_frame = { true, 0 };
static const Texture* Get2DAlphaMaskTexture(long bankIndex, unsigned long mipmap, bool& preloading, bool force)
{
    if (bankIndex == 0) return &g_sat;
    Frame* frame = (bankIndex == 1) ? &g_frame : 0;
    unsigned long flags = (g_blockOnLoad || force) ? 0x11 : 0;
    const Texture* texture;
    if (frame && (texture = frame->GetTexture(mipmap, flags)) != 0) return texture;
    preloading = true;
    return 0;
}
int main() {
    bool pre = false;
    bool ok = Get2DAlphaMaskTexture(0, 0, pre, false) == &g_sat && !pre;
    ok = ok && Get2DAlphaMaskTexture(1, 0, pre, false) == &g_frameTex && g_frame.seenFlags == 0 && !pre;
    ok = ok && Get2DAlphaMaskTexture(1, 0, pre, true) == &g_frameTex && g_frame.seenFlags == 0x11;
    g_blockOnLoad = true; ok = ok && Get2DAlphaMaskTexture(1, 0, pre, false) == &g_frameTex && g_frame.seenFlags == 0x11 && !pre;
    g_frame.loaded = false; ok = ok && Get2DAlphaMaskTexture(1, 0, pre, false) == 0 && pre;
    pre = false; ok = ok && Get2DAlphaMaskTexture(5, 0, pre, false) == 0 && pre;
    if (ok) printf("00ba9a70_TEST PASS\n"); else printf("00ba9a70_TEST FAIL\n");
    return ok ? 0 : 1;
}