// Behaviour test for the advance-in-cover GetAllThingsInRoughArea (0x009793D0): every mapwho layer and cell of the scaled
// box is walked, each mapwho is visited once per search ID (a thing reachable from two maps is appended once), and only
// things passing cover / not-occupied / not-excluded / in-range are appended, in walk order.
#include <cstdio>
#include <vector>

struct Thing { int id; bool cover, occupied; float x; unsigned int searchID; };
struct Node { Thing* thing; Node* next; };
struct Map { int levels; int width[2]; int shift[2]; std::vector<Node*> cells[2]; };
static unsigned int g_searchID = 0;
struct Filters {
    const Thing* excluded; float range;
    bool operator()(const Thing& t) const
    {
        return t.cover && !t.occupied && &t != excluded && (t.x < range && t.x > -range);
    }
};
struct Box { long TLX, TLY, BRX, BRY; };
static void GetAll(const std::vector<Map*>& maps, const Box& area, const Filters& filter, std::vector<Thing*>& result)
{
    ++g_searchID;
    for (unsigned int m = 0; m < maps.size(); ++m) {
        Map* map = maps[m];
        for (int layer = 0; layer < map->levels; ++layer) {
            long shift = map->shift[layer];
            Box s; s.TLX = area.TLX >> shift; s.TLY = area.TLY >> shift;
            s.BRX = ((area.BRX - 1) >> shift) + 1; s.BRY = ((area.BRY - 1) >> shift) + 1;
            if (s.BRX - s.TLX == 0 || s.BRY - s.TLY == 0) continue;
            for (long y = s.TLY; y < s.BRY; ++y)
                for (long x = s.TLX; x < s.BRX; ++x)
                    for (Node* n = map->cells[layer][y * map->width[layer] + x]; n; n = n->next)
                        if (n->thing->searchID < g_searchID) {
                            n->thing->searchID = g_searchID;
                            Thing* typed = n->thing;
                            if (filter(*typed)) result.push_back(typed);
                        }
        }
    }
}

int main()
{
    Thing t[5] = { {0, true, false, 1.0f, 0}, {1, true, true, 1.0f, 0}, {2, false, false, 1.0f, 0},
                   {3, true, false, 50.0f, 0}, {4, true, false, -2.0f, 0} };
    Node n[7] = { {&t[0], 0}, {&t[1], 0}, {&t[2], 0}, {&t[3], 0}, {&t[4], 0}, {&t[0], 0}, {&t[4], 0} };
    n[0].next = &n[1];
    Map a; a.levels = 2; a.width[0] = 4; a.width[1] = 2; a.shift[0] = 0; a.shift[1] = 1;
    a.cells[0].assign(16, (Node*)0); a.cells[1].assign(4, (Node*)0);
    a.cells[0][1 * 4 + 1] = &n[0]; a.cells[0][1 * 4 + 2] = &n[2]; a.cells[0][3 * 4 + 3] = &n[3]; a.cells[1][0] = &n[4];
    Map b = a; b.cells[0].assign(16, (Node*)0); b.cells[1].assign(4, (Node*)0); b.cells[0][0] = &n[5]; b.cells[1][1] = &n[6];
    std::vector<Map*> maps; maps.push_back(&a); maps.push_back(&b);
    Box area = { 0, 0, 4, 2 };
    Filters f = { &t[3], 10.0f };
    std::vector<Thing*> out;
    GetAll(maps, area, f, out);
    bool ok = out.size() == 2 && out[0] == &t[0] && out[1] == &t[4];
    std::vector<Thing*> again;
    GetAll(maps, area, f, again);                       // a new search ID revisits everything
    ok = ok && again.size() == 2;
    if (!ok) { std::printf("009793d0_TEST FAIL\n"); return 1; }
    std::printf("009793d0_TEST PASS\n");
    return 0;
}