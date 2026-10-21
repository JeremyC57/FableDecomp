// Behaviour test for NThingMap::CThingMap::Clear (0x0085CEC0): every node of every cell unlinked, InMapwho cleared,
// cell heads zeroed, NoThings reset; the unbounded level (Size -1) is one cell.
#include <cstdio>

struct CTCMapwho { bool InMapwho; };
struct CMapwhoNode { CTCMapwho* PMapwho; CMapwhoNode* PNext; CMapwhoNode* PPrevious; };
struct CThingMapCell { CMapwhoNode* PFirstNode; };
struct CMapwhoLevelInfo { long Size; long ExtentsX; };
static long g_NoMapwhoLevels = 2;
struct CThingMap {
    CMapwhoLevelInfo Info[2];
    CThingMapCell Cells0[4];
    CThingMapCell Cells1[1];
    long X0, Y0, X1, Y1;
    long NoThings;
    CThingMapCell& GetCell(unsigned char level, long x, long y) { return (level ? Cells1 : Cells0)[Info[level].ExtentsX * y + x]; }
    void Clear()
    {
        for (long level = 0; level < g_NoMapwhoLevels; ++level) {
            long width, height;
            long size = Info[level].Size;
            if (size == -1) {
                width = height = 1;
            } else {
                width = (X1 - X0) / size;
                height = (Y1 - Y0) / size;
            }
            for (long y = 0; y < height; ++y)
                for (long x = 0; x < width; ++x) {
                    CThingMapCell& cell = GetCell((unsigned char)level, x, y);
                    CMapwhoNode* node = cell.PFirstNode;
                    while (node) {
                        CMapwhoNode* next = node->PNext;
                        node->PMapwho->InMapwho = false;
                        node->PPrevious = 0;
                        node->PNext = 0;
                        node = next;
                    }
                    cell.PFirstNode = 0;
                }
        }
        NoThings = 0;
    }
};

int main()
{
    CTCMapwho m[3]; for (int i = 0; i < 3; ++i) m[i].InMapwho = true;
    CMapwhoNode n[3];
    n[0].PMapwho = &m[0]; n[0].PNext = &n[1]; n[0].PPrevious = 0;
    n[1].PMapwho = &m[1]; n[1].PNext = 0; n[1].PPrevious = &n[0];
    n[2].PMapwho = &m[2]; n[2].PNext = 0; n[2].PPrevious = 0;
    CThingMap map;
    map.Info[0].Size = 10; map.Info[0].ExtentsX = 2; map.Info[1].Size = -1; map.Info[1].ExtentsX = 1;
    map.X0 = 0; map.Y0 = 0; map.X1 = 20; map.Y1 = 20; map.NoThings = 3;
    for (int i = 0; i < 4; ++i) map.Cells0[i].PFirstNode = 0;
    map.Cells0[3].PFirstNode = &n[0];
    map.Cells1[0].PFirstNode = &n[2];
    map.Clear();
    bool ok = map.NoThings == 0 && map.Cells0[3].PFirstNode == 0 && map.Cells1[0].PFirstNode == 0;
    for (int i = 0; i < 3; ++i) ok = ok && !m[i].InMapwho && n[i].PNext == 0 && n[i].PPrevious == 0;
    if (!ok) { std::printf("0085cec0_TEST FAIL\n"); return 1; }
    std::printf("0085cec0_TEST PASS\n");
    return 0;
}