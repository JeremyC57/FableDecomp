#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test: CEngineWaterBackgroundSubPatch::Save writes counts, type, and the compressed buffers for geometry types
static long g_longs[8]; static int g_nLongs = 0; static unsigned short g_shorts[4]; static int g_nShorts = 0;
static long g_written = 0; static int g_writes = 0, g_vlock = 0, g_vunlock = 0, g_ilock = 0, g_iunlock = 0;
static long g_lastStride = 0, g_lastCount = 0;
struct CDataOutputStream {
    void Write(const void*, long size) { g_written += size; ++g_writes; }
    void WriteUShort(unsigned short v) { g_shorts[g_nShorts++] = v; }
    void WriteLong(long v) { g_longs[g_nLongs++] = v; }
};
template <class T> struct CArray {
    T* First; T* Last; T* End;
    CArray() : First(0), Last(0), End(0) {}
    ~CArray() { if (First) std::free(First); }
    unsigned long Size() const { return (unsigned long)(Last - First); }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};
struct CRangeCompressor {
    long Compress(void*, long count, long stride, CArray<unsigned char>* out)
    {
        g_lastStride = stride; g_lastCount = count;
        if (out->First) std::free(out->First);
        out->First = (unsigned char*)std::malloc(10); out->Last = out->First + 10; out->End = out->Last;
        return count + stride;
    }
};
struct CVertexBufferWin32 { void* Lock(unsigned long, unsigned long, unsigned long) { ++g_vlock; return this; } void Unlock() { ++g_vunlock; } };
struct CIndexBuffer { void* Lock(unsigned long, unsigned long, unsigned long) { ++g_ilock; return this; } void Unlock() { ++g_iunlock; } };
struct CEngineWaterBackgroundSubPatch {
    CIndexBuffer* IndexBuffer; CVertexBufferWin32* VertexBuffer; unsigned short PolyCount, VertexCount; long WaterType;
    void Save(CDataOutputStream& stream);
};
void CEngineWaterBackgroundSubPatch::Save(CDataOutputStream& stream)
{
    stream.WriteUShort(VertexCount);
    stream.WriteUShort(PolyCount);
    stream.WriteLong(WaterType);
    long stride;
    switch (WaterType) {
    case 1: case 2: case 6: case 8: stride = 0x38; break;
    case 3: case 4: case 5: stride = 0xc; break;
    default: return;
    }
    CRangeCompressor compressor;
    CArray<unsigned char> buffer;
    void* vertices = VertexBuffer->Lock(0, 0, 0);
    long size = compressor.Compress(vertices, VertexCount, stride, &buffer);
    stream.WriteLong(stride);
    stream.WriteLong(size);
    stream.Write(buffer.GetData(), size);
    VertexBuffer->Unlock();
    if (PolyCount != 0) {
        void* indices = IndexBuffer->Lock(0, 0, 0);
        size = compressor.Compress(indices, PolyCount * 3, 2, &buffer);
        stream.WriteLong(size);
        stream.Write(buffer.GetData(), size);
        IndexBuffer->Unlock();
    }
}
int main()
{
    CIndexBuffer ib; CVertexBufferWin32 vb; CDataOutputStream s;
    CEngineWaterBackgroundSubPatch p; p.IndexBuffer = &ib; p.VertexBuffer = &vb; p.PolyCount = 4; p.VertexCount = 9; p.WaterType = 3;
    p.Save(s);
    bool ok = g_nShorts == 2 && g_shorts[0] == 9 && g_shorts[1] == 4 && g_nLongs == 4 && g_longs[0] == 3 && g_longs[1] == 0xc
        && g_longs[2] == 9 + 0xc && g_longs[3] == 12 + 2 && g_lastStride == 2 && g_lastCount == 12 && g_writes == 2
        && g_vlock == 1 && g_vunlock == 1 && g_ilock == 1 && g_iunlock == 1;
    g_nShorts = g_nLongs = 0; p.WaterType = 7; p.Save(s);
    ok = ok && g_nLongs == 1 && g_vlock == 1;
    g_nShorts = g_nLongs = 0; p.WaterType = 8; p.PolyCount = 0; p.Save(s);
    ok = ok && g_nLongs == 3 && g_longs[1] == 0x38 && g_ilock == 1 && g_vlock == 2;
    if (!ok) { std::printf("00bfd670_TEST FAIL\n"); return 1; }
    std::printf("00bfd670_TEST PASS\n");
    return 0;
}