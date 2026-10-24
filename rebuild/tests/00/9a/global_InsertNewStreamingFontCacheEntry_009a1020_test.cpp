// Behaviour test for 0x009A1020 InsertNewStreamingFontCacheEntry: the oldest entry is recycled -- stamped with the
// frame, made the newest, its index returned -- and the next-oldest entry becomes the chain end.
#include <cstdio>

static long g_frameCounter = 55;

struct StreamingFontCacheEntry {
    unsigned short older;
    unsigned short newer;
    long           lastUseTime;
    unsigned int   glyphIndex : 24;
    unsigned int   loading    : 1;
    unsigned int   cacheIndex;
};
struct StreamingFontCacheBufferData {
    unsigned int numEntries;
    StreamingFontCacheEntry* entries;
    unsigned int newestEntry;
    unsigned int oldestEntry;
};

static void Init(StreamingFontCacheBufferData& d, StreamingFontCacheEntry* e, unsigned int n)
{
    d.numEntries = n; d.entries = e; d.newestEntry = 0; d.oldestEntry = n - 1;
    for (unsigned int i = 0; i < n; ++i) {
        e[i].older = (unsigned short)(i + 1); e[i].newer = (unsigned short)(i == 0 ? n : i - 1);
        e[i].lastUseTime = 0; e[i].glyphIndex = 0; e[i].loading = 0; e[i].cacheIndex = i;
    }
}
static int ChainIs(const StreamingFontCacheBufferData& d, const unsigned int* want, unsigned int n)
{
    unsigned int cur = d.newestEntry, prev = d.numEntries;
    for (unsigned int k = 0; k < n; ++k) {
        if (cur != want[k] || d.entries[cur].newer != prev) return 0;
        prev = cur; cur = d.entries[cur].older;
    }
    return cur == d.numEntries && d.oldestEntry == want[n - 1];
}

static unsigned short InsertNewStreamingFontCacheEntry(StreamingFontCacheBufferData& data)
{
    unsigned short index = (unsigned short)data.oldestEntry;
    StreamingFontCacheEntry& entry = data.entries[index];
    entry.lastUseTime = g_frameCounter;
    data.entries[entry.newer].older = (unsigned short)data.numEntries;
    data.oldestEntry = entry.newer;
    entry.newer = (unsigned short)data.numEntries;
    entry.older = (unsigned short)data.newestEntry;
    data.entries[entry.older].newer = index;
    data.newestEntry = index;
    return index;
}

int main()
{
    StreamingFontCacheEntry e[4];
    StreamingFontCacheBufferData d;
    Init(d, e, 4);
    int bad = 0;
    if (InsertNewStreamingFontCacheEntry(d) != 3) bad = 1;
    { unsigned int w[4] = { 3, 0, 1, 2 }; if (!ChainIs(d, w, 4) || e[3].lastUseTime != 55) bad = 1; }
    if (InsertNewStreamingFontCacheEntry(d) != 2) bad = 1;
    { unsigned int w[4] = { 2, 3, 0, 1 }; if (!ChainIs(d, w, 4)) bad = 1; }
    if (InsertNewStreamingFontCacheEntry(d) != 1) bad = 1;
    if (InsertNewStreamingFontCacheEntry(d) != 0) bad = 1;
    if (InsertNewStreamingFontCacheEntry(d) != 3) bad = 1;   // wrapped round: the first recycled is oldest again
    { unsigned int w[4] = { 3, 0, 1, 2 }; if (!ChainIs(d, w, 4)) bad = 1; }
    if (bad) { std::printf("009a1020_TEST FAIL\n"); return 1; }
    std::printf("009a1020_TEST PASS\n");
    return 0;
}