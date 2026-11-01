// Test for COptimisedPrimitive::PaintSegment (0x00AC8170): breadth-first flood fill over the faces' three adjacency slots;
// the start face is painted unconditionally, a neighbour is painted and queued only when it exists and is still -1, -1
// slots are skipped, and the caller's list receives every visited face in visiting order.
#include <cstdio>
struct Face { long Edges[3]; bool Exists; long Segment; };
static void PaintSegment(Face* faces, unsigned long face, long segment, long* out, int& outCount)
{
    long queue[64]; int head = 0, tail = 0;
    faces[face].Segment = segment;
    queue[tail++] = (long)face;
    while (head != tail) {
        long current = queue[head++];
        out[outCount++] = current;
        Face& f = faces[current];
        for (long i = 0; i < 3; ++i) {
            long adjacent = f.Edges[i];
            if (adjacent != -1 && faces[adjacent].Exists && faces[adjacent].Segment == -1) {
                faces[adjacent].Segment = segment;
                queue[tail++] = adjacent;
            }
        }
    }
}
int main()
{
    // 0-1-2 chain plus 3 (deleted face next to 2) and 4 (already painted next to 0), 5 isolated
    Face f[6] = {
        {{1, 4, -1}, true, -1}, {{0, 2, -1}, true, -1}, {{1, 3, -1}, true, -1},
        {{2, -1, -1}, false, -1}, {{0, -1, -1}, true, 9}, {{-1, -1, -1}, true, -1} };
    long out[8]; int n = 0;
    PaintSegment(f, 0, 2, out, n);
    if (n != 3 || out[0] != 0 || out[1] != 1 || out[2] != 2 || f[0].Segment != 2 || f[1].Segment != 2 || f[2].Segment != 2 ||
        f[3].Segment != -1 || f[4].Segment != 9 || f[5].Segment != -1) { std::printf("00ac8170_TEST FAIL\n"); return 1; }
    n = 0;
    PaintSegment(f, 5, 3, out, n);
    if (n != 1 || out[0] != 5 || f[5].Segment != 3) { std::printf("00ac8170_TEST FAIL\n"); return 1; }
    std::printf("00ac8170_TEST PASS\n");
    return 0;
}