// Behaviour test for 0x00BFD0B0 CPatchTesselationEdgeStrip::SaveVerts<CFanBaseVertex>: writes the vertex count; when there are vertices, the compressed size and the compressed bytes (the buffer is freed)
#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_written[8];
static int g_longs = 0;
static unsigned char g_bytes[64];
static long g_byteCount = -1;
static int g_compressCalls = 0;
class CDataOutputStream {
public:
    virtual void Write(const void* data, long size) { std::memcpy(g_bytes, data, size); g_byteCount = size; }
    virtual ~CDataOutputStream() { }
    void WriteLong(long value) { g_written[g_longs++] = value; }
};
template <class T> struct CArray {
    T* First;
    T* Last;
    T* End;
    CArray() : First(0), Last(0), End(0) { }
    ~CArray() { if (First) std::free(First); }
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};
class CRangeCompressor {
public:
    long Compress(void* data, long count, long stride, CArray<unsigned char>* output)
    {
        // stand-in compressor: one byte per vertex (its first byte), plus the stride
        ++g_compressCalls;
        long size = count + 1;
        output->First = (unsigned char*)std::malloc(size);
        long i;
        for (i = 0; i < count; ++i)
            output->First[i] = ((unsigned char*)data)[i * stride];
        output->First[count] = (unsigned char)stride;
        output->Last = output->End = output->First + size;
        return size;
    }
};
template <class T> class CEdgeStipVertexArray {
public:
    T* Data;
    long Count;
};
class CPatchTesselationEdgeStrip {
public:
    struct CFanBaseVertex {
        unsigned char _bytes[0x38];
    };
    template <class T> void SaveVerts(CEdgeStipVertexArray<T>& verts, CDataOutputStream& stream);
};
template <class T>
void CPatchTesselationEdgeStrip::SaveVerts(CEdgeStipVertexArray<T>& verts, CDataOutputStream& stream)
{
    stream.WriteLong(verts.Count);
    if (verts.Count > 0) {
        CRangeCompressor compressor;
        CArray<unsigned char> buffer;
        T* data = verts.Data;
        long size = compressor.Compress(data, verts.Count, sizeof(T), &buffer);
        stream.WriteLong(size);
        stream.Write(buffer.GetData(), size);
    }
}
int main()
{
    CPatchTesselationEdgeStrip strip;
    CDataOutputStream stream;
    CPatchTesselationEdgeStrip::CFanBaseVertex verts[3];
    std::memset(verts, 0, sizeof(verts));
    verts[0]._bytes[0] = 11; verts[1]._bytes[0] = 22; verts[2]._bytes[0] = 33;
    CEdgeStipVertexArray<CPatchTesselationEdgeStrip::CFanBaseVertex> none; none.Data = 0; none.Count = 0;
    strip.SaveVerts(none, stream);
    if (g_longs != 1 || g_written[0] != 0 || g_compressCalls != 0 || g_byteCount != -1) { std::printf("00bfd0b0_TEST FAIL\n"); return 1; }
    CEdgeStipVertexArray<CPatchTesselationEdgeStrip::CFanBaseVertex> three; three.Data = verts; three.Count = 3;
    strip.SaveVerts(three, stream);
    if (g_longs != 3 || g_written[1] != 3 || g_written[2] != 4 || g_byteCount != 4 || g_bytes[0] != 11 || g_bytes[2] != 33 || g_bytes[3] != 0x38)
        { std::printf("00bfd0b0_TEST FAIL\n"); return 1; }
    std::printf("00bfd0b0_TEST PASS\n");
    return 0;
}