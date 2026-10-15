#include <cstdio>
struct Node {
    long regionId; long type; int madeCalls;
    const Node* nb[4]; signed char count;
    long GetRegionID() const { return regionId; }
    long GetNodeType() const { return type; }
    bool MakeRegion(const void*, void*) { ++madeCalls; return false; }
    bool AddNavigableNodeNeighbouringSwitchableNodeToRegion(const void* region, void* nodes)
    {
        if (GetRegionID() == 0) {
            for (const Node** it = nb; it != nb + count; ++it) {
                const Node* n = *it;
                if (n->GetNodeType() == 3 && n->GetRegionID() != 0) { MakeRegion(region, nodes); return true; }
            }
        }
        return false;
    }
};
int main()
{
    Node a = { 0, 1, 0, {0}, 0 }, s1 = { 0, 3, 0, {0}, 0 }, s2 = { 5, 3, 0, {0}, 0 }, n1 = { 7, 1, 0, {0}, 0 };
    a.nb[0] = &n1; a.nb[1] = &s1; a.count = 2;
    bool fail = false;
    if (a.AddNavigableNodeNeighbouringSwitchableNodeToRegion(0, 0) || a.madeCalls != 0) fail = true;
    a.nb[2] = &s2; a.count = 3;
    if (!a.AddNavigableNodeNeighbouringSwitchableNodeToRegion(0, 0) || a.madeCalls != 1) fail = true;
    a.regionId = 2;
    if (a.AddNavigableNodeNeighbouringSwitchableNodeToRegion(0, 0) || a.madeCalls != 1) fail = true;
    if (fail) { std::printf("00a76490_TEST FAIL\n"); return 1; }
    std::printf("00a76490_TEST PASS\n");
    return 0;
}