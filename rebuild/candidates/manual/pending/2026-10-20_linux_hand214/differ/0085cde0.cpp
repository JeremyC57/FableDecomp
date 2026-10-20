// CThingMap::Clear() -- walk every cell of every layer and each cell's thing-link list (the release work of this build compiles away, leaving the bare walk)
// Grade: DIFFER(229v214) -- register allocation and frame shape of the triple loop (retail keeps this in ebp, the layer in ebx with its frame copy, the row count in a frame word and the x count-down in ebx); one form tried, the empty link-walk body is speculative
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085CDE0, length 214 bytes, `ret`.
// Globals: the layer count @ 0x013BA848.
// Layout (Ego_r.pdb names, retail offsets = PDB minus 4 per 16-byte vector): MapwhoLevelInfo +0x04 (0x14-byte records:
//   cell size +0, cells per row +4), Manager +0x10,
//   Cells +0x14 (per-layer 12-byte vectors of list heads), Area +0x20 (C2DBoxI); a link's next
//   pointer at +4.
// Shape: a layer whose cell size is -1 is a single cell; otherwise the grid is the bounds divided by the cell size;
//   the cell is addressed through the byte-sized layer index; the inner walk is a link-list traversal whose per-link
//   body is empty.
struct SThingLink {
    void* Thing;                                // +0x00
    SThingLink* Next;                           // +0x04
};
struct CMapwhoLevelInfo {                         // 0x14 bytes
    long CellSize;                              // +0x00
    long CellsPerRow;                           // +0x04
    long _pad08[3];
};
struct SCellArray {                             // STLport vector<SThingLink*> (12 bytes)
    SThingLink** first;
    SThingLink** last;
    SThingLink** end;
};
struct C2DBoxI { long Left, Top, Right, Bottom; };
extern long g_NoMapwhoLevels;      // 0x013BA848

struct CThingMap {
    void* __vftable;                            // +0x00
    CMapwhoLevelInfo* MapwhoLevelInfo;                     // +0x04
    unsigned char _pad08[0x14 - 0x08];
    SCellArray* Cells;                          // +0x14
    unsigned char _pad18[0x20 - 0x18];
    C2DBoxI Area;                             // +0x20
    SThingLink** GetCell(unsigned char layer, long x, long y) { return Cells[layer].first + MapwhoLevelInfo[layer].CellsPerRow * y + x; }
    void Clear();
};

void CThingMap::Clear()
{
    for (long layer = 0; layer < g_NoMapwhoLevels; ++layer) {
        long width, height;
        long cellSize = MapwhoLevelInfo[layer].CellSize;
        if (cellSize == -1) {
            width = 1;
            height = 1;
        } else {
            width = (Area.Right - Area.Left) / cellSize;
            height = (Area.Bottom - Area.Top) / cellSize;
        }
        for (long y = 0; y < height; ++y) {
            for (long x = 0; x < width; ++x) {
                SThingLink* link = *GetCell((unsigned char)layer, x, y);
                while (link) {
                    SThingLink* next = link->Next;
                    for (SThingLink* tail = next; tail; tail = tail->Next)
                        ;
                    link = next;
                }
            }
        }
    }
}
