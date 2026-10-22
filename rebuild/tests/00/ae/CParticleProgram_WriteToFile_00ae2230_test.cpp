// Behaviour test for CParticleProgram::WriteToFile (0x00AE2230): records the stream calls and checks the record order,
// the counts, the raw byte lengths and the empty-array data pointer (the array object itself).
#include <cstdio>
struct CCharString { int m_id; };
struct C4DVector { float X, Y, Z, W; };
struct C2DVector { float X, Y; };
struct C3DTriangle2 { unsigned short Index[3]; };
struct C3DQuad2 { unsigned short Index[4]; };
struct C3DClothRenderVertex { unsigned short Particle; unsigned short TexCoord; };
struct C3DGroup2 { int m_id; };
static unsigned long g_log[256];
static const void* g_ptr[64];
static int g_n = 0, g_np = 0;
class CDataOutputStream {
public:
    virtual void Write(const void* data, long length) { g_log[g_n++] = 1000 + length; g_ptr[g_np++] = data; }
    void WriteEBOOL(bool value) { g_log[g_n++] = 2000 + value; }
    void WriteULong(unsigned long value) { g_log[g_n++] = value; }
    void WriteFloat(float value) { g_log[g_n++] = 3000 + (unsigned long)value; }
    void WriteNullTerminatedString(const CCharString& text) { g_log[g_n++] = 4000 + text.m_id; }
};
class CMemoryDataOutputStream : public CDataOutputStream {
public:
    char m_bytes[5];
    long GetSize() { return 5; }
    void* PeekData() { return m_bytes; }
};
template <class T> struct CArray {
    T* m_first;
    T* m_last;
    T* m_end;
    unsigned long size() const { return m_last - m_first; }
    const T* GetData() const { return size() > 0 ? m_first : (const T*)this; }
};
struct CParticleSimulation {
    void WriteToFile(CDataOutputStream& stream) { stream.WriteULong(777); }
};
struct CExportParticle { CCharString first; unsigned long second; };
static void WriteVectors_00ae18c0(const C4DVector* vectors, unsigned long count, CDataOutputStream& stream) { stream.WriteULong(5000 + count); }
static void WriteGroups(CDataOutputStream& stream, const CArray<C3DGroup2>* groups) { stream.WriteULong(6000 + groups->size()); }
class CParticleProgram {
public:
    CParticleSimulation           InitialSimulation;
    CArray<CExportParticle>       ExportParticles;
    CArray<C3DTriangle2>          RenderTris;
    CArray<C3DQuad2>              RenderQuads;
    CArray<C3DClothRenderVertex>  RenderVertices;
    unsigned long                 NonSimCount;
    C4DVector*                    NonSimPositions;
    CArray<C2DVector>             IndexedTextureCoords;
    CArray<C4DVector>             TempVertexPositions;
    CArray<int>                  ParticleIndices;
    CArray<int>                  VertexIndices;
    bool                          BezierEnable;
    float                         AveragePatchSize;
    CArray<C3DGroup2>             SimGroups;
    CArray<C3DGroup2>             NonSimGroups;
    CMemoryDataOutputStream       Constraints;
    void WriteToFile(CDataOutputStream& stream);
};
void CParticleProgram::WriteToFile(CDataOutputStream& stream)
{
    stream.WriteULong(0x14d);
    stream.WriteULong(Constraints.GetSize());
    stream.Write(Constraints.PeekData(), Constraints.GetSize());
    InitialSimulation.WriteToFile(stream);
    stream.WriteULong(RenderTris.size());
    {
        const void* data = RenderTris.GetData();
        const CArray<C3DTriangle2>* const& array = &RenderTris;
        stream.Write(data, array->size() * sizeof(C3DTriangle2));
    }
    stream.WriteULong(RenderQuads.size());
    {
        const void* data = RenderQuads.GetData();
        const CArray<C3DQuad2>* const& array = &RenderQuads;
        stream.Write(data, array->size() * sizeof(C3DQuad2));
    }
    stream.WriteULong(RenderVertices.size());
    {
        const void* data = RenderVertices.GetData();
        const CArray<C3DClothRenderVertex>* const& array = &RenderVertices;
        stream.Write(data, array->size() * sizeof(C3DClothRenderVertex));
    }
    stream.WriteULong(NonSimCount);
    WriteVectors_00ae18c0(NonSimPositions, NonSimCount, stream);
    stream.WriteULong(IndexedTextureCoords.size());
    {
        const void* data = IndexedTextureCoords.GetData();
        const CArray<C2DVector>* const& array = &IndexedTextureCoords;
        stream.Write(data, array->size() * sizeof(C2DVector));
    }
    stream.WriteULong(ParticleIndices.size());
    {
        const void* data = ParticleIndices.GetData();
        const CArray<int>* const& array = &ParticleIndices;
        stream.Write(data, array->size() * sizeof(int));
    }
    stream.WriteULong(VertexIndices.size());
    {
        const void* data = VertexIndices.GetData();
        const CArray<int>* const& array = &VertexIndices;
        stream.Write(data, array->size() * sizeof(int));
    }
    stream.WriteFloat(AveragePatchSize);
    stream.WriteEBOOL(BezierEnable);
    stream.WriteULong(ExportParticles.size());
    for (unsigned long i = 0; i < ExportParticles.size(); ++i) {
        stream.WriteNullTerminatedString(ExportParticles.m_first[i].first);
        stream.WriteULong(ExportParticles.m_first[i].second);
    }
    WriteGroups(stream, &SimGroups);
    WriteGroups(stream, &NonSimGroups);
}

template <class T> static void Set(CArray<T>& a, T* p, int n) { a.m_first = p; a.m_last = p + n; a.m_end = p + n; }
int main()
{
    static C3DTriangle2 tris[3];
    static C3DQuad2 quads[2];
    static C2DVector coords[4];
    static int indices[7];
    static CExportParticle exports[2];
    static C3DGroup2 groups[3];
    static CParticleProgram p;
    Set(p.RenderTris, tris, 3);
    Set(p.RenderQuads, quads, 2);
    Set(p.RenderVertices, (C3DClothRenderVertex*)0, 0);
    Set(p.IndexedTextureCoords, coords, 4);
    Set(p.ParticleIndices, indices, 7);
    Set(p.VertexIndices, indices, 0);
    exports[0].first.m_id = 11; exports[0].second = 21;
    exports[1].first.m_id = 12; exports[1].second = 22;
    Set(p.ExportParticles, exports, 2);
    Set(p.SimGroups, groups, 3);
    Set(p.NonSimGroups, groups, 1);
    p.NonSimCount = 9; p.BezierEnable = true; p.AveragePatchSize = 4.0f;
    CDataOutputStream out;
    p.WriteToFile(out);
    static const unsigned long expect[] = { 0x14d, 5, 1005, 777, 3, 1018, 2, 1016, 0, 1000, 9, 5009, 4, 1032, 7, 1028,
        0, 1000, 3004, 2001, 2, 4011, 21, 4012, 22, 6003, 6001 };
    int bad = 0;
    int n = (int)(sizeof(expect) / sizeof(expect[0]));
    if (g_n != n) bad = 1;
    for (int i = 0; i < n && !bad; ++i)
        if (g_log[i] != expect[i]) bad = 10 + i;
    if (g_ptr[0] != p.Constraints.m_bytes || g_ptr[1] != tris || g_ptr[3] != (const void*)&p.RenderVertices) bad = 2;
    if (g_ptr[6] != (const void*)&p.VertexIndices || g_ptr[4] != coords) bad = 3;
    if (bad) { std::printf("00ae2230_TEST FAIL\n"); return 1; }
    std::printf("00ae2230_TEST PASS\n");
    return 0;
}