// Behaviour test for NThingMap::CThingMap::~CThingMap (0x0085D0A0): every mapwho node of every cell of every level is
// unlinked (cell, next and previous zeroed) and every cell head cleared, before the vectors are freed.
#include <cstdio>
#include <cstdlib>
namespace NThingMap {
long g_NoMapwhoLevels = 2;
class CThingMapCell;
class CMapwhoNode { public:
    void*          PMapwho;
    CMapwhoNode*   PNext;
    CMapwhoNode*   PPrevious;
    CThingMapCell* PCell;
};
class CThingMapCell { public:
    CMapwhoNode* PFirstNode;
};
struct CCellVector {                                        // the level's cell vector {start, finish}
    CThingMapCell* Start;
    CThingMapCell* Finish;
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    CThingMapCell& operator[](unsigned int n) { return Start[n]; }
};
class CThingMap { public:
    CCellVector* Cells;
    CThingMap() : Cells(0) { }
    ~CThingMap()
    {
        for (long level = 0; level < g_NoMapwhoLevels; ++level) {
            for (unsigned int i = 0; i < Cells[level].size(); ++i) {
                CThingMapCell& cell = Cells[level][i];
                CMapwhoNode* node = cell.PFirstNode;
                while (node) {
                    CMapwhoNode* next = node->PNext;
                    node->PCell = 0;
                    node->PNext = 0;
                    node->PPrevious = 0;
                    node = next;
                }
                cell.PFirstNode = 0;
            }
        }
    }
};
}
using namespace NThingMap;

int main()
{
    CThingMapCell cells0[3], cells1[1];
    CMapwhoNode nodes[4];
    CCellVector levels[2];
    levels[0].Start = cells0; levels[0].Finish = cells0 + 3;
    levels[1].Start = cells1; levels[1].Finish = cells1 + 1;
    cells0[0].PFirstNode = &nodes[0]; nodes[0].PNext = &nodes[1]; nodes[1].PNext = 0;
    nodes[0].PPrevious = 0; nodes[1].PPrevious = &nodes[0]; nodes[0].PCell = nodes[1].PCell = &cells0[0];
    cells0[1].PFirstNode = 0;
    cells0[2].PFirstNode = &nodes[2]; nodes[2].PNext = 0; nodes[2].PPrevious = 0; nodes[2].PCell = &cells0[2];
    cells1[0].PFirstNode = &nodes[3]; nodes[3].PNext = 0; nodes[3].PPrevious = 0; nodes[3].PCell = &cells1[0];
    {
        CThingMap map;
        map.Cells = levels;
    }
    for (int i = 0; i < 4; ++i)
        if (nodes[i].PNext || nodes[i].PPrevious || nodes[i].PCell) { std::printf("0085d0a0_TEST FAIL\n"); return 1; }
    if (cells0[0].PFirstNode || cells0[2].PFirstNode || cells1[0].PFirstNode) { std::printf("0085d0a0_TEST FAIL\n"); return 1; }
    std::printf("0085d0a0_TEST PASS\n");
    return 0;
}