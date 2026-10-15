#include <cstdio>
#include <cstring>
// CPatchTesselationEdgeStrip::Reset: all four vertex arrays resized to size+1 (Verts, WaterVerts, FanBaseVerts,
// WaterFanBaseVerts), then StartPos / Size stored as words, Tesselated cleared, Vertical set
struct CMovableResource { long dummy; };
static char g_order[16]; static long g_n;
template <int K> struct CEdgeStipVertexArray {
    void* Data; long Count; CMovableResource* LastResource;
    bool Resize(long count, CMovableResource* resource) { Count = count; LastResource = resource; g_order[g_n++] = (char)('0' + K); g_order[g_n] = 0; return true; }
};
struct CPatchTesselationEdgeStrip {
    unsigned short StartPos; unsigned short Size;
    CEdgeStipVertexArray<0> Verts; CEdgeStipVertexArray<1> FanBaseVerts; CEdgeStipVertexArray<2> WaterVerts; CEdgeStipVertexArray<3> WaterFanBaseVerts;
    bool Tesselated; bool Vertical;
    void Reset(long startPos, long size, bool vertical, CMovableResource* resource)
    {
        Verts.Resize(size + 1, resource);
        WaterVerts.Resize(size + 1, resource);
        FanBaseVerts.Resize(size + 1, resource);
        WaterFanBaseVerts.Resize(size + 1, resource);
        Size = (unsigned short)size;
        StartPos = (unsigned short)startPos;
        Tesselated = false;
        Vertical = vertical;
    }
};
int main()
{
    CMovableResource res; CPatchTesselationEdgeStrip s; s.Tesselated = true; s.Vertical = false;
    s.Reset(0x10004, 9, true, &res);
    bool ok = std::strcmp(g_order, "0213") == 0 && s.Verts.Count == 10 && s.FanBaseVerts.Count == 10 && s.WaterVerts.Count == 10
        && s.WaterFanBaseVerts.Count == 10 && s.WaterFanBaseVerts.LastResource == &res && s.StartPos == 4 && s.Size == 9 && !s.Tesselated && s.Vertical;
    if (!ok) { std::printf("00bfc3b0_TEST FAIL\n"); return 1; }
    std::printf("00bfc3b0_TEST PASS\n");
    return 0;
}