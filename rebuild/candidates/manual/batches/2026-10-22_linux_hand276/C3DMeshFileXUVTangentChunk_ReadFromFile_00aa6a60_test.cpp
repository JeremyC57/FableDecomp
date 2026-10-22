#include <cstdio>
#include <cstring>
#include <vector>
// Behaviour copy of C3DMeshFileXUVTangentChunk::ReadFromFile (0x00AA6A60): version, count, resize both arrays, bulk reads.
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
struct Vec { float X, Y, Z; };
struct Chunk {
    std::vector<Vec> U, V;
    void ReadFromFile(Stream& in) {
        in.ReadULong();
        unsigned int count = in.ReadULong();
        U.resize(count); V.resize(count);
        if (!U.empty()) in.Read(&U[0], (int)(U.size() * sizeof(Vec)));
        if (!V.empty()) in.Read(&V[0], (int)(V.size() * sizeof(Vec)));
    }
};
int main() {
    Writer w; w.U32(1); w.U32(2);
    for (int i = 0; i < 12; ++i) w.F32((float)i);
    Stream s(w.buf, w.n); Chunk c; c.ReadFromFile(s);
    bool ok = c.U.size() == 2 && c.V.size() == 2 && c.U[1].Z == 5.0f && c.V[0].X == 6.0f && c.V[1].Z == 11.0f
        && s.StreamPos == (unsigned int)w.n && s.overflowCalls == 0;
    Writer z; z.U32(1); z.U32(0);
    Stream t(z.buf, z.n); Chunk e; e.ReadFromFile(t);
    ok = ok && e.U.empty() && e.V.empty() && t.StreamPos == 8;
    if (!ok) { std::printf("00aa6a60_TEST FAIL\n"); return 1; }
    std::printf("00aa6a60_TEST PASS\n"); return 0;
}
