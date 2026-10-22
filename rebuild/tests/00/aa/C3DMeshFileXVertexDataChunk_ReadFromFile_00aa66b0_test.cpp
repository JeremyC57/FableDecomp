#include <cstdio>
#include <cstring>
#include <vector>
// Behaviour copy of C3DMeshFileXVertexDataChunk::ReadFromFile (0x00AA66B0): header dwords, version-gated params, bytes.
struct Stream {
    unsigned int StreamPos; const unsigned char* CurrentSourcePtr; int SourceChunkBytesRemaining; int overflowCalls;
    Stream(const unsigned char* d, int n) : StreamPos(0), CurrentSourcePtr(d), SourceChunkBytesRemaining(n), overflowCalls(0) {}
    void ReadWithSrcChunkOverflow(unsigned char* buf, int size) { ++overflowCalls; std::memset(buf, 0, size); StreamPos += size; }
    void Read(void* buf, int size) {
        if (size <= 0) return;
        long long np = (long long)(StreamPos + size);
        if (!(np >= 0 && np <= 0x7fffffff)) return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(buf, CurrentSourcePtr, size); CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buf, size);
    }
    unsigned int ReadULong() { unsigned int v; Read(&v, 4); return v; }
    float ReadFloat() { float v; Read(&v, 4); return v; }
    unsigned short ReadUShort() { unsigned short v; Read(&v, 2); return v; }
    bool ReadBool() { char v; Read(&v, 1); return v != 0; }
    unsigned int GetPosition() const { return StreamPos; }
};
struct Writer {
    unsigned char buf[512]; int n;
    Writer() : n(0) {}
    void U32(unsigned int v) { std::memcpy(buf + n, &v, 4); n += 4; }
    void F32(float v) { std::memcpy(buf + n, &v, 4); n += 4; }
    void U16(unsigned short v) { std::memcpy(buf + n, &v, 2); n += 2; }
    void U8(unsigned char v) { buf[n++] = v; }
};
struct Chunk {
    unsigned int FormatID, VertexCount, VertexSize, InitFlags; int paramsRead;
    std::vector<char> VertexData;
    Chunk() : paramsRead(0) {}
    void ReadParams(Stream& in) { ++paramsRead; in.ReadFloat(); }
    void ReadFromFile(Stream& in) {
        unsigned int version = in.ReadULong();
        FormatID = in.ReadULong(); VertexCount = in.ReadULong(); VertexSize = in.ReadULong(); InitFlags = in.ReadULong();
        if (version >= 2) ReadParams(in);
        int bytes = (int)(VertexCount * VertexSize);
        VertexData.resize(bytes, 0);
        if (bytes > 0) in.Read(&VertexData[0], bytes);
    }
};
int main() {
    Writer w; w.U32(2); w.U32(7); w.U32(3); w.U32(2); w.U32(9); w.F32(1.5f);
    for (int i = 0; i < 6; ++i) w.U8((unsigned char)(10 + i));
    Stream s(w.buf, w.n); Chunk c; c.ReadFromFile(s);
    bool ok = c.FormatID == 7 && c.VertexCount == 3 && c.VertexSize == 2 && c.InitFlags == 9 && c.paramsRead == 1
        && c.VertexData.size() == 6 && c.VertexData[5] == 15 && s.StreamPos == (unsigned int)w.n;
    Writer v; v.U32(1); v.U32(1); v.U32(1); v.U32(1); v.U32(0); v.U8(99);
    Stream t(v.buf, v.n); Chunk d; d.ReadFromFile(t);
    ok = ok && d.paramsRead == 0 && d.VertexData.size() == 1 && d.VertexData[0] == 99;
    if (!ok) { std::printf("00aa66b0_TEST FAIL\n"); return 1; }
    std::printf("00aa66b0_TEST PASS\n"); return 0;
}