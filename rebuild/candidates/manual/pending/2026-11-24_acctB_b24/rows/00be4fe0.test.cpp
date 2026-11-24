// Behaviour test for CWaterPatchTesselatorBase<CTVertexSeaBackground>::TesselateEdge (0x00BE4FE0). The production texts are copied verbatim
// (TesselateEdge, the sea AddDuplicateEdgeVertex / AddFanBaseVertex, the shared FindFirstWaterAdjacentVertex / IterateThroughAdjacentVerts,
// FindWaterVert, AddPoly) over fixture classes with the PDB member names; CEngineLandscapeMap::GetEdgeStrip is a fixture that hands out
// adjacent strips by position. Checks: the bridging fans (vertex order, winding with and without `flip`), the edge index redirection
// (water fan base / 0xffff / plain edge vertex), the skipped spans and the walk across two adjacent strips.
#include <stdio.h>
#include <vector>
#define FABLE_FORCEINLINE inline
enum LANDSCAPE_EDGE_STRIP_SIDE { SIDE_TOP };
template <class T> class CArray : public std::vector<T> {
public:
    void push_back(const T& v) { std::vector<T>::push_back(v); }
    void push_back() { T element; push_back(element); }
};
template <class T> class CEdgeStipVertexArray : public std::vector<T> {
public:
    long Size() const { return (long)this->size(); }
};
class CPatchTesselationEdgeStrip {
public:
    struct CVertex { float WorldZ; unsigned long Normal; unsigned short WorldX, WorldY, VertexBufferIndex, IndexBufferIndex, FanBaseIndex; unsigned char U, V; };
    struct CFanBaseVertex { float WorldZ; unsigned long Normal; unsigned short WorldX, WorldY, VertexBufferIndex; unsigned char U, V; };
    struct CWaterVertex { float Z; float ShoreLookUpArray[12]; unsigned short EdgeVertexBufferIndex, EdgeIndexBufferIndex, FanBaseIndex, LandscapeVertexIndex; };
    struct CWaterFanBaseVertex { float Z; float ShoreLookUpArray[12]; unsigned short VertexBufferIndex; };
    unsigned short StartPos, Size;
    CEdgeStipVertexArray<CVertex> Verts;
    CEdgeStipVertexArray<CFanBaseVertex> FanBaseVerts;
    CEdgeStipVertexArray<CWaterVertex> WaterVerts;
    CEdgeStipVertexArray<CWaterFanBaseVertex> WaterFanBaseVerts;
    bool Vertical;
    CWaterVertex& GetWaterVertex(unsigned long index) { return WaterVerts[index]; }
    CVertex& GetVert(unsigned long index) { return Verts[index]; }
    CFanBaseVertex& GetFanBaseVert(unsigned long index) { return FanBaseVerts[index]; }
    CWaterFanBaseVertex& GetWaterFanBaseVert(unsigned long index) { return WaterFanBaseVerts[index]; }
    unsigned long FindWaterVert(unsigned long pos) const;
    long GetMainAxisVertexWorldPos(unsigned long index) { return Vertical ? Verts[index].WorldY : Verts[index].WorldX; }
    bool PeekIsVertical() const { return Vertical; }
    long PeekSize() const { return Size; }
    long PeekEndPos() const { return StartPos + Size; }
    long PeekWaterVertexCount() const { return WaterVerts.Size(); }
};
FABLE_FORCEINLINE unsigned long CPatchTesselationEdgeStrip::FindWaterVert(unsigned long pos) const
{
    unsigned long i = 0;
    if (WaterVerts.Size() == 0) return 0;
    if (Vertical) {
        while (i < (unsigned long)WaterVerts.Size() && Verts[WaterVerts[i].LandscapeVertexIndex].WorldY < pos) i++;
    } else {
        while (i < (unsigned long)WaterVerts.Size() && Verts[WaterVerts[i].LandscapeVertexIndex].WorldX < pos) i++;
    }
    return i;
}

static CPatchTesselationEdgeStrip* g_adjacent[4]; static int g_adjacent_count, g_edge_strip_calls;
class CEngineLandscapeMap {
public:
    CPatchTesselationEdgeStrip* GetEdgeStrip(long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side)
    {
        g_edge_strip_calls++;
        for (int i = 0; i < g_adjacent_count; i++)
            if (length > 0 && x >= g_adjacent[i]->StartPos && x < g_adjacent[i]->PeekEndPos()) return g_adjacent[i];
        return 0;
    }
};
class CEngineMap { public: CEngineLandscapeMap* LandscapeMap; CEngineLandscapeMap* GetLandscapeMap() const { return LandscapeMap; } };
class CTesselationData { public: long Width, Height; CEngineMap* EngineMap;
    long PeekWidth() { return Width; } long PeekHeight() { return Height; } const CEngineMap* PeekEngineMap() { return EngineMap; } };
class CTVertexSeaBackground { public: float X, Y, Z; };
class CLandscapeTesselatorBase {
public:
    void AddPoly(long a, long b, long c);
    CArray<unsigned short> BridgeIndexBuffer;
    CTesselationData* Data;
};
FABLE_FORCEINLINE void CLandscapeTesselatorBase::AddPoly(long a, long b, long c)
{
    BridgeIndexBuffer.push_back((unsigned short)a);
    BridgeIndexBuffer.push_back((unsigned short)b);
    BridgeIndexBuffer.push_back((unsigned short)c);
}

template <class T> class CWaterPatchTesselatorBase : public CLandscapeTesselatorBase {
public:
    void TesselateEdge(CPatchTesselationEdgeStrip* strip, long a, long b, bool c, bool d, LANDSCAPE_EDGE_STRIP_SIDE side, unsigned short* indices);
    void FindFirstWaterAdjacentVertex(CPatchTesselationEdgeStrip*& strip, long& index, const CEngineMap* map, bool vertical, long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side);
    void IterateThroughAdjacentVerts(CPatchTesselationEdgeStrip*& strip, long& index, const CEngineMap* map, long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side);
    unsigned short AddDuplicateEdgeVertex(CPatchTesselationEdgeStrip* strip, long index);
    unsigned short AddFanBaseVertex(CPatchTesselationEdgeStrip* strip, long index);
    CArray<T> BridgeVertexBuffer;
};
template <class T>
FABLE_FORCEINLINE void CWaterPatchTesselatorBase<T>::FindFirstWaterAdjacentVertex(CPatchTesselationEdgeStrip*& strip, long& index, const CEngineMap* map, bool vertical, long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side)
{
    strip = map->GetLandscapeMap()->GetEdgeStrip(x, y, length, side);
    while (strip) {
        index = strip->FindWaterVert(vertical ? y : x);
        if (index < strip->PeekWaterVertexCount()) return;
        long size = strip->PeekSize();
        if (vertical) y += size;
        else x += size;
        length -= size;
        if (length <= 0) {
            strip = 0;
            index = 0;
            return;
        }
        strip = map->GetLandscapeMap()->GetEdgeStrip(x, y, length, side);
    }
}
template <class T>
FABLE_FORCEINLINE void CWaterPatchTesselatorBase<T>::IterateThroughAdjacentVerts(CPatchTesselationEdgeStrip*& strip, long& index, const CEngineMap* map, long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side)
{
    CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(strip->GetWaterVertex(index).LandscapeVertexIndex);
    if (index < strip->PeekWaterVertexCount() - 1) {
        index++;
        return;
    }
    bool vertical = strip->PeekIsVertical();
    if (vertical) {
        length = length - strip->PeekEndPos() + y;
        y = strip->PeekEndPos();
        if (y == vert.WorldY) {
            y++;
            length--;
        }
    } else {
        length = length - strip->PeekEndPos() + x;
        x = strip->PeekEndPos();
        if (x == vert.WorldX) {
            x++;
            length--;
        }
    }
    if (length > 0) {
        FindFirstWaterAdjacentVertex(strip, index, map, vertical, x, y, length, side);
    } else {
        strip = 0;
        index = 0;
    }
}
template <>
FABLE_FORCEINLINE unsigned short CWaterPatchTesselatorBase<CTVertexSeaBackground>::AddDuplicateEdgeVertex(CPatchTesselationEdgeStrip* strip, long index)
{
    long new_index = BridgeVertexBuffer.size();
    BridgeVertexBuffer.push_back();
    CTVertexSeaBackground& v = BridgeVertexBuffer[new_index];
    CPatchTesselationEdgeStrip::CWaterVertex& water = strip->GetWaterVertex(index);
    CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(water.LandscapeVertexIndex);
    v.X = vert.WorldX;
    v.Y = vert.WorldY;
    v.Z = water.Z;
    return (unsigned short)new_index;
}

template <>
FABLE_FORCEINLINE unsigned short CWaterPatchTesselatorBase<CTVertexSeaBackground>::AddFanBaseVertex(CPatchTesselationEdgeStrip* strip, long index)
{
    long new_index = BridgeVertexBuffer.size();
    BridgeVertexBuffer.push_back();
    CTVertexSeaBackground& v = BridgeVertexBuffer[new_index];
    CPatchTesselationEdgeStrip::CWaterVertex& water = strip->GetWaterVertex(index);
    CPatchTesselationEdgeStrip::CWaterFanBaseVertex& water_fan = strip->GetWaterFanBaseVert(water.FanBaseIndex);
    CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(water.LandscapeVertexIndex);
    CPatchTesselationEdgeStrip::CFanBaseVertex& fan = strip->GetFanBaseVert(vert.FanBaseIndex);
    v.X = fan.WorldX;
    v.Y = fan.WorldY;
    v.Z = water_fan.Z;
    return (unsigned short)new_index;
}

template <class T>
void CWaterPatchTesselatorBase<T>::TesselateEdge(CPatchTesselationEdgeStrip* strip, long x, long y, bool vertical, bool flip, LANDSCAPE_EDGE_STRIP_SIDE side, unsigned short* indices)
{
    long length = vertical ? Data->PeekHeight() : Data->PeekWidth();
    const CEngineMap* map = Data->PeekEngineMap();
    CPatchTesselationEdgeStrip* adjacent_strip;
    long adjacent_index = 0;
    FindFirstWaterAdjacentVertex(adjacent_strip, adjacent_index, map, vertical, x, y, length, side);
    long previous = -1;
    for (long i = 0; i < strip->PeekWaterVertexCount() - 1; i++) {
        CPatchTesselationEdgeStrip::CWaterVertex& water = strip->GetWaterVertex(i);
        if (water.EdgeIndexBufferIndex != 0xffff) {
            long start = strip->GetMainAxisVertexWorldPos(strip->GetWaterVertex(i).LandscapeVertexIndex);
            long end = strip->GetMainAxisVertexWorldPos(strip->GetWaterVertex(i + 1).LandscapeVertexIndex);
            if (end - start > 1 && strip->GetWaterVertex(i + 1).LandscapeVertexIndex == water.LandscapeVertexIndex + 1) {
                bool found = false;
                long fan = -1;
                while (adjacent_strip && adjacent_strip->GetMainAxisVertexWorldPos(adjacent_strip->GetWaterVertex(adjacent_index).LandscapeVertexIndex) < end) {
                    if (adjacent_strip->GetMainAxisVertexWorldPos(adjacent_strip->GetWaterVertex(adjacent_index).LandscapeVertexIndex) > start) {
                        found = true;
                        if (fan < 0) fan = AddFanBaseVertex(strip, i);
                        if (previous < 0) previous = AddDuplicateEdgeVertex(strip, i);
                        long added = AddDuplicateEdgeVertex(adjacent_strip, adjacent_index);
                        if (flip) AddPoly(fan, added, previous);
                        else AddPoly(fan, previous, added);
                        previous = added;
                    }
                    IterateThroughAdjacentVerts(adjacent_strip, adjacent_index, map, x, y, length, side);
                }
                if (found) {
                    long added = AddDuplicateEdgeVertex(strip, i + 1);
                    if (flip) AddPoly(fan, added, previous);
                    else AddPoly(fan, previous, added);
                    previous = added;
                    if (water.FanBaseIndex != 0xffff) indices[water.EdgeIndexBufferIndex] = strip->GetWaterFanBaseVert(water.FanBaseIndex).VertexBufferIndex;
                    else indices[water.EdgeIndexBufferIndex] = 0xffff;
                } else {
                    previous = -1;
                    indices[water.EdgeIndexBufferIndex] = water.EdgeVertexBufferIndex;
                }
                continue;
            }
        }
        previous = -1;
    }
}

template void CWaterPatchTesselatorBase<CTVertexSeaBackground>::TesselateEdge(CPatchTesselationEdgeStrip*, long, long, bool, bool, LANDSCAPE_EDGE_STRIP_SIDE, unsigned short*);

static CPatchTesselationEdgeStrip::CVertex V(unsigned short x, unsigned short fan) { CPatchTesselationEdgeStrip::CVertex v = { 0, 0, x, 7, 0, 0, fan, 0, 0 }; return v; }
static CPatchTesselationEdgeStrip::CWaterVertex WV(unsigned short lvi, unsigned short eibi, unsigned short evbi, unsigned short fbi, float z)
{ CPatchTesselationEdgeStrip::CWaterVertex w; w.Z = z; w.LandscapeVertexIndex = lvi; w.EdgeIndexBufferIndex = eibi; w.EdgeVertexBufferIndex = evbi; w.FanBaseIndex = fbi; return w; }
static void Strip(CPatchTesselationEdgeStrip& s, unsigned short start, unsigned short size, const unsigned short* xs, int n)
{
    s.StartPos = start; s.Size = size; s.Vertical = false;
    for (int i = 0; i < n; i++) { s.Verts.push_back(V(xs[i], 0)); s.WaterVerts.push_back(WV((unsigned short)i, 0xffff, 0, 0, 10.0f + i)); }
}
static int Check(const CArray<unsigned short>& got, const unsigned short* want, int n)
{
    if ((int)got.size() != n) return 0;
    for (int i = 0; i < n; i++) if (got[i] != want[i]) return 0;
    return 1;
}
int main()
{
    int ok = 1;
    // This strip: water vertices at x = 0, 4, 8 (edge indices 0, 1, 2; the middle one has no water fan base), fan base at x = 2 / 6.
    CPatchTesselationEdgeStrip s; s.StartPos = 0; s.Size = 8; s.Vertical = false;
    s.Verts.push_back(V(0, 0)); s.Verts.push_back(V(4, 1)); s.Verts.push_back(V(8, 1));
    CPatchTesselationEdgeStrip::CFanBaseVertex f0 = { 0, 0, 2, 9, 0, 0, 0 }, f1 = { 0, 0, 6, 9, 0, 0, 0 };
    s.FanBaseVerts.push_back(f0); s.FanBaseVerts.push_back(f1);
    s.WaterVerts.push_back(WV(0, 0, 50, 0, 1.0f)); s.WaterVerts.push_back(WV(1, 1, 51, 0xffff, 2.0f)); s.WaterVerts.push_back(WV(2, 2, 52, 0, 3.0f));
    s.WaterFanBaseVerts.resize(0x10000); s.WaterFanBaseVerts[0].VertexBufferIndex = 200; s.WaterFanBaseVerts[0].Z = 5.0f; s.WaterFanBaseVerts[0xffff].Z = 6.0f;
    // Adjacent strips: A (0..4) water at x = 1, 3; B (4..8) water at x = 6.
    CPatchTesselationEdgeStrip a, b; unsigned short ax[2] = { 1, 3 }, bx[1] = { 6 };
    Strip(a, 0, 4, ax, 2); Strip(b, 4, 4, bx, 1);
    g_adjacent[0] = &a; g_adjacent[1] = &b; g_adjacent_count = 2;
    CEngineLandscapeMap landscape; CEngineMap map; map.LandscapeMap = &landscape;
    CTesselationData data; data.Width = 8; data.Height = 99; data.EngineMap = &map;
    {
        CWaterPatchTesselatorBase<CTVertexSeaBackground> t; t.Data = &data;
        unsigned short idx[3] = { 9, 9, 9 };
        t.TesselateEdge(&s, 0, 0, false, false, SIDE_TOP, idx);
        unsigned short want[15] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 4, 6, 5, 6, 7 };
        ok &= Check(t.BridgeIndexBuffer, want, 15);
        ok &= idx[0] == 200 && idx[1] == 0xffff && idx[2] == 9;
        ok &= t.BridgeVertexBuffer.size() == 8;
        if (t.BridgeVertexBuffer.size() == 8) {
            ok &= t.BridgeVertexBuffer[0].X == 2.0f && t.BridgeVertexBuffer[0].Y == 9.0f && t.BridgeVertexBuffer[0].Z == 5.0f;   // fan base of span 0
            ok &= t.BridgeVertexBuffer[1].X == 0.0f && t.BridgeVertexBuffer[1].Z == 1.0f;                                        // duplicate of this strip's x = 0
            ok &= t.BridgeVertexBuffer[2].X == 1.0f && t.BridgeVertexBuffer[3].X == 3.0f && t.BridgeVertexBuffer[6].X == 6.0f;  // adjacent vertices
            ok &= t.BridgeVertexBuffer[5].X == 6.0f && t.BridgeVertexBuffer[5].Z == 6.0f && t.BridgeVertexBuffer[7].X == 8.0f;  // fan base of span 1, last edge vertex
        }
    }
    {
        CWaterPatchTesselatorBase<CTVertexSeaBackground> t; t.Data = &data;
        unsigned short idx[3] = { 9, 9, 9 };
        t.TesselateEdge(&s, 0, 0, false, true, SIDE_TOP, idx);   // flip: (fan, added, previous)
        unsigned short want[15] = { 0, 2, 1, 0, 3, 2, 0, 4, 3, 5, 6, 4, 5, 7, 6 };
        ok &= Check(t.BridgeIndexBuffer, want, 15);
    }
    {
        // No adjacent vertex strictly inside a span: plain edge vertex indices, no polygons.
        CPatchTesselationEdgeStrip c; unsigned short cx[1] = { 0 }; Strip(c, 0, 8, cx, 1);
        g_adjacent[0] = &c; g_adjacent_count = 1;
        CWaterPatchTesselatorBase<CTVertexSeaBackground> t; t.Data = &data;
        unsigned short idx[3] = { 9, 9, 9 };
        t.TesselateEdge(&s, 0, 0, false, false, SIDE_TOP, idx);
        ok &= t.BridgeIndexBuffer.size() == 0 && t.BridgeVertexBuffer.size() == 0 && idx[0] == 50 && idx[1] == 51 && idx[2] == 9;
        // Skipped spans: an edge index of 0xffff and a non-adjacent landscape vertex leave the indices untouched.
        s.WaterVerts[0].EdgeIndexBufferIndex = 0xffff; s.WaterVerts[1].LandscapeVertexIndex = 2;
        unsigned short idx2[3] = { 9, 9, 9 };
        t.TesselateEdge(&s, 0, 0, false, false, SIDE_TOP, idx2);
        ok &= idx2[0] == 9 && idx2[1] == 9 && idx2[2] == 9;
    }
    // Empty adjacency: no strip at all.
    {
        g_adjacent_count = 0; g_edge_strip_calls = 0;
        CWaterPatchTesselatorBase<CTVertexSeaBackground> t; t.Data = &data;
        unsigned short idx[3] = { 9, 9, 9 };
        s.WaterVerts[0].EdgeIndexBufferIndex = 0; s.WaterVerts[1].LandscapeVertexIndex = 1;
        t.TesselateEdge(&s, 0, 0, false, false, SIDE_TOP, idx);
        ok &= g_edge_strip_calls == 1 && idx[0] == 50 && idx[1] == 51;
    }
    printf(ok ? "00be4fe0_TEST PASS\n" : "00be4fe0_TEST FAIL\n");
    return ok ? 0 : 1;
}
