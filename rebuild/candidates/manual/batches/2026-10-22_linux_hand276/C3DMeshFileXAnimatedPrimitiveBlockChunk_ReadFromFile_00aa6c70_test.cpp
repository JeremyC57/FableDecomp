#include <cstdio>
#include <cstring>
#include <vector>
// Behaviour copy of C3DMeshFileXAnimatedPrimitiveBlockChunk::ReadFromFile (0x00AA6C70).
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
struct BoneWeight { unsigned int BoneIndex; unsigned int Weight; };
struct Chunk {
    std::vector<unsigned int> Groups; unsigned int BonesPerVertex;
    std::vector<unsigned int> VertexList; std::vector<BoneWeight> BoneWeights;
    void ReadFromFile(Stream& in) {
        unsigned int groups = in.ReadULong();
        for (unsigned int i = 0; i < groups; ++i) Groups.push_back(in.ReadULong());
        BonesPerVertex = in.ReadULong();
        unsigned int vertices = in.ReadULong();
        VertexList.resize(vertices);
        if (vertices) in.Read(&VertexList[0], (int)(vertices * 4));
        unsigned int weights = BonesPerVertex * vertices;
        BoneWeights.resize(weights);
        for (unsigned int k = 0; k < weights; ++k) {
            unsigned int bone = in.ReadULong(); unsigned int weight = in.ReadULong();
            BoneWeights[k].BoneIndex = bone; BoneWeights[k].Weight = (unsigned int)(long)(float)weight;
        }
    }
};
int main() {
    Writer w; w.U32(2); w.U32(10); w.U32(11); w.U32(2); w.U32(2); w.U32(100); w.U32(200);
    w.U32(1); w.U32(64); w.U32(2); w.U32(128); w.U32(3); w.U32(255); w.U32(4); w.U32(0);
    Stream s(w.buf, w.n); Chunk c; c.ReadFromFile(s);
    bool ok = c.Groups.size() == 2 && c.Groups[1] == 11 && c.BonesPerVertex == 2 && c.VertexList.size() == 2 && c.VertexList[1] == 200
        && c.BoneWeights.size() == 4 && c.BoneWeights[2].BoneIndex == 3 && c.BoneWeights[2].Weight == 255 && s.StreamPos == (unsigned int)w.n;
    if (!ok) { std::printf("00aa6c70_TEST FAIL\n"); return 1; }
    std::printf("00aa6c70_TEST PASS\n"); return 0;
}
