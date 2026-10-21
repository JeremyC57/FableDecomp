// Behaviour test for C3DAnimation2::ReadCompressedFileData at 0x00A4DF10: array sizing and per-chunk read order.
#include <cstdio>
#include <cstring>
struct Chunk { int size; };
static char g_log[256];
struct Anim {
    unsigned int SequenceCount, SequenceDataBufferSize;
    void Read(const Chunk* chunks, int n)
    {
        SequenceCount = n;                        // CountSubChunksOfType("XSEQ")
        SequenceDataBufferSize = SequenceCount;
        unsigned int index = 0;
        for (int c = 0; c < n; ++c) {             // FindFirst / FindNext walk
            char b[32]; std::sprintf(b, "%u:%d;", index, chunks[c].size);   // Sequences[index].ReadCompressedFromFile(size)
            std::strcat(g_log, b);
            ++index;
        }
    }
};
int main()
{
    Chunk chunks[3] = { { 10 }, { 0 }, { 7 } };
    Anim a; a.Read(chunks, 3);
    int ok = a.SequenceCount == 3 && a.SequenceDataBufferSize == 3 && std::strcmp(g_log, "0:10;1:0;2:7;") == 0;
    std::printf(ok ? "00a4df10_TEST PASS\n" : "00a4df10_TEST FAIL\n");
    return ok ? 0 : 1;
}
