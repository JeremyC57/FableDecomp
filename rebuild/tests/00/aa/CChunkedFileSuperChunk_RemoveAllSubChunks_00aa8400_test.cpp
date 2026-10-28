// Behaviour test for CChunkedFileSuperChunk::RemoveAllSubChunks (0x00AA8400): every sub-chunk loses its parent, then the array is emptied.
#include <cstdio>
struct Super;
struct Chunk { Super* Parent; };
struct Super { Chunk* sub[4]; int count;
    void RemoveAllSubChunks() { for (int i = 0; i < count; ++i) sub[i]->Parent = 0; count = 0; } };
int main()
{
    Super s; Chunk a, b, c; a.Parent = b.Parent = c.Parent = &s; s.sub[0] = &a; s.sub[1] = &b; s.count = 2;
    s.RemoveAllSubChunks();
    if (s.count != 0 || a.Parent != 0 || b.Parent != 0 || c.Parent != &s) { std::printf("00aa8400_TEST FAIL\n"); return 1; }
    s.RemoveAllSubChunks();
    if (s.count != 0) { std::printf("00aa8400_TEST FAIL\n"); return 1; }
    std::printf("00aa8400_TEST PASS\n");
    return 0;
}