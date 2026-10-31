// Behaviour test for CParticleProgram::ReadGroups 0x00AE3A60: count, resize, then per group bone / blend count / blend array,
// read through the inline stream Read (fast path from the chunk buffer, overflow path when the chunk runs short).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif

struct Stream {
    unsigned long StreamPos; long StreamSize; unsigned char* CurrentSourcePtr; long SourceChunkBytesRemaining;
    unsigned char* full; int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {   // the slow path: refill from the full image at StreamPos
        ++overflowCalls;
        std::memcpy(buffer, full + StreamPos, size);
        StreamPos += size;
        CurrentSourcePtr = full + StreamPos;
        SourceChunkBytesRemaining = 64;
    }
    static bool IsValidPosition(i64 p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        i64 np = (i64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (size <= SourceChunkBytesRemaining) {
            std::memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    unsigned int ReadULONG() { unsigned int v; Read(&v, 4); return v; }
    int ReadSLONG() { int v; Read(&v, 4); return v; }
};
struct Blend { int Vertex; float Weight; };
struct Group {
    int BoneIndex; Blend* blends; unsigned int n;
    void SetBoneIndex(int b) { BoneIndex = b; }
    void Resize(unsigned int k) { blends = (Blend*)std::realloc(blends, (k ? k : 1) * sizeof(Blend)); n = k; }
    Blend* GetVertexBlends() { return n > 0 ? blends : (Blend*)&blends; }
};
struct Groups {
    Group* g; unsigned int n;
    void resize(unsigned int k) { g = (Group*)std::calloc(k ? k : 1, sizeof(Group)); n = k; for (unsigned int i = 0; i < k; ++i) g[i].BoneIndex = -1; }
};
static void ReadGroups(Stream& in, Groups& groups)
{
    unsigned int count = in.ReadULONG();
    groups.resize(count);
    for (unsigned int i = 0; i < count; ++i) {
        Group& group = groups.g[i];
        group.SetBoneIndex(in.ReadSLONG());
        unsigned int blends = in.ReadULONG();
        group.Resize(blends);
        int bytes = blends * sizeof(Blend);
        Blend* data = group.GetVertexBlends();
        if (bytes > 0) in.Read(data, bytes);
    }
}
int main()
{
    unsigned char img[256]; int w = 0;
    unsigned int words[] = { 3, 7, 2, 0, 0, 0, 0, 9, 0, 5, 1, 0, 0 };
    float f1 = 0.25f, f2 = 0.75f, f3 = 1.0f;
    std::memcpy(words + 4, &f1, 4); std::memcpy(words + 6, &f2, 4);
    words[3] = 11; words[5] = 12;
    // group0: bone 7, 2 blends {11,0.25},{12,0.75}; group1: bone 9, 0 blends; group2: bone 5, 1 blend {0,1.0}
    words[11] = 0; std::memcpy(words + 12, &f3, 4);
    std::memcpy(img, words, sizeof(words)); w = sizeof(words);
    Stream s; s.StreamPos = 0; s.StreamSize = w; s.full = img; s.CurrentSourcePtr = img; s.SourceChunkBytesRemaining = 10; s.overflowCalls = 0;
    Groups gs; gs.g = 0; gs.n = 0;
    ReadGroups(s, gs);
    bool ok = gs.n == 3 && gs.g[0].BoneIndex == 7 && gs.g[0].n == 2 && gs.g[0].blends[0].Vertex == 11 &&
              gs.g[0].blends[1].Vertex == 12 && gs.g[0].blends[1].Weight > 0.74f && gs.g[0].blends[1].Weight < 0.76f &&
              gs.g[1].BoneIndex == 9 && gs.g[1].n == 0 && gs.g[2].BoneIndex == 5 && gs.g[2].n == 1 &&
              gs.g[2].blends[0].Weight > 0.99f && s.StreamPos == (unsigned long)w && s.overflowCalls >= 1;
    if (!ok) { std::printf("00ae3a60_TEST FAIL\n"); return 1; }
    std::printf("00ae3a60_TEST PASS\n");
    return 0;
}