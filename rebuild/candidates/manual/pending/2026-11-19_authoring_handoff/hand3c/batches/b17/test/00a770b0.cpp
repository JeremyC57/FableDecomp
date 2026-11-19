// Behaviour test for CNavQuadTree::GetNodeAt (0x00A770B0), a portable model. No node unless the tree is activated or initialising,
// the layer is in range and the point lies in [MapPos, MapPos + size) on both axes. The cell is floor((pos - MapPos) / 32) on each
// axis; the node is the layer's top-level array at width * y + x with width = floor(MapWidth / 32). The shared blocked node is
// returned with its centre at (x * 32 + MapPos.X + 16, y * 32 + MapPos.Y + 16), level 0 and the layer; any other node answers its
// own leaf lookup.
#include <stdio.h>
#include <math.h>
#include <vector>
struct Node { bool blocked; float cx, cy; int level; unsigned char layer; int leaf_lookups; };
struct Tree {
    bool activated, initialising; float map_x, map_y, map_w, map_h;
    std::vector<std::vector<Node*> > layers; Node* blocked;
    Node* GetNodeAt(float px, float py, long layer)
    {
        if (!activated && !initialising) return 0;
        if ((unsigned long)layer >= layers.size()) return 0;
        if (!(px >= map_x && px < map_x + map_w && py >= map_y && py < map_y + map_h)) return 0;
        unsigned long x = (unsigned long)floor((px - map_x) / 32.0f), y = (unsigned long)floor((py - map_y) / 32.0f);
        unsigned long width = (unsigned long)floor(map_w / 32.0f);
        Node* node = layers[layer][width * y + x];
        if (node == blocked) {
            blocked->cx = x * 32.0f + map_x + 16.0f; blocked->cy = y * 32.0f + map_y + 16.0f;
            blocked->layer = (unsigned char)layer; blocked->level = 0; return blocked;
        }
        ++node->leaf_lookups; return node;
    }
};
int main()
{
    int bad = 0;
    Node blocked = { true, 0, 0, 5, 9, 0 }, open = { false, 0, 0, 0, 0, 0 };
    Tree t; t.activated = true; t.initialising = false; t.map_x = 100.0f; t.map_y = -64.0f; t.map_w = 64.0f; t.map_h = 64.0f; t.blocked = &blocked;
    std::vector<Node*> l0(4, &open); l0[3] = &blocked; t.layers.push_back(l0); t.layers.push_back(l0);
    if (t.GetNodeAt(110.0f, -60.0f, 0) != &open || open.leaf_lookups != 1) ++bad;               // cell (0,0)
    Node* b = t.GetNodeAt(140.0f, -10.0f, 1);                                                    // cell (1,1) = index 3
    if (b != &blocked || blocked.cx != 148.0f || blocked.cy != -16.0f || blocked.level != 0 || blocked.layer != 1) ++bad;
    if (t.GetNodeAt(164.0f, -60.0f, 0) != 0) ++bad;                                              // x == MapPos.X + width: outside
    if (t.GetNodeAt(99.9f, -60.0f, 0) != 0 || t.GetNodeAt(110.0f, -60.0f, 2) != 0 || t.GetNodeAt(110.0f, -60.0f, -1) != 0) ++bad;
    t.activated = false; if (t.GetNodeAt(110.0f, -60.0f, 0) != 0) ++bad;
    t.initialising = true; if (t.GetNodeAt(110.0f, -60.0f, 0) != &open) ++bad;
    printf(bad ? "00a770b0_TEST FAIL\n" : "00a770b0_TEST PASS\n");
    return bad ? 1 : 0;
}
