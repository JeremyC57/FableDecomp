#include <cstdio>
#include <cstring>
// Behaviour copy of C3DMeshFileDummyObjectChunk::ReadFromFile (0x00AA9E60): version gates the matrix and the bool.
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
    float Dir[3]; float M[12]; bool UseLocalOrigin; unsigned int baseSize;
    void ReadFromFile(Stream& in, unsigned int chunkSize) {
        unsigned int start = in.GetPosition();
        unsigned int version = in.ReadULong();
        Dir[0] = in.ReadFloat(); Dir[1] = in.ReadFloat(); Dir[2] = in.ReadFloat();
        if (version >= 2) { for (int i = 0; i < 12; ++i) M[i] = in.ReadFloat(); }
        else { for (int i = 0; i < 12; ++i) M[i] = 0.0f; M[0] = M[4] = M[8] = 1.0f; }
        if (version >= 3) UseLocalOrigin = in.ReadBool(); else UseLocalOrigin = false;
        baseSize = chunkSize - (in.GetPosition() - start);
    }
};
int main() {
    Writer w; w.U32(1); w.F32(1.0f); w.F32(2.0f); w.F32(3.0f);
    Stream s(w.buf, w.n); Chunk c; c.UseLocalOrigin = true; c.ReadFromFile(s, 100);
    bool ok = c.Dir[2] == 3.0f && c.M[0] == 1.0f && c.M[4] == 1.0f && c.M[8] == 1.0f && c.M[1] == 0.0f && c.M[9] == 0.0f
        && !c.UseLocalOrigin && c.baseSize == 84;
    Writer v; v.U32(3); v.F32(0); v.F32(0); v.F32(-1.0f);
    for (int i = 0; i < 12; ++i) v.F32((float)(i + 1));
    v.U8(1);
    Stream t(v.buf, v.n); Chunk d; d.ReadFromFile(t, 80);
    ok = ok && d.Dir[2] == -1.0f && d.M[11] == 12.0f && d.UseLocalOrigin && d.baseSize == 80 - 65;
    if (!ok) { std::printf("00aa9e60_TEST FAIL\n"); return 1; }
    std::printf("00aa9e60_TEST PASS\n"); return 0;
}
