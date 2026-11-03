// Behaviour test for CNavQuadTree::SetUpRegions (0x00A7A0A0, parked): a standalone model of the region flood -- region 1 is created
// first; every seed whose node starts a region floods the node list (each listed node may append neighbours) and opens the next
// region; navigable nodes beside switchable ones grow regions until they report no more; region-zero nodes are replaced by the
// shared blocked node. Region ids are (map << 16) | (index & 0xFFFF).
#include <cstdio>
#include <list>
#include <vector>
struct Node {
    int region; bool seedStarts; int borders; bool blocked; Node* next;
    bool MakeRegion(int r, std::list<Node*>& out) { if (region != 0) return false; region = r; if (next) out.push_back(next); return true; }
    bool AddBordering(int r, std::list<Node*>& out) { if (borders <= 0) return false; --borders; region = r; return true; }
    bool RemoveRegionZero() const { return region == 0; }
};
static Node g_blocked = { -1, false, 0, true, 0 };
static long Id(long map, long index) { return (map << 16) | (index & 0xFFFF); }
static void SetUpRegions(long map, std::vector<Node*>& seeds, std::vector<Node*>& nodes, std::vector<long>& regions)
{
    long index = 1;
    regions.push_back(Id(map, index));
    for (size_t s = 0; s < seeds.size(); ++s) {
        Node* n = seeds[s];
        if (!n) continue;
        std::list<Node*> list;
        if (n->MakeRegion(regions.back(), list)) {
            for (std::list<Node*>::iterator it = list.begin(); it != list.end(); ++it) (*it)->MakeRegion(regions.back(), list);
            regions.push_back(Id(map, ++index));
        }
    }
    for (size_t i = 0; i < nodes.size(); ++i) {
        std::list<Node*> list;
        while (nodes[i]->AddBordering(regions.back(), list)) {
            for (std::list<Node*>::iterator it = list.begin(); it != list.end(); ++it) (*it)->MakeRegion(regions.back(), list);
            regions.push_back(Id(map, ++index));
            list.clear();
        }
    }
    for (size_t i = 0; i < nodes.size(); ++i)
        if (nodes[i]->RemoveRegionZero() && nodes[i] != &g_blocked) nodes[i] = &g_blocked;
}
int main()
{
    Node c = { 0, false, 0, false, 0 }, b = { 0, false, 0, false, &c }, a = { 0, true, 0, false, &b }, d = { 0, false, 2, false, 0 }, e = { 0, false, 0, false, 0 };
    std::vector<Node*> seeds; seeds.push_back(&a); seeds.push_back(0);
    std::vector<Node*> nodes; nodes.push_back(&a); nodes.push_back(&b); nodes.push_back(&c); nodes.push_back(&d); nodes.push_back(&e);
    std::vector<long> regions;
    SetUpRegions(3, seeds, nodes, regions);
    if (regions.size() != 4 || regions[0] != 0x30001 || regions[3] != 0x30004) { std::printf("00a7a0a0_TEST FAIL\n"); return 1; }
    if (a.region != 0x30001 || b.region != 0x30001 || c.region != 0x30001 || d.region != 0x30003) { std::printf("00a7a0a0_TEST FAIL\n"); return 1; }
    if (nodes[4] != &g_blocked || nodes[3] != &d) { std::printf("00a7a0a0_TEST FAIL\n"); return 1; }
    std::printf("00a7a0a0_TEST PASS\n");
    return 0;
}
