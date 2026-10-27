// Behaviour test for 0x00A96270 GFExplicitNew<N3DPrimitive::CAnimatedBlock> -- placement-construct count blocks in raw storage
#include <cstdio>
#include <new>
template <class T> class CIVCountedPointer { public: T* Object; CIVCountedPointer() : Object(0) { } };
class CBaseBlock {
public:
    unsigned long PrimitiveCount; unsigned long StartIndex; bool IsStrip; bool DegenerateTriangles; unsigned char ChangeFlags;
    CBaseBlock() : PrimitiveCount(0), StartIndex(0), IsStrip(false), DegenerateTriangles(false), ChangeFlags(0) { }
};
class CAnimatedBlock : public CBaseBlock {
public:
    unsigned long BoneConstantsFixupOffset; unsigned long VertexCount; unsigned short BonesPerVertex;
    bool PalettedFlag; unsigned char GroupCount; unsigned char Groups[18];
    CIVCountedPointer<int> Data;
    CAnimatedBlock() { }
};
template <class T> void GFExplicitNew(T* blocks, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i)
        new (&blocks[i]) T;
}
int main()
{
    static unsigned char raw[4 * sizeof(CAnimatedBlock)];
    for (unsigned int i = 0; i < sizeof(raw); ++i) raw[i] = 0xAB;
    CAnimatedBlock* b = (CAnimatedBlock*)raw;
    GFExplicitNew(b, 3);
    for (int i = 0; i < 3; ++i)
        if (b[i].PrimitiveCount != 0 || b[i].StartIndex != 0 || b[i].IsStrip || b[i].DegenerateTriangles || b[i].ChangeFlags != 0 || b[i].Data.Object != 0)
            { std::printf("00a96270_TEST FAIL\n"); return 1; }
    if (b[3].ChangeFlags != 0xAB || b[3].GroupCount != 0xAB) { std::printf("00a96270_TEST FAIL\n"); return 1; }
    std::printf("00a96270_TEST PASS\n");
    return 0;
}