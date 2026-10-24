// Behaviour test for 0x009A0F00 InitializeStreamingFontCacheEntries: an empty cache becomes one LRU chain
// (entry i: older i+1, newer i-1; entry 0's newer and the last entry's older are numEntries = "none"),
// every entry aged to FrameCounter - 0x40000000 with glyphIndex 0 and `loading` kept; a non-empty cache is untouched.
#include <cstdio>

static long g_frameCounter = 0x50000010;

struct StreamingFontCacheEntry {
    unsigned short older;
    unsigned short newer;
    long           lastUseTime;
    unsigned int   glyphIndex : 24;
    unsigned int   loading    : 1;
    unsigned int   cacheIndex;
};
struct StreamingFontCacheBufferData {
    unsigned int width, height, numEntries, entryWidth, entryHeight, entrySize;
    StreamingFontCacheEntry* entries;
    unsigned int newestEntry;
    unsigned int oldestEntry;
};

static void InitializeStreamingFontCacheEntries(StreamingFontCacheBufferData& data)
{
    if (data.newestEntry == data.oldestEntry) {
        long frame = g_frameCounter;
        StreamingFontCacheEntry* first = data.entries;
        first->older = 1;
        first->newer = (unsigned short)data.numEntries;
        first->lastUseTime = frame - 0x40000000;
        first->glyphIndex = 0;
        data.newestEntry = 0;
        for (unsigned short i = 1; i < data.numEntries; ++i) {
            StreamingFontCacheEntry& entry = data.entries[i];
            entry.older = i + 1;
            entry.newer = i - 1;
            entry.lastUseTime = frame - 0x40000000;
            entry.glyphIndex = 0;
        }
        data.oldestEntry = data.numEntries - 1;
    }
}

int main()
{
    StreamingFontCacheEntry e[5];
    for (int i = 0; i < 5; ++i) { e[i].older = 99; e[i].newer = 99; e[i].lastUseTime = 7; e[i].glyphIndex = 0x123456; e[i].loading = (i & 1); e[i].cacheIndex = 40 + i; }
    StreamingFontCacheBufferData d;
    d.width = d.height = d.entryWidth = d.entryHeight = d.entrySize = 0;
    d.numEntries = 5; d.entries = e; d.newestEntry = 0; d.oldestEntry = 0;
    InitializeStreamingFontCacheEntries(d);
    int bad = d.newestEntry != 0 || d.oldestEntry != 4 || e[0].older != 1 || e[0].newer != 5;
    for (int i = 0; i < 5; ++i) {
        if (e[i].lastUseTime != 0x10000010 || e[i].glyphIndex != 0 || e[i].loading != (unsigned)(i & 1) || e[i].cacheIndex != (unsigned)(40 + i)) bad = 1;
        if (i > 0 && (e[i].older != i + 1 || e[i].newer != i - 1)) bad = 1;
    }
    // a non-empty cache (newest != oldest) is left alone
    e[2].older = 77; d.newestEntry = 3; d.oldestEntry = 1;
    InitializeStreamingFontCacheEntries(d);
    if (e[2].older != 77 || d.newestEntry != 3) bad = 1;
    if (bad) { std::printf("009a0f00_TEST FAIL\n"); return 1; }
    std::printf("009a0f00_TEST PASS\n");
    return 0;
}