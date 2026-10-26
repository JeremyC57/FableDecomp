// behaviour test for 0x00AE0680 CParticleProgram::WriteGroups: count, then per group bone index, blend count, blend bytes
#include <cstdio>
class C3DVertexBlend2 { public: long Vertex; float Weight; };
class C3DGroup2 { public:
    long BoneIndex;
    C3DVertexBlend2* Start;
    C3DVertexBlend2* Finish;
    long GetBoneIndex() const { return BoneIndex; }
    long GetVertexBlendCount() const { return (long)(Finish - Start); }
    const C3DVertexBlend2* PeekVertexBlends() const { return Start; }
};
struct CGroupArray {
    C3DGroup2* Start;
    C3DGroup2* Finish;
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    const C3DGroup2& operator[](unsigned int i) const { return Start[i]; }
};
class CDataOutputStream { public:
    unsigned long Words[32];
    int NoWords;
    long RawBytes;
    const void* LastRaw;
    void WriteULong(unsigned long v) { Words[NoWords++] = v; }
    void Write(const void* data, long length) { LastRaw = data; RawBytes += length; }
};
void WriteGroups(CDataOutputStream& stream, const CGroupArray& groups)
{
    unsigned long count = groups.size();
    stream.WriteULong(count);
    for (unsigned long i = 0; i < count; ++i) {
        const C3DGroup2& group = groups[i];
        stream.WriteULong(group.GetBoneIndex());
        long blends = group.GetVertexBlendCount();
        stream.WriteULong(blends);
        stream.Write(group.PeekVertexBlends(), blends * (long)sizeof(C3DVertexBlend2));
    }
}
int main()
{
    C3DVertexBlend2 b[3] = { { 1, 0.5f }, { 2, 0.25f }, { 3, 1.0f } };
    C3DGroup2 g[2];
    g[0].BoneIndex = 7; g[0].Start = b; g[0].Finish = b + 2;
    g[1].BoneIndex = 9; g[1].Start = b + 2; g[1].Finish = b + 3;
    CGroupArray arr; arr.Start = g; arr.Finish = g + 2;
    CDataOutputStream s; s.NoWords = 0; s.RawBytes = 0; s.LastRaw = 0;
    WriteGroups(s, arr);
    unsigned long expect[5] = { 2, 7, 2, 9, 1 };
    if (s.NoWords != 5) { std::printf("00ae0680_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (s.Words[i] != expect[i]) { std::printf("00ae0680_TEST FAIL\n"); return 1; }
    if (s.RawBytes != 3 * (long)sizeof(C3DVertexBlend2) || s.LastRaw != b + 2) { std::printf("00ae0680_TEST FAIL\n"); return 1; }
    std::printf("00ae0680_TEST PASS\n");
    return 0;
}