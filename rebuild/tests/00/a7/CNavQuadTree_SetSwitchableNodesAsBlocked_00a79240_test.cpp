// Behaviour test for 0x00A79240 CNavQuadTree::SetSwitchableNodesAsBlocked: every switchable leaf registered under the
// switch's 64-bit unique ID (the [LowerBound, UpperBound) range of the sorted id -> node map) receives (id, blocked);
// nodes under other ids are untouched.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

struct Node {
    u64 LastID;
    int Calls;
    bool Blocked;
    void SetAsBlocked(u64 id, bool blocked) { LastID = id; Blocked = blocked; ++Calls; }
};
struct Entry { u64 first; Node* second; };

struct Map {
    Entry* First;
    Entry* Last;
    Entry* LowerBound(const u64& key)
    {
        Entry* first = First;
        int count = (int)(Last - First);
        while (count > 0) {
            int half = count / 2;
            Entry* middle = first + half;
            if (middle->first < key) { first = middle + 1; count -= half + 1; } else count = half;
        }
        return first;
    }
    Entry* UpperBound(const u64& key)
    {
        Entry* first = First;
        int count = (int)(Last - First);
        while (count > 0) {
            int half = count / 2;
            Entry* middle = first + half;
            if (!(key < middle->first)) { first = middle + 1; count -= half + 1; } else count = half;
        }
        return first;
    }
};

struct Tree {
    Map SwitchableNodes;
    void SetSwitchableNodesAsBlocked(u64 uniqueID, bool blocked)
    {
        Entry* last = SwitchableNodes.UpperBound(uniqueID);
        for (Entry* it = SwitchableNodes.LowerBound(uniqueID); it != last; ++it)
            it->second->SetAsBlocked(uniqueID, blocked);
    }
};

int main()
{
    static Node nodes[5];
    for (unsigned int i = 0; i < 5; ++i) { nodes[i].LastID = 0; nodes[i].Calls = 0; nodes[i].Blocked = false; }
    u64 big = ((u64)7 << 16 << 16) | 3u;            // high word matters
    Entry entries[5] = { { 3, &nodes[0] }, { big, &nodes[1] }, { big, &nodes[2] }, { big, &nodes[3] }, { big + 1, &nodes[4] } };
    Tree tree; tree.SwitchableNodes.First = entries; tree.SwitchableNodes.Last = entries + 5;
    int ok = 1;
    tree.SetSwitchableNodesAsBlocked(big, true);
    if (nodes[0].Calls != 0 || nodes[4].Calls != 0) ok = 0;
    for (unsigned int i = 1; i <= 3; ++i)
        if (nodes[i].Calls != 1 || !nodes[i].Blocked || nodes[i].LastID != big) ok = 0;
    tree.SetSwitchableNodesAsBlocked(big, false);
    for (unsigned int i = 1; i <= 3; ++i)
        if (nodes[i].Calls != 2 || nodes[i].Blocked) ok = 0;
    tree.SetSwitchableNodesAsBlocked(5, true);                          // absent id: nothing called
    tree.SetSwitchableNodesAsBlocked(3, true);
    if (nodes[0].Calls != 1 || !nodes[0].Blocked || nodes[1].Calls != 2) ok = 0;
    std::printf(ok ? "00a79240_TEST PASS\n" : "00a79240_TEST FAIL\n");
    return ok ? 0 : 1;
}