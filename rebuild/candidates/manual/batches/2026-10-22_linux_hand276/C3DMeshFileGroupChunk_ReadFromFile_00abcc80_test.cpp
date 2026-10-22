#include <cstdio>
#include <cstring>
#include <vector>
// Behaviour copy of C3DMeshFileGroupChunk::ReadFromFile (0x00ABCC80): clear, then merge repeated vertex weights.
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
struct Blend { int VertexIndex; float Weight; };
struct Chunk {
    int BoneIndex; std::vector<Blend> VertexList;
    void ReadFromFile(Stream& in) {
        BoneIndex = (int)in.ReadULong();
        VertexList.clear();
        unsigned int count = in.ReadULong();
        for (unsigned int i = 0; i < count; ++i) {
            Blend b; b.VertexIndex = in.ReadUShort(); b.Weight = in.ReadFloat();
            if (!VertexList.empty() && VertexList.back().VertexIndex == b.VertexIndex) VertexList.back().Weight += b.Weight;
            else VertexList.push_back(b);
        }
    }
};
int main() {
    Writer w; w.U32(5); w.U32(4);
    w.U16(1); w.F32(0.25f); w.U16(1); w.F32(0.5f); w.U16(2); w.F32(1.0f); w.U16(1); w.F32(0.125f);
    Stream s(w.buf, w.n); Chunk c; Blend old = { 9, 9.0f }; c.VertexList.push_back(old); c.ReadFromFile(s);
    bool ok = c.BoneIndex == 5 && c.VertexList.size() == 3 && c.VertexList[0].VertexIndex == 1 && c.VertexList[0].Weight == 0.75f
        && c.VertexList[1].VertexIndex == 2 && c.VertexList[2].VertexIndex == 1 && c.VertexList[2].Weight == 0.125f;
    if (!ok) { std::printf("00abcc80_TEST FAIL\n"); return 1; }
    std::printf("00abcc80_TEST PASS\n"); return 0;
}
