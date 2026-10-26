#include <cstdio>
class CChunkedFileSuperChunk;
class CChunkedFileChunk { public: void* vt; char ChunkType[5]; unsigned char pad[3]; CChunkedFileSuperChunk* Parent; };
struct CCountedChunkPointer { CChunkedFileChunk* Object; void* Info; };
static CCountedChunkPointer* g_erased = 0; static int g_erases = 0;
struct CRetailChunkArray {
    CCountedChunkPointer* Begin; CCountedChunkPointer* End; CCountedChunkPointer* Capacity;
    void Erase(CCountedChunkPointer* where) { g_erased = where; ++g_erases; }
};
struct CChunkedFileSuperChunk_Retail {
    unsigned char _base_0x00[0x10]; CRetailChunkArray SubChunks; CRetailChunkArray RegisteredSubChunkTypes; unsigned long CurrentChunkIndex;
    void RemoveSubChunk(CCountedChunkPointer& chunk)
    {
        CCountedChunkPointer* it = SubChunks.Begin; CCountedChunkPointer* end = SubChunks.End;
        if (it == end) return;
        CChunkedFileChunk* object = chunk.Object;
        do { if (it->Object == object) { it->Object->Parent = 0; SubChunks.Erase(it); return; } ++it; } while (it != end);
    }
};
int main()
{
    CChunkedFileSuperChunk_Retail super; CChunkedFileChunk chunks[3]; CCountedChunkPointer ptrs[3];
    for (int i = 0; i < 3; ++i) { chunks[i].Parent = (CChunkedFileSuperChunk*)&super; ptrs[i].Object = &chunks[i]; ptrs[i].Info = 0; }
    super.SubChunks.Begin = ptrs; super.SubChunks.End = ptrs + 3; super.SubChunks.Capacity = ptrs + 3;
    CCountedChunkPointer victim; victim.Object = &chunks[1]; victim.Info = 0;
    super.RemoveSubChunk(victim);
    if (g_erases != 1 || g_erased != ptrs + 1 || chunks[1].Parent != 0 || chunks[0].Parent == 0 || chunks[2].Parent == 0) { std::printf("00aa8280_TEST FAIL\n"); return 1; }
    CChunkedFileChunk other; other.Parent = (CChunkedFileSuperChunk*)&super; CCountedChunkPointer missing; missing.Object = &other; missing.Info = 0;
    super.RemoveSubChunk(missing);
    if (g_erases != 1 || other.Parent == 0) { std::printf("00aa8280_TEST FAIL\n"); return 1; }
    super.SubChunks.End = ptrs; super.RemoveSubChunk(victim);
    if (g_erases != 1) { std::printf("00aa8280_TEST FAIL\n"); return 1; }
    std::printf("00aa8280_TEST PASS\n"); return 0;
}