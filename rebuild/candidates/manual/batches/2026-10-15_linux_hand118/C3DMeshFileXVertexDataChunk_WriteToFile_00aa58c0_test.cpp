// Behaviour test for 0x00AA58C0 C3DMeshFileXVertexDataChunk::WriteToFile
#include <cstdio>
#include <cstring>
struct COut {
    unsigned long u[16]; int nu; float f[16]; int nf; const void* data; unsigned long size; int dataCalls;
    COut() : nu(0), nf(0), data(0), size(0), dataCalls(0) {}
    void WriteULong(unsigned long v) { u[nu++] = v; }
    void WriteFloat(float v) { f[nf++] = v; }
    void WriteData(const void* d, unsigned long n) { data = d; size = n; ++dataCalls; }
};
struct CVertexCompressionParams { float PosScaleX, PosScaleY, PosScaleZ, PosScaleW, PosBiasX, PosBiasY, PosBiasZ, PosBiasW; };
static void WriteParams(const CVertexCompressionParams& p, COut& out)
{
    out.WriteFloat(p.PosScaleX); out.WriteFloat(p.PosScaleY); out.WriteFloat(p.PosScaleZ); out.WriteFloat(p.PosScaleW);
    out.WriteFloat(p.PosBiasX); out.WriteFloat(p.PosBiasY); out.WriteFloat(p.PosBiasZ); out.WriteFloat(p.PosBiasW);
}
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T* GetData() const { return Size() != 0 ? m_begin : (const T*)this; }
};
struct C3DMeshFileXVertexDataChunk {
    unsigned long FormatID, VertexCount, VertexSize, InitFlags;
    CVertexCompressionParams VBCParams;
    CArray<char> VertexData;
    void WriteToFile(COut& out) const
    {
        out.WriteULong(2); out.WriteULong(FormatID); out.WriteULong(VertexCount); out.WriteULong(VertexSize); out.WriteULong(InitFlags);
        WriteParams(VBCParams, out);
        out.WriteData(VertexData.GetData(), VertexData.Size());
    }
};
int main()
{
    char bytes[10];
    C3DMeshFileXVertexDataChunk c; c.FormatID = 7; c.VertexCount = 3; c.VertexSize = 12; c.InitFlags = 1;
    CVertexCompressionParams p = { 1, 2, 3, 4, 5, 6, 7, 8 }; c.VBCParams = p;
    c.VertexData.m_begin = bytes; c.VertexData.m_end = bytes + 10; c.VertexData.m_capacity = bytes + 10;
    COut out; c.WriteToFile(out);
    bool ok = out.nu == 5 && out.u[0] == 2 && out.u[1] == 7 && out.u[2] == 3 && out.u[3] == 12 && out.u[4] == 1
        && out.nf == 8 && out.f[0] == 1 && out.f[7] == 8 && out.dataCalls == 1 && out.data == bytes && out.size == 10;
    c.VertexData.m_end = c.VertexData.m_begin;
    COut o2; c.WriteToFile(o2);
    ok = ok && o2.size == 0 && o2.data == (const void*)&c.VertexData;
    if (!ok) { std::printf("00aa58c0_TEST FAIL\n"); return 1; }
    std::printf("00aa58c0_TEST PASS\n");
    return 0;
}
