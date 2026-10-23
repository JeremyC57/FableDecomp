// Behaviour test for CTCInventoryClothing::InitialiseMannequin @ 0x005B37F7
#include <cstdio>
#include <set>
#include <vector>

struct Thing { std::set<int> tcs; std::vector<int> copied; std::vector<int> added; int anim; bool edited; };
static Thing g_mannequin;
static Thing* CreateThing(int def) { if (def == 0) return 0; g_mannequin = Thing(); g_mannequin.anim = -1; g_mannequin.edited = false; return &g_mannequin; }

static Thing* InitialiseMannequin(int mannequinDef, const Thing& parent, int defaultAnim)
{
    if (mannequinDef == 0)
        return 0;
    Thing* m = CreateThing(mannequinDef);
    if (m) {
        m->copied.push_back(-1);                 // the owner's +0x64 component
        const int types[3] = { 3, 0x5e, 0x78 };
        for (int i = 0; i < 3; ++i)
            if (parent.tcs.count(types[i]))
                m->copied.push_back(types[i]);
        m->copied.push_back(parent.tcs.count(0x5c) ? 0x5c : 0);
        m->added.push_back(0x5a);
        m->tcs.insert(0x5a);
        m->anim = defaultAnim;
        if (m->tcs.count(0x5e))
            m->edited = true;
    }
    return m;
}

int main()
{
    Thing parent; parent.tcs.insert(3); parent.tcs.insert(0x78);
    if (InitialiseMannequin(0, parent, 4) != 0) { std::printf("005b37f7_TEST FAIL\n"); return 1; }
    Thing* m = InitialiseMannequin(9, parent, 4);
    if (!m || m->copied.size() != 4 || m->copied[1] != 3 || m->copied[2] != 0x78 || m->copied[3] != 0 || m->anim != 4 || m->edited) {
        std::printf("005b37f7_TEST FAIL\n"); return 1;
    }
    std::printf("005b37f7_TEST PASS\n");
    return 0;
}
