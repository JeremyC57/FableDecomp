// Behaviour test for 0x006BC350 CRegion::GetMiniMapRegionExitTextOffset(const CCharString&, C2DVector&) const.
#include <cstdio>
#include <cstring>

struct CCharString { const char* text; };
struct C2DVector { float X, Y; };

struct Node { const char* key; C2DVector value; };

struct ExitMap {                                        // stand-in: flat array, header = not found
    Node* _M_header;
    Node* nodes;
    int   n;
    Node* _M_find(const CCharString& k) const
    {
        for (int i = 0; i < n; ++i) if (std::strcmp(nodes[i].key, k.text) == 0) return &nodes[i];
        return _M_header;
    }
};

struct CRegion {
    ExitMap MiniMapRegionExitTextOffsets;
    bool GetMiniMapRegionExitTextOffset(const CCharString& exitName, C2DVector& offset) const
    {
        Node* it = MiniMapRegionExitTextOffsets._M_find(exitName);
        if (it != MiniMapRegionExitTextOffsets._M_header) {
            offset = it->value;
            return true;
        }
        return false;
    }
};

int main()
{
    Node header = { "", { 0, 0 } };
    Node exits[2] = { { "EXIT_NORTH", { 1.5f, -2.0f } }, { "EXIT_SOUTH", { 3.0f, 4.25f } } };
    CRegion r; r.MiniMapRegionExitTextOffsets._M_header = &header; r.MiniMapRegionExitTextOffsets.nodes = exits; r.MiniMapRegionExitTextOffsets.n = 2;
    C2DVector v = { 9.0f, 9.0f };
    CCharString south = { "EXIT_SOUTH" }, west = { "EXIT_WEST" };
    int ok = r.GetMiniMapRegionExitTextOffset(south, v) && v.X == 3.0f && v.Y == 4.25f;
    if (r.GetMiniMapRegionExitTextOffset(west, v) || v.X != 3.0f || v.Y != 4.25f) ok = 0;   // not found: untouched
    if (!ok) { std::printf("006bc350_TEST FAIL\n"); return 1; }
    std::printf("006bc350_TEST PASS\n");
    return 0;
}