// Behaviour test for CTCWound::AddWound (0x00757B30): a random wound for the location comes from the creature def's
// morph table (index starts at -1); none means nothing changes; otherwise the change frame is stamped, an identical
// wound already present just has its duration reset to 0, else a new wound (duration 0, decal IDs 0, the morph index,
// not dead, no update) goes on the front and the list is trimmed from the back to 15 entries.
#include <cstdio>
#include <cstring>
#include <list>

struct CEntry { long location, a[8]; float strength; };
struct CWound {
    CEntry desc; unsigned long duration; long ids[4]; long morphIndex; bool dead, hadUpdate;
    CWound(const CEntry& d, long m) : desc(d), duration(0), morphIndex(m), dead(false), hadUpdate(false)
    { ids[0] = ids[1] = ids[2] = ids[3] = 0; }
};
static long g_frame = 0;
static const CEntry* g_pick = 0;
static long g_pickIndex = 0;
static long g_seenIndexIn = 0;

struct CTCWound {
    unsigned long lastFrameChanged;
    std::list<CWound> wounds;
    void AddWound(long)
    {
        long morphIndex = -1;
        g_seenIndexIn = morphIndex;
        const CEntry* entry = g_pick;
        if (entry) morphIndex = g_pickIndex;
        if (entry != 0) {
            lastFrameChanged = g_frame;
            for (std::list<CWound>::iterator it = wounds.begin(); it != wounds.end(); ++it)
                if (std::memcmp(entry, &it->desc, sizeof(CEntry)) == 0) { it->duration = 0; return; }
            wounds.push_front(CWound(*entry, morphIndex));
            while (wounds.size() > 15) wounds.pop_back();
        }
    }
};

int main()
{
    int ok = 1;
    CTCWound w; w.lastFrameChanged = 7;
    g_frame = 50;
    w.AddWound(1);
    ok &= w.wounds.empty() && w.lastFrameChanged == 7 && g_seenIndexIn == -1;
    CEntry e[20];
    std::memset(e, 0, sizeof(e));
    for (int i = 0; i < 20; ++i) e[i].a[0] = i;
    g_pick = &e[0]; g_pickIndex = 3;
    w.AddWound(1);
    ok &= w.wounds.size() == 1 && w.lastFrameChanged == 50 && w.wounds.front().morphIndex == 3 && w.wounds.front().duration == 0;
    w.wounds.front().duration = 99;
    g_frame = 60;
    w.AddWound(1);                                           // same wound: duration reset, nothing added
    ok &= w.wounds.size() == 1 && w.wounds.front().duration == 0 && w.lastFrameChanged == 60;
    for (int i = 1; i < 20; ++i) { g_pick = &e[i]; w.AddWound(1); }
    ok &= w.wounds.size() == 15 && w.wounds.front().desc.a[0] == 19 && w.wounds.back().desc.a[0] == 5;
    std::printf(ok ? "00757b30_TEST PASS\n" : "00757b30_TEST FAIL\n");
    return ok ? 0 : 1;
}
