// Behaviour test for ReadFromFile(CVertexCompressionParams&, CDataInputStream&) @ 0x00AA5460 (copy of the model).
#include <cstdio>
#include <cstring>
class CDataInputStream {
public:
    void*         __vftable;
    unsigned long StreamPos;
    long          StreamSize;
    void*         CurrentSourcePtr;
    unsigned long SourceChunkStreamPos;
    long          SourceChunkBytesRemaining;
    int           overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {   // test stand-in: copy from the source anyway and count the slow path
        ++overflowCalls;
        std::memcpy(buffer, CurrentSourcePtr, size);
        CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
        SourceChunkBytesRemaining = 1000;
        StreamPos += size;
    }
};
class CVertexCompressionParams {
public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW, PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
};
struct CInputStreamAccess : CDataInputStream {
    static bool IsValidPosition(long long pos) { return pos >= 0 && pos <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        if (size <= 0) return;
        long long newPosition = (long long)(StreamPos + size);
        if (!IsValidPosition(newPosition)) return;
        if (size <= SourceChunkBytesRemaining) {
            std::memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    float ReadFloat() { float value; Read(&value, 4); return value; }
};
static void ReadFromFile(CVertexCompressionParams& params, CDataInputStream& input)
{
    CInputStreamAccess& in = (CInputStreamAccess&)input;
    params.PosScaleX = in.ReadFloat();
    params.PosScaleY = in.ReadFloat();
    params.PosScaleZ = in.ReadFloat();
    params.PosScaleW = in.ReadFloat();
    params.PosBiasX = in.ReadFloat();
    params.PosBiasY = in.ReadFloat();
    params.PosBiasZ = in.ReadFloat();
    params.PosBiasW = in.ReadFloat();
}
int main()
{
    float src[8] = {1.5f, 2.5f, 3.5f, 4.5f, -1.0f, -2.0f, -3.0f, -4.0f};
    CDataInputStream s; std::memset(&s, 0, sizeof(s));
    s.CurrentSourcePtr = src; s.SourceChunkBytesRemaining = 12;   // three floats in the chunk, then the overflow path
    CVertexCompressionParams p; std::memset(&p, 0, sizeof(p));
    ReadFromFile(p, s);
    if (p.PosScaleX != 1.5f || p.PosScaleY != 2.5f || p.PosScaleZ != 3.5f || p.PosScaleW != 4.5f ||
        p.PosBiasX != -1.0f || p.PosBiasY != -2.0f || p.PosBiasZ != -3.0f || p.PosBiasW != -4.0f) { std::printf("00aa5460_TEST FAIL\n"); return 1; }
    if (s.StreamPos != 32 || s.overflowCalls != 1) { std::printf("00aa5460_TEST FAIL\n"); return 1; }
    std::printf("00aa5460_TEST PASS\n");
    return 0;
}