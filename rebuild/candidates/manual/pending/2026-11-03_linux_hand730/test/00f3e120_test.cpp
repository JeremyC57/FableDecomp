// Behaviour test for the closed-fan check of CTriangleStripifier2::FindBestStartVertex (0x00F3E120): walking the fan of a
// vertex around its polygons returns to the first neighbour exactly when the vertex is interior (its fan is closed).
#include <cstdio>
#include <vector>
static bool IsClosedFan(long i, const std::vector<unsigned short>& polys, const std::vector<std::vector<long> >& vtpm)
{
    const std::vector<long>& ring = vtpm[i];
    if (ring.empty()) return false;
    long m0 = 0;
    while (polys[ring[0] * 3 + m0] != i) ++m0;
    long first = polys[ring[0] * 3 + (m0 + 1) % 3];
    long vert = first;
    for (unsigned long k = 0; k < ring.size(); ++k) {
        bool found = false;
        for (unsigned long p = 0; p < ring.size() && !found; ++p) {
            long q = ring[p];
            for (long m = 0; m < 3; ++m) {
                long next = (m + 1) % 3, third = (next + 1) % 3;
                if (polys[q * 3 + m] == vert && polys[q * 3 + next] == i) { vert = polys[q * 3 + third]; found = true; break; }
            }
        }
        if (!found) return false;
        if (vert == first) return true;
    }
    return false;
}
int main()
{
    // a square split into 4 triangles around centre vertex 4: corners 0,1,2,3 (counter-clockwise)
    unsigned short tri[] = { 0,1,4, 1,2,4, 2,3,4, 3,0,4 };
    std::vector<unsigned short> polys(tri, tri + 12);
    std::vector<std::vector<long> > vtpm(5);
    for (long p = 0; p < 4; ++p) for (long m = 0; m < 3; ++m) vtpm[polys[p * 3 + m]].push_back(p);
    if (!IsClosedFan(4, polys, vtpm)) { std::printf("00f3e120_TEST FAIL\n"); return 1; }
    if (IsClosedFan(0, polys, vtpm)) { std::printf("00f3e120_TEST FAIL\n"); return 1; }
    std::printf("00f3e120_TEST PASS\n");
    return 0;
}
