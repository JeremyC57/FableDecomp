// Behaviour test for 0x00A79930 CNavSwitchableLeafNode::CNavSwitchableLeafNode(generator ctor): leaf base fields set,
// blocked, no region, every unique id added as closed and the id map sorted ascending.
#include <cstdio>

struct C2DVector { float X, Y; };
struct SUniqueIDOpen { unsigned long long first; bool second; };
struct CNode {
    C2DVector Position; unsigned char QuadTreeLevel, NavLayer; bool LeafNode, Blocked; void* PRegion; unsigned char PreferabilityCost;
    SUniqueIDOpen Ids[16]; int Count; bool Dirty;
    CNode(const C2DVector& p, unsigned char level, unsigned char layer, unsigned char pref, const unsigned long long* ids, int n)
        : Position(p), QuadTreeLevel(level), NavLayer(layer), LeafNode(true), Blocked(false), PRegion(0), PreferabilityCost(pref), Count(0), Dirty(false)
    {
        Blocked = true;
        for (int i = 0; i < n; ++i) { Ids[Count].first = ids[i]; Ids[Count].second = false; ++Count; Dirty = true; }
        PRegion = 0;
        if (Dirty) {
            for (int a = 1; a < Count; ++a) for (int b = a; b > 0 && Ids[b].first < Ids[b - 1].first; --b) { SUniqueIDOpen t = Ids[b]; Ids[b] = Ids[b - 1]; Ids[b - 1] = t; }
            Dirty = false;
        }
    }
};
int main()
{
    unsigned long long ids[3] = { 0x500000001ULL, 7, 0x300000000ULL };
    C2DVector p = { 2, 3 };
    CNode n(p, 2, 1, 0x40, ids, 3);
    if (!n.LeafNode || !n.Blocked || n.PRegion || n.PreferabilityCost != 0x40 || n.QuadTreeLevel != 2 || n.Count != 3 || n.Dirty) { std::printf("00a79930_TEST FAIL\n"); return 1; }
    if (n.Ids[0].first != 7 || n.Ids[1].first != 0x300000000ULL || n.Ids[2].first != 0x500000001ULL || n.Ids[0].second || n.Ids[2].second) { std::printf("00a79930_TEST FAIL\n"); return 1; }
    std::printf("00a79930_TEST PASS\n");
    return 0;
}