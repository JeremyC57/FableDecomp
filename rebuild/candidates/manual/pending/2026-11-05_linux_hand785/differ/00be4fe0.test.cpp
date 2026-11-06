// Behaviour test for CWaterPatchTesselatorBase<CTVertexSeaBackground>::TesselateEdge (0x00BE4FE0): a model of the edge
// bridging -- the adjacent strips' water vertices are walked in world order (FindFirstWaterAdjacentVertex skips strips with no
// water vertex at or past the position, IterateThroughAdjacentVerts steps to the next strip past the shared end vertex); every
// one-vertex span of this strip with adjacent water vertices strictly inside it becomes a fan of triangles around the span's fan
// base vertex (wound by `flip`), and its edge index is redirected to the fan base; spans without such vertices keep the plain
// edge vertex index.
#include <stdio.h>
#include <string.h>

struct Strip { int start, size; bool vertical; int verts[8]; int count; };   // verts: main-axis world positions of water vertices
static Strip adj[2] = { { 0, 4, false, { 1, 3 }, 2 }, { 4, 4, false, { 6 }, 1 } };
static Strip* GetEdgeStrip(long x, long, long length, int) { for (int i = 0; i < 2; i++) if (x >= adj[i].start && x < adj[i].start + adj[i].size && length > 0) return &adj[i]; return 0; }
static long FindWaterVert(Strip* s, long pos) { long i = 0; while (i < s->count && s->verts[i] < pos) i++; return i; }
static void FindFirst(Strip*& strip, long& index, long x, long length)
{
    strip = GetEdgeStrip(x, 0, length, 0);
    while (strip) {
        index = FindWaterVert(strip, x);
        if (index < strip->count) return;
        long size = strip->size; x += size; length -= size;
        if (length <= 0) { strip = 0; index = 0; return; }
        strip = GetEdgeStrip(x, 0, length, 0);
    }
}
static void Iterate(Strip*& strip, long& index, long x, long length)
{
    long vert = strip->verts[index];
    if (index < strip->count - 1) { index++; return; }
    length = length - (strip->start + strip->size) + x; x = strip->start + strip->size;
    if (x == vert) { x++; length--; }
    if (length > 0) FindFirst(strip, index, x, length); else { strip = 0; index = 0; }
}
static int polys[32][3], npolys, nverts;
static void AddPoly(long a, long b, long c) { polys[npolys][0] = a; polys[npolys][1] = b; polys[npolys][2] = c; npolys++; }

static void TesselateEdge(const int* pos, const int* fanbase, int count, bool flip, unsigned short* indices)
{
    long length = 8; Strip* as; long ai = 0; FindFirst(as, ai, 0, length);
    long previous = -1;
    for (long i = 0; i < count - 1; i++) {
        long start = pos[i], end = pos[i + 1];
        if (end - start > 1) {
            bool found = false; long fan = -1;
            while (as && as->verts[ai] < end) {
                if (as->verts[ai] > start) {
                    found = true;
                    if (fan < 0) fan = 100 + i;
                    if (previous < 0) previous = nverts++;
                    long added = nverts++;
                    if (flip) AddPoly(fan, added, previous); else AddPoly(fan, previous, added);
                    previous = added;
                }
                Iterate(as, ai, 0, length);
            }
            if (found) {
                long added = nverts++;
                if (flip) AddPoly(fan, added, previous); else AddPoly(fan, previous, added);
                previous = added;
                indices[i] = fanbase[i] != 0xffff ? (unsigned short)(200 + i) : 0xffff;
                continue;
            }
            indices[i] = (unsigned short)(50 + i);
        }
        previous = -1;
    }
}

int main()
{
    int pos[3] = { 0, 4, 8 }; int fb[3] = { 0, 0xffff, 0 }; unsigned short idx[3] = { 9, 9, 9 };
    TesselateEdge(pos, fb, 3, false, idx);
    // span 0..4 holds adjacent verts 1 and 3 (3 triangles); span 4..8 holds 6 (2 triangles), the second span has no fan base
    if (npolys != 5 || idx[0] != 200 || idx[1] != 0xffff || polys[0][0] != 100 || polys[0][1] != 0 || polys[0][2] != 1
        || polys[3][0] != 101 || polys[3][1] != 3 || polys[3][2] != 4) { printf("00be4fe0_TEST FAIL\n"); return 1; }
    npolys = 0; nverts = 0; memset(idx, 9, sizeof(idx));
    int pos2[3] = { 0, 1, 2 };
    TesselateEdge(pos2, fb, 3, true, idx);
    if (npolys != 0 || idx[0] != 0x0909 || idx[1] != 0x0909) { printf("00be4fe0_TEST FAIL\n"); return 1; }
    printf("00be4fe0_TEST PASS\n");
    return 0;
}
