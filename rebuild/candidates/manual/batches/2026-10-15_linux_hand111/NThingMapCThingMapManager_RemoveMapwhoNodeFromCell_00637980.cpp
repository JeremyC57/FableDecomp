// NThingMap::CThingMapManager::RemoveMapwhoNodeFromCell(CMapwhoCell* cell, CMapwhoNode* node) -- unlinks a node from its cell's doubly linked list (fixing the cell head) and clears the node's links and cell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00637980, length 72 bytes.
// No callees. A __thiscall member that never reads `this` (`ret 8`, ecx overwritten at once).
// Node layout: +0x04 next, +0x08 prev, +0x0c owning cell; the cell's first word is its head.
// Shape: null compared against a hoisted zero register (edx, also the value of the three clears);
// the clears (prev, next, cell -- in that order) are tail-duplicated into both arms of the
// `prev` test.
struct CMapwhoNode;
struct CMapwhoCell {
    CMapwhoNode* PHead;                         // +0x00
};
struct CMapwhoNode {
    void*        PThing;                        // +0x00
    CMapwhoNode* PNext;                         // +0x04
    CMapwhoNode* PPrev;                         // +0x08
    CMapwhoCell* PCell;                         // +0x0c
};
namespace NThingMap {
struct CThingMapManager {
    void RemoveMapwhoNodeFromCell(CMapwhoCell* cell, CMapwhoNode* node);
};

void CThingMapManager::RemoveMapwhoNodeFromCell(CMapwhoCell* cell, CMapwhoNode* node)
{
    if (node->PNext != 0)
        node->PNext->PPrev = node->PPrev;
    if (node->PPrev != 0)
        node->PPrev->PNext = node->PNext;
    else if (cell->PHead == node)
        cell->PHead = node->PNext;
    node->PPrev = 0;
    node->PNext = 0;
    node->PCell = 0;
}
}
