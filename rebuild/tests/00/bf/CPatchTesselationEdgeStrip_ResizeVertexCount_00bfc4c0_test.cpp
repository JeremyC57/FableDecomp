#include <cstdio>
#include <cstring>
// CPatchTesselationEdgeStrip::ResizeVertexCount: each array resized to its own count, in layout order, one resource
class CMovableResource { public: long dummy; };
static char g_order[16]; static long g_n;
template <int K> class CEdgeStipVertexArray { public:
    void* Data; long Count; CMovableResource* LastResource;
    bool Resize(long count, CMovableResource* resource) { Count = count; LastResource = resource; g_order[g_n++] = (char)('0' + K); g_order[g_n] = 0; return true; }
};
class CPatchTesselationEdgeStrip { public:
    unsigned short StartPos; unsigned short Size;
    CEdgeStipVertexArray<0> Verts; CEdgeStipVertexArray<1> FanBaseVerts; CEdgeStipVertexArray<2> WaterVerts; CEdgeStipVertexArray<3> WaterFanBaseVerts;
    bool Tesselated; bool Vertical;
    void ResizeVertexCount(long verts, long fanBaseVerts, long waterVerts, long waterFanBaseVerts, CMovableResource* resource)
    {
        Verts.Resize(verts, resource);
        FanBaseVerts.Resize(fanBaseVerts, resource);
        WaterVerts.Resize(waterVerts, resource);
        WaterFanBaseVerts.Resize(waterFanBaseVerts, resource);
    }
};
int main()
{
    CMovableResource res; CPatchTesselationEdgeStrip s;
    s.ResizeVertexCount(5, 6, 7, 8, &res);
    bool ok = std::strcmp(g_order, "0123") == 0 && s.Verts.Count == 5 && s.FanBaseVerts.Count == 6 && s.WaterVerts.Count == 7
        && s.WaterFanBaseVerts.Count == 8 && s.Verts.LastResource == &res && s.WaterVerts.LastResource == &res;
    if (!ok) { std::printf("00bfc4c0_TEST FAIL\n"); return 1; }
    std::printf("00bfc4c0_TEST PASS\n");
    return 0;
}