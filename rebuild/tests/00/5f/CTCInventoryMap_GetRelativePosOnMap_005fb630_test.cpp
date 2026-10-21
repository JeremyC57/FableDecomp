// Behaviour test for CTCInventoryMap::GetRelativePosOnMap (0x005FB630): the extent is the bounding box of records 1..n-1
// (record 0 ignored) and the result is position * (map size / extent) per axis.
#include <cstdio>

struct Box { long x0, y0, x1, y1; };
static bool Relative(const Box* boxes, long n, float mapW, float mapH, float px, float py, float* rx, float* ry)
{
    if (n > 0) {
        Box b;
        b.x0 = b.y0 = b.x1 = b.y1 = 0;
        for (long i = 1; i < n; ++i) {
            const Box& r = boxes[i];
            if (i == 1) b = r;
            else {
                if (b.x0 > r.x0) b.x0 = r.x0;
                if (b.y0 > r.y0) b.y0 = r.y0;
                if (b.x1 < r.x1) b.x1 = r.x1;
                if (b.y1 < r.y1) b.y1 = r.y1;
            }
        }
        long w = b.x1 - b.x0, h = b.y1 - b.y0;
        *rx = mapW / w * px;
        *ry = mapH / h * py;
        return true;
    }
    return false;
}
int main()
{
    Box boxes[3] = { { -1000, -1000, 1000, 1000 }, { 0, 0, 100, 50 }, { -100, 10, 50, 150 } };
    float rx = 0, ry = 0;
    if (!Relative(boxes, 3, 400.0f, 300.0f, 10.0f, 20.0f, &rx, &ry)) { std::printf("005fb630_TEST FAIL\n"); return 1; }
    // extent: x -100..100 = 200, y 0..150 = 150
    if (rx != 20.0f || ry != 40.0f) { std::printf("005fb630_TEST FAIL\n"); return 1; }
    std::printf("005fb630_TEST PASS\n");
    return 0;
}