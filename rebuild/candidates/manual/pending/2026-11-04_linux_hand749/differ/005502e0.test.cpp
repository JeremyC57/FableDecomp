// Behaviour test for NUISystem::CMesh::Draw (0x005502E0): a standalone model -- invisible means alpha 0 or both zoom components
// <= 0; the first invisible frame still draws (NoRenderNextFrame set), the next one clears one child primitive (parent handle) or
// the persistent handle and returns. The screen box is centred on the render position with size (zoom.X * Width, Height * zoom.Y),
// corners rounded to nearest; a box with TL.X >= back-buffer width, TL.Y >= height, BR.X <= 0 or BR.Y <= 0 clears the handle. A mesh
// index absent from the mesh bank draws nothing; data type 5 (animated) always sets the persistent handle; other meshes set a child
// primitive when both the parent handle and the child index are given, else the persistent handle.
#include <cstdio>
#include <cmath>
struct Box { float tlx, tly, brx, bry; };
enum { NONE, CHILD, PERSISTENT, CLEAR_CHILD, CLEAR_HANDLE };
static int Draw(unsigned char alpha, float zx, float zy, unsigned char& nr, float px, float py, unsigned long w, unsigned long h,
                bool exists, int type, bool parent, bool hasIndex, float bbw, float bbh, Box& box)
{
    if (alpha <= 0 || (zx <= 0.0f && zy <= 0.0f)) {
        if (nr == 1) return parent ? CLEAR_CHILD : CLEAR_HANDLE;
        nr = 1;
    } else nr = 0;
    float sx = zx * w, sy = h * zy;
    px -= 0.5f * sx; py -= 0.5f * sy;
    box.tlx = (float)std::nearbyint(px); box.tly = (float)std::nearbyint(py);
    box.brx = (float)std::nearbyint(px + sx); box.bry = (float)std::nearbyint(py + sy);
    if (box.tlx >= bbw || box.tly >= bbh || box.brx <= 0.0f || box.bry <= 0.0f) return CLEAR_HANDLE;
    if (!exists) return NONE;
    if (type == 5) return PERSISTENT;
    return parent && hasIndex ? CHILD : PERSISTENT;
}
int main()
{
    int fail = 0; unsigned char nr = 0; Box b;
    if (Draw(255, 1, 1, nr, 100, 100, 40, 20, true, 1, true, true, 640, 480, b) != CHILD || b.tlx != 80 || b.tly != 90 || b.brx != 120 || b.bry != 110) fail = 1;
    if (Draw(255, 0.5f, 2, nr, 100.3f, 100, 41, 20, true, 1, false, false, 640, 480, b) != PERSISTENT || b.tlx != 90 || b.brx != 111 || b.tly != 80) fail = 2;
    if (Draw(255, 1, 1, nr, 100, 100, 40, 20, true, 5, true, true, 640, 480, b) != PERSISTENT) fail = 3;   // animated: never a child
    if (Draw(255, 1, 1, nr, 100, 100, 40, 20, false, 1, true, true, 640, 480, b) != NONE) fail = 4;
    if (Draw(255, 1, 1, nr, -30, 100, 40, 20, true, 1, true, true, 640, 480, b) != CLEAR_HANDLE) fail = 5; // BR.X <= 0
    if (Draw(255, 1, 1, nr, 700, 100, 40, 20, true, 1, true, true, 640, 480, b) != CLEAR_HANDLE) fail = 6; // TL.X >= width
    if (Draw(0, 1, 1, nr, 100, 100, 40, 20, true, 1, true, true, 640, 480, b) != CHILD || nr != 1) fail = 7;
    if (Draw(0, 1, 1, nr, 100, 100, 40, 20, true, 1, true, true, 640, 480, b) != CLEAR_CHILD) fail = 8;
    if (Draw(255, 0, 1, nr, 100, 100, 40, 20, true, 1, false, false, 640, 480, b) != PERSISTENT || nr != 0) fail = 9; // one zoom > 0 is visible
    if (fail) { std::printf("005502e0_TEST FAIL %d\n", fail); return 1; }
    std::printf("005502e0_TEST PASS\n");
    return 0;
}
