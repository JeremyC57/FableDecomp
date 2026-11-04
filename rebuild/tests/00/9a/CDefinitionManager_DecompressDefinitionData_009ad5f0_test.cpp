// Behaviour test for CDefinitionManager::DecompressDefinitionData (0x009AD5F0): a model of the row -- the upper bound of the
// def index picks the block (the pair before it), the block is "inflated" into the static cache only when the cache holds a
// different block or owner, and the stream gets [table[i], table[i+1]) or [table[i], cache size) for the block's last def.
#include <stdio.h>
#include <string.h>
struct Pair { unsigned long first, second; };
struct Cache { const void* Owner; unsigned long BlockOffset, Size; unsigned char Data[64]; };
static Cache g_cache;
static int g_inflates;
struct Stream { const unsigned char* p; unsigned long n; };
struct Manager {
    const unsigned char* Compressed; Pair Offsets[3]; int Count;
    const Pair* UpperBound(unsigned long key) const
    {
        int i = 0;
        while (i < Count && !(key < Offsets[i].first)) ++i;
        return &Offsets[i];
    }
    void Decompress(Stream& stream, unsigned long defIndex) const
    {
        const Pair* next = UpperBound(defIndex);
        unsigned long blockFirst = next[-1].first, blockOffset = next[-1].second;
        unsigned long compressedSize = next->second - blockOffset, count = next->first - blockFirst;
        if (g_cache.Owner != this || g_cache.BlockOffset != blockOffset) {
            ++g_inflates;                                   // "inflate": copy the block
            memcpy(g_cache.Data, Compressed + blockOffset, compressedSize);
            g_cache.Size = compressedSize;
            g_cache.Owner = this;
            g_cache.BlockOffset = blockOffset;
        }
        const unsigned short* table = (const unsigned short*)g_cache.Data;
        unsigned long local = defIndex - blockFirst;
        unsigned long start = table[local];
        unsigned long end = (local + 1 < count) ? table[local + 1] : g_cache.Size;
        stream.p = g_cache.Data + start; stream.n = end - start;
    }
};
int main()
{
    // block 0: defs 0..1, table {4, 6}, data "ab" "cd"; block 1 (offset 8): def 2, table {2}, data "xyz"
    static unsigned char raw[16];
    unsigned short t0[2] = { 4, 6 }; memcpy(raw, t0, 4); memcpy(raw + 4, "abcd", 4);
    unsigned short t1 = 2; memcpy(raw + 8, &t1, 2); memcpy(raw + 10, "xyz", 3);
    Manager m = { raw, { { 0, 0 }, { 2, 8 }, { 3, 13 } }, 3 };
    Stream s;
    m.Decompress(s, 0); int ok = s.n == 2 && s.p[0] == 'a' && g_inflates == 1;
    m.Decompress(s, 1); ok = ok && s.n == 2 && s.p[0] == 'c' && g_inflates == 1;   // last def of block 0: up to the cache size
    m.Decompress(s, 2); ok = ok && s.n == 3 && s.p[0] == 'x' && g_inflates == 2;
    printf(ok ? "009ad5f0_TEST PASS\n" : "009ad5f0_TEST FAIL\n");
    return ok ? 0 : 1;
}