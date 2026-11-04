// NThingMap::CThingMap::Validate() -- walk every cell of every level and check each cell's node list (the release build's checks compile away, leaving the walks)
// Grade: DIFFER(214v214) -- the row is CThingMap::Validate (Ego_r.pdb Validate 0x00853700 sits before Clear 0x00853720; retail Clear is the landed 0x0085CEC0), rewritten on the landed same-TU Clear model with an inline ValidateCell walk; width/height defaulted to 1 gives the retail length (if/else adds loop alignment, 229 bytes); left: this in ecx (retail ebp), width in ebp (retail ecx) and the then-arm jmp; 16 grades (if/else orders, C2DExtentsI local, ValidateCell loop forms)
// Leaf: ?Validate@CThingMap@NThingMap@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085CDE0, length 214 bytes, no callees; directly before Clear 0x0085CEC0 as Ego_r.pdb
//   Validate 0x00853700 is before Clear 0x00853720 (the row was parked under the name Clear, which is 0x0085CEC0's).
// Globals: g_NoMapwhoLevels @ 0x013BA848 (as the landed Clear / GetMemoryFootprint).
// Layout: the landed same-TU Clear 0x0085CEC0 model (PDB NThingMap::CThingMap, retail 12-byte vectors).
// Shape: the Clear grid walk; per cell the inline protected ValidateCell(const CThingMapCell&) const walks the nodes and, for
//   each, the rest of the list (the duplicate check's assert is compiled out).
namespace NThingMap {
struct C2DBoxI { long X0, Y0, X1, Y1; };
struct C2DExtentsI { long X, Y; };
class CMapwhoLevelInfo { public:
    long        Size;                              // +0x00
    C2DExtentsI Extents;                           // +0x04
    long        ScaleShift;                        // +0x0c
    long        NoThings;                          // +0x10
};
class CMapwhoNode;
class CThingMapCell { public:
    CMapwhoNode* PFirstNode;
};
struct CTCMapwho {
    unsigned char _pad00[0x50];
    unsigned char InMapwho : 1;                    // +0x50 bit 0
};
class CMapwhoNode { public:
    CTCMapwho*     PMapwho;                        // +0x00
    CMapwhoNode*   PNext;                          // +0x04
    CMapwhoNode*   PPrevious;                      // +0x08
    CThingMapCell* PCell;                          // +0x0c
};
template<class T> struct CRetailVector {           // STLport vector: {start, finish, end_of_storage}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
extern long g_NoMapwhoLevels;                      // 0x013BA848
class CThingMap { public:
    virtual void v00();
    CRetailVector<CMapwhoLevelInfo>               MapwhoLevelInfo;   // +0x04
    void*                                         Manager;           // +0x10
    CRetailVector< CRetailVector<CThingMapCell> > Cells;             // +0x14
    C2DBoxI                                       Area;              // +0x20
    long                                          NoThings;          // +0x30
    CThingMapCell& GetCell(unsigned char level, long x, long y) { return Cells[level][MapwhoLevelInfo[level].Extents.X * y + x]; }
    void ValidateCell(const CThingMapCell& cell) const
    {
        for (CMapwhoNode* node = cell.PFirstNode; node; node = node->PNext)
            for (CMapwhoNode* other = node->PNext; other; other = other->PNext)
                ;
    }
    void Validate();
};

void CThingMap::Validate()
{
    for (long level = 0; level < g_NoMapwhoLevels; ++level) {
        long size = MapwhoLevelInfo[level].Size;
        long width = 1;
        long height = 1;
        if (size != -1) {
            width = (Area.X1 - Area.X0) / size;
            height = (Area.Y1 - Area.Y0) / size;
        }
        for (long y = 0; y < height; ++y)
            for (long x = 0; x < width; ++x)
                ValidateCell(GetCell((unsigned char)level, x, y));
    }
}
}
