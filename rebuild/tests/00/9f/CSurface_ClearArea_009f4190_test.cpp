// Behaviour test for 0x009F4190 CSurface::ClearArea: the box is clamped to [0, width] x [0, height] and every pixel of the
// clamped box is written with the colour (bytes per pixel from the format, rows stepped by the pitch); nothing else changes.
#include <cstdio>
#include <cstring>

struct Colour { unsigned char B, G, R, A; };
struct Box { long left, top, right, bottom; };

struct Surface {
    long width, height, pitch, depth;
    unsigned char mem[16 * 8 * 4];
    void ClearArea(const Box& area, const Colour& colour)
    {
        Box box;
        box.left = area.left; box.top = area.top; box.right = area.right; box.bottom = area.bottom;
        if (box.left < 0) box.left = 0; else if (box.left > width) box.left = width;
        if (box.top < 0) box.top = 0; else if (box.top > height) box.top = height;
        if (box.right < 0) box.right = 0; else if (box.right > width) box.right = width;
        if (box.bottom < 0) box.bottom = 0; else if (box.bottom > height) box.bottom = height;
        unsigned char* row = mem + depth * box.left + pitch * box.top;
        for (long y = box.top; y < box.bottom; ++y) {
            unsigned char* p = row;
            for (long x = box.left; x < box.right; ++x) {
                std::memcpy(p, &colour, (size_t)depth);
                p += depth;
            }
            row += pitch;
        }
    }
};

int main()
{
    Surface s;
    s.width = 16; s.height = 8; s.depth = 4; s.pitch = 64;
    std::memset(s.mem, 0, sizeof(s.mem));
    Colour c = { 1, 2, 3, 4 };
    Box b = { -5, 6, 3, 20 };
    s.ClearArea(b, c);
    int ok = 1;
    for (long y = 0; y < 8; ++y)
        for (long x = 0; x < 16; ++x) {
            const unsigned char* p = s.mem + y * 64 + x * 4;
            bool inside = x < 3 && y >= 6;
            ok &= inside ? (p[0] == 1 && p[1] == 2 && p[2] == 3 && p[3] == 4) : (p[0] == 0 && p[3] == 0);
        }
    Box empty = { 10, 2, 4, 5 };
    Colour d = { 9, 9, 9, 9 };
    s.ClearArea(empty, d);
    ok &= s.mem[2 * 64 + 10 * 4] == 0;
    if (ok) std::printf("009f4190_TEST PASS\n"); else std::printf("009f4190_TEST FAIL\n");
    return ok ? 0 : 1;
}