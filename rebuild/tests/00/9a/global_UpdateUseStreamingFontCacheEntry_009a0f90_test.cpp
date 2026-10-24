// Behaviour test for 0x009A0F90 UpdateUseStreamingFontCacheEntry: a used entry is stamped with the frame and moved
// to the newest end of the LRU chain (from the middle, from the oldest end, and a no-op when already stamped).
#include <cstdio>

static long g_frameCounter = 100;

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

static void UpdateUseStreamingFontCacheEntry(StreamingFontCacheBufferData& data, unsigned short index)
{
    StreamingFontCacheEntry& entry = data.entries[index];
    long frame = g_frameCounter;
    if (entry.lastUseTime != frame) {
        entry.lastUseTime = frame;
        if (entry.newer < data.numEntries)
            data.entries[entry.newer].older = entry.older;
        else
            data.newestEntry = entry.older;
        if (entry.older < data.numEntries)
            data.entries[entry.older].newer = entry.newer;
        else
            data.oldestEntry = entry.newer;
        entry.newer = (unsigned short)data.numEntries;
        entry.older = (unsigned short)data.newestEntry;
        data.entries[entry.older].newer = index;
        data.newestEntry = index;
    }
}

int main()
{
    StreamingFontCacheEntry e[4];
    StreamingFontCacheBufferData d;
    Init(d, e, 4);
    int bad = 0;
    UpdateUseStreamingFontCacheEntry(d, 2);                  // middle -> newest
    { unsigned int w[4] = { 2, 0, 1, 3 }; if (!ChainIs(d, w, 4) || e[2].lastUseTime != 100) bad = 1; }
    UpdateUseStreamingFontCacheEntry(d, 3);                  // oldest -> newest
    { unsigned int w[4] = { 3, 2, 0, 1 }; if (!ChainIs(d, w, 4) || e[3].lastUseTime != 100) bad = 1; }
    UpdateUseStreamingFontCacheEntry(d, 1);                  // oldest again
    { unsigned int w[4] = { 1, 3, 2, 0 }; if (!ChainIs(d, w, 4)) bad = 1; }
    UpdateUseStreamingFontCacheEntry(d, 2);                  // already stamped this frame: unchanged
    { unsigned int w[4] = { 1, 3, 2, 0 }; if (!ChainIs(d, w, 4)) bad = 1; }
    g_frameCounter = 101;
    UpdateUseStreamingFontCacheEntry(d, 2);
    { unsigned int w[4] = { 2, 1, 3, 0 }; if (!ChainIs(d, w, 4) || e[2].lastUseTime != 101) bad = 1; }
    if (bad) { std::printf("009a0f90_TEST FAIL\n"); return 1; }
    std::printf("009a0f90_TEST PASS\n");
    return 0;
}