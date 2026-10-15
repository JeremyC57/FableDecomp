#include <cstdio>
// Behaviour test for CThingMapManager::RemoveMapwhoNodeFromCell (0x00637980).
struct CMapwhoNode;
struct CMapwhoCell { CMapwhoNode* PHead; };
struct CMapwhoNode { void* PThing; CMapwhoNode* PNext; CMapwhoNode* PPrev; CMapwhoCell* PCell; };
static void RemoveMapwhoNodeFromCell(CMapwhoCell* cell, CMapwhoNode* node)
{
    if (node->PNext != 0) node->PNext->PPrev = node->PPrev;
    if (node->PPrev != 0) node->PPrev->PNext = node->PNext;
    else if (cell->PHead == node) cell->PHead = node->PNext;
    node->PPrev = 0; node->PNext = 0; node->PCell = 0;
}
int main()
{
    CMapwhoCell cell; CMapwhoNode n[3];
    for (int i = 0; i < 3; ++i) { n[i].PThing = 0; n[i].PCell = &cell; }
    n[0].PPrev = 0; n[0].PNext = &n[1]; n[1].PPrev = &n[0]; n[1].PNext = &n[2]; n[2].PPrev = &n[1]; n[2].PNext = 0;
    cell.PHead = &n[0];
    int fails = 0;
    RemoveMapwhoNodeFromCell(&cell, &n[1]);           // middle
    if (n[0].PNext != &n[2] || n[2].PPrev != &n[0] || cell.PHead != &n[0]) ++fails;
    if (n[1].PNext || n[1].PPrev || n[1].PCell) ++fails;
    RemoveMapwhoNodeFromCell(&cell, &n[0]);           // head
    if (cell.PHead != &n[2] || n[2].PPrev != 0 || n[0].PCell) ++fails;
    RemoveMapwhoNodeFromCell(&cell, &n[2]);           // last
    if (cell.PHead != 0) ++fails;
    if (fails) { std::printf("00637980_TEST FAIL\n"); return 1; }
    std::printf("00637980_TEST PASS\n");
    return 0;
}
