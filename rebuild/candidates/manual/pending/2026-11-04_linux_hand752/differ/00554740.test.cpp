// Behaviour test for NUISystem::CMorphingSprite::Draw (0x00554740): a standalone model of the per-pass handle bookkeeping and the
// sprite build -- an invisible sprite (alpha 0, or both render zooms <= 0) still draws one frame (NoRenderNextFrame set) and on the
// next frame releases: pass 0 clears the component handle (or a child of the parent handle), later passes drop the list handle of
// that pass (or grow the list). A visible pass picks the component handle (pass 0) or the list handle of the pass (created when
// missing), rounds the render position to nearest, sizes the box by Size * RelativeRenderZoom, uses the state's graphic unless
// SpriteGraphicIndex overrides it, and sets a child primitive when the caller gives a parent handle and a child index.
#include <cstdio>
#include <cmath>
#include <map>
struct Sprite {
    unsigned char alpha; float zoomX, zoomY; unsigned char noRender; long renderCount; unsigned long handles; bool handleCleared;
    float posX, posY, sizeX, sizeY, rzX, rzY; std::map<unsigned long, unsigned long> indices; unsigned long state, override_;
};
struct Out { int action; float l, t, r, b; unsigned long graphic; long childIndex; };   // 1 child, 2 persistent, 3 clear child, 4 cleared, 0 none
static Out Draw(Sprite& s, bool parent, bool hasIndex, long index)
{
    Out o = { 0, 0, 0, 0, 0, 0, index };
    if (s.alpha > 0 && (!(s.zoomX <= 0.0f) || !(s.zoomY <= 0.0f))) s.noRender = 0;
    else if (s.noRender == 1) {
        if (s.renderCount == 0) { if (!parent) { s.handleCleared = true; o.action = 4; return o; } }
        else { if (s.handles >= (unsigned long)s.renderCount) --s.handles; else ++s.handles; if (!parent) return o; }
        o.action = 3; ++o.childIndex; return o;
    } else s.noRender = 1;
    if (s.renderCount > 0 && s.handles < (unsigned long)s.renderCount) ++s.handles;
    ++s.renderCount;
    float x = (float)std::nearbyint(s.posX), y = (float)std::nearbyint(s.posY);
    o.l = x; o.t = y; o.r = x + s.sizeX * s.rzX; o.b = y + s.sizeY * s.rzY;
    o.graphic = s.override_ ? s.override_ : s.indices[s.state];
    if (parent && hasIndex) { o.action = 1; ++o.childIndex; } else o.action = 2;
    return o;
}
int main()
{
    int fail = 0;
    Sprite s = { 255, 1, 1, 0, 0, 0, false, 10.4f, 20.6f, 32, 16, 2, 0.5f, std::map<unsigned long, unsigned long>(), 3, 0 };
    s.indices[3] = 77;
    Out a = Draw(s, false, false, 0);
    if (a.action != 2 || a.l != 10.0f || a.t != 21.0f || a.r != 74.0f || a.b != 29.0f || a.graphic != 77 || s.renderCount != 1) fail = 1;
    Out b = Draw(s, true, true, 4);                                   // second pass: a list handle is created, child primitive
    if (b.action != 1 || b.childIndex != 5 || s.handles != 1 || s.renderCount != 2) fail = 2;
    s.override_ = 9;
    if (Draw(s, false, false, 0).graphic != 9) fail = 3;
    s.alpha = 0; s.renderCount = 0;
    Out c = Draw(s, false, false, 0);                                 // first invisible frame still draws
    if (c.action != 2 || s.noRender != 1) fail = 4;
    s.renderCount = 0;
    Out d = Draw(s, false, false, 0);                                 // second: pass 0 clears the component handle
    if (d.action != 4 || !s.handleCleared) fail = 5;
    s.renderCount = 2; s.handles = 2;
    Out e = Draw(s, true, true, 7);                                   // later pass with a parent: drop the list handle, clear the child
    if (e.action != 3 || e.childIndex != 8 || s.handles != 1) fail = 6;
    if (fail) { std::printf("00554740_TEST FAIL %d\n", fail); return 1; }
    std::printf("00554740_TEST PASS\n");
    return 0;
}
