// Behaviour test for CNavSwitchableLeafNode::CNavSwitchableLeafNode 0x00A79A30 (NF68: restated after the port onto
// engine/CNavSwitchableLeafNode.h): a copy of the constructor's logic over std containers with neutral stand-in names. The base part is
// loaded first, the node is blocked, the id count and then each 64-bit id are read from the file (flags 0), every id is inserted as
// closed (false) and marks the map dirty, the region is cleared, and the map is sorted once only when something was inserted.
#include <cstdio>
#include <vector>
#include <utility>
#include <algorithm>

static int fails = 0, g_reads = 0, g_sorts = 0; static long g_flags = -1; static unsigned long g_count = 0;
static unsigned __int64 g_ids[4] = { 9, 3, 7, 1 };
#define CHECK(c) do { if (!(c)) { ++fails; std::printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct File { void Read(void* data, long size, long flags) {
    g_flags = flags; if (g_reads == 0) { CHECK(size == 4); *(unsigned long*)data = g_count; } else { CHECK(size == 8); *(unsigned __int64*)data = g_ids[(g_reads - 1) & 3]; } ++g_reads; } };
struct IdMap {
    std::vector<std::pair<unsigned __int64, bool> > Entries; bool Dirty;
    IdMap() : Dirty(false) { }
    void Insert(const unsigned __int64& key, const bool& value) { Entries.push_back(std::make_pair(key, value)); Dirty = true; }
    void Update() { if (Dirty) { std::sort(Entries.begin(), Entries.end()); ++g_sorts; Dirty = false; } }
};
struct Node {
    bool Blocked; void* PRegion; IdMap UniqueIDsOpen;
    Node(File& file) : Blocked(false), PRegion((void*)1) {
        Blocked = true;
        unsigned long count; file.Read(&count, 4, 0);
        for (unsigned long i = 0; i < count; ++i) { unsigned __int64 id; file.Read(&id, 8, 0); UniqueIDsOpen.Insert(id, false); }
        PRegion = 0;
        UniqueIDsOpen.Update();
    }
};
int main()
{
    File file;
    g_count = 4;
    Node n(file);
    CHECK(n.Blocked && n.PRegion == 0 && g_reads == 5 && g_flags == 0 && g_sorts == 1 && !n.UniqueIDsOpen.Dirty);
    CHECK(n.UniqueIDsOpen.Entries.size() == 4 && n.UniqueIDsOpen.Entries[0].first == 1 && n.UniqueIDsOpen.Entries[3].first == 9);
    CHECK(!n.UniqueIDsOpen.Entries[0].second && !n.UniqueIDsOpen.Entries[3].second);
    g_count = 0; g_reads = 0;
    Node m(file);
    CHECK(m.UniqueIDsOpen.Entries.empty() && g_sorts == 1 && g_reads == 1);
    if (fails == 0) std::printf("00a79a30_TEST PASS\n"); else std::printf("00a79a30_TEST FAIL\n");
    return fails;
}
