#include <cstdio>
#include <cstring>
#include <vector>
// Behaviour copy of C3DAnimFileSequenceChunk::ReadFromFile (0x00AAF850): matrices, optional bone type, optional base.
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
struct Matrix { float m[12]; };
struct Chunk {
    int BoneIndex, ParentIndex; float SamplesPerSecond; int BoneType; std::vector<Matrix> Matrices; int baseCalls; unsigned int baseSize;
    void ReadFromFile(Stream& in, unsigned int chunkSize) {
        unsigned int end = in.GetPosition() + chunkSize;
        BoneIndex = (int)in.ReadULong(); ParentIndex = (int)in.ReadULong();
        while (true) { unsigned char ch; in.Read(&ch, 1); if (!ch) break; }
        SamplesPerSecond = in.ReadFloat();
        Matrices.clear();
        unsigned int count = in.ReadULong();
        for (unsigned int i = 0; i < count; ++i) { Matrix m; for (int j = 0; j < 12; ++j) m.m[j] = in.ReadFloat(); Matrices.push_back(m); }
        BoneType = 0;
        if (in.GetPosition() < end) { unsigned char b; in.Read(&b, 1); BoneType = b; }
        if (in.GetPosition() < end) { ++baseCalls; baseSize = end - in.GetPosition(); }
    }
};
int main() {
    Writer w; w.U32(3); w.U32(1); w.U8(0); w.F32(30.0f); w.U32(2);
    for (int i = 0; i < 24; ++i) w.F32((float)i);
    int noType = w.n; w.U8(5);
    Stream s(w.buf, w.n); Chunk c; c.baseCalls = 0; c.ReadFromFile(s, (unsigned int)w.n + 8);
    bool ok = c.BoneIndex == 3 && c.Matrices.size() == 2 && c.Matrices[1].m[11] == 23.0f && c.BoneType == 5 && c.baseCalls == 1 && c.baseSize == 8;
    Stream t(w.buf, noType); Chunk d; d.baseCalls = 0; d.BoneType = 9; d.ReadFromFile(t, (unsigned int)noType);
    ok = ok && d.BoneType == 0 && d.baseCalls == 0 && d.SamplesPerSecond == 30.0f;
    if (!ok) { std::printf("00aaf850_TEST FAIL\n"); return 1; }
    std::printf("00aaf850_TEST PASS\n"); return 0;
}