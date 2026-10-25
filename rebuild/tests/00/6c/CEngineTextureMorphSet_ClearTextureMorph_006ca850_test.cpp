// Behaviour test for 0x006CA850 CEngineTextureMorphSet::ClearTextureMorph(long): every morph whose OriginalTextureIndex matches is erased (tail shifted down, the old last destroyed); the others keep their order.
#include <cstdio>
static int g_destroyed = 0;
struct Morph {
    long OriginalTextureIndex; long MorphID;
    ~Morph() { ++g_destroyed; }
};
struct MorphSet {
    Morph* start; Morph* finish;
    Morph* erase(Morph* pos)
    {
        for (Morph* p = pos + 1; p != finish; ++p) { p[-1].OriginalTextureIndex = p->OriginalTextureIndex; p[-1].MorphID = p->MorphID; }
        --finish;
        finish->~Morph();
        return pos;
    }
    void ClearTextureMorph(long index)
    {
        for (Morph* it = start; it != finish; ) {
            if (it->OriginalTextureIndex == index) it = erase(it);
            else ++it;
        }
    }
};
int main()
{
    static unsigned char raw[6 * sizeof(Morph)];
    Morph* m = (Morph*)raw;
    long idx[6] = { 4, 2, 4, 4, 5, 4 };
    for (int i = 0; i < 6; ++i) { m[i].OriginalTextureIndex = idx[i]; m[i].MorphID = 10 + i; }
    MorphSet s; s.start = m; s.finish = m + 6;
    s.ClearTextureMorph(4);
    if (s.finish != m + 2 || g_destroyed != 4 || m[0].OriginalTextureIndex != 2 || m[0].MorphID != 11 ||
        m[1].OriginalTextureIndex != 5 || m[1].MorphID != 14) { std::printf("006ca850_TEST FAIL\n"); return 1; }
    s.ClearTextureMorph(9);                            // no match: untouched
    if (s.finish != m + 2 || g_destroyed != 4) { std::printf("006ca850_TEST FAIL\n"); return 1; }
    std::printf("006ca850_TEST PASS\n");
    return 0;
}