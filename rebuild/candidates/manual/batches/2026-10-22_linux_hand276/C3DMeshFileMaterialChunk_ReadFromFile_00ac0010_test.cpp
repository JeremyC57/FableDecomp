#include <cstdio>
#include <cstring>
// Behaviour copy of C3DMeshFileMaterialChunk::ReadFromFile (0x00AC0010): name, index, bool, three colours, three floats.
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
    int nameReads; int MaterialIndex; bool TwoSided; unsigned int Ambient, Diffuse, Specular; float Shiny, ShinyStrength, Transparency;
    unsigned int baseSize;
    void ReadFromFile(Stream& in, unsigned int chunkSize) {
        unsigned int start = in.GetPosition();
        while (true) { unsigned char ch; in.Read(&ch, 1); if (!ch) break; ++nameReads; }
        MaterialIndex = (int)in.ReadULong(); TwoSided = in.ReadBool();
        Ambient = in.ReadULong(); Diffuse = in.ReadULong(); Specular = in.ReadULong();
        Shiny = in.ReadFloat(); ShinyStrength = in.ReadFloat(); Transparency = in.ReadFloat();
        baseSize = chunkSize - (in.GetPosition() - start);
    }
};
int main() {
    Writer w; w.U8('a'); w.U8('b'); w.U8(0); w.U32(3); w.U8(2); w.U32(0xff0000ffu); w.U32(2); w.U32(3); w.F32(0.5f); w.F32(1.5f); w.F32(0.25f);
    Stream s(w.buf, w.n); Chunk c; c.nameReads = 0; c.ReadFromFile(s, 64);
    bool ok = c.nameReads == 2 && c.MaterialIndex == 3 && c.TwoSided && c.Ambient == 0xff0000ffu && c.Specular == 3
        && c.Transparency == 0.25f && c.baseSize == 64 - (unsigned int)w.n;
    if (!ok) { std::printf("00ac0010_TEST FAIL\n"); return 1; }
    std::printf("00ac0010_TEST PASS\n"); return 0;
}
