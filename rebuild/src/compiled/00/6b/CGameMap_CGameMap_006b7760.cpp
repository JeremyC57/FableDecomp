// CGameMap::CGameMap(CWorld* world, CMap* map, const C2DCoordI& size) -- CBase base, own vtable, an empty cell vector, the map/world pointers and the cell grid size; when SizeX * SizeY fits a positive 32-bit count, size the cell vector to it and clear every cell
// Leaf: ??0CGameMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B7760, length 137 bytes. `ret 0Ch`.
// Callees: CBase::CBase @ 0x0099A2F0, vector<CGameMapCell>::resize(unsigned, const CGameMapCell&) @ 0x006B81F0 (label propagated from the
// CBrainBehaviour instantiation), CGameMap::Clear @ 0x006B7510 (landed). Vtable 0x0125D558 = CGameMap (rtti-vtables.tsv).
// Layout: Ego_r.pdb CGameMap with the retail 12-byte vector (as the landed Clear): Cells +4, Map +0x10, World +0x14, WorldSeed +0x18,
// SizeX +0x1c, SizeY +0x20.
// Shape: the init list in declaration order after the vtable store (the STLport vector's three zero words); the overflow guard is a
// 64-bit product of the SIZE ARGUMENT's fields tested against [0, 0x7fffffff]; the fill value is a `CGameMapCell()` TEMPORARY of a cell
// whose user constructor does nothing (frame bytes 1..7, `lea eax,[esp+0Dh]`; a named local sits at frame byte 0).
class CWorld;
class CMap;

class C2DCoordI { public:
    long X;
    long Y;
};

class CGameMapCell { public:
    unsigned char AtmosThemes[3];                   // +0
    unsigned char AtmosBlends[2];                   // +3
    signed char   SoundTheme;                       // +5
    unsigned char Music;                            // +6
    CGameMapCell() { }                              // leaves the cell uninitialised
};

struct CGameMapCellVector {                         // retail STLport vector<CGameMapCell>
    CGameMapCell* _M_start;
    CGameMapCell* _M_finish;
    CGameMapCell* _M_end_of_storage;
    CGameMapCellVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void resize(unsigned long count, const CGameMapCell& value);   // 0x006B81F0
};

struct CBase {
    virtual void v00();
    CBase();                                        // 0x0099A2F0
};

class CGameMap : public CBase { public:                           // vtable 0x0125D558
    CGameMapCellVector Cells;                       // +0x04
    CMap*              Map;                         // +0x10
    CWorld*            World;                       // +0x14
    unsigned long*     WorldSeed;                   // +0x18
    long               SizeX;                       // +0x1c
    long               SizeY;                       // +0x20
    virtual void v00();
    CGameMap(CWorld* world, CMap* map, const C2DCoordI& size);
    void Clear();                                   // 0x006B7510
};

CGameMap::CGameMap(CWorld* world, CMap* map, const C2DCoordI& size)
    : CBase()
    , Map(map)
    , World(world)
    , SizeX(size.X)
    , SizeY(size.Y)
{
    __int64 total = (__int64)size.X * size.Y;
    if (total >= 0 && total <= 0x7fffffff) {
        Cells.resize(size.X * size.Y, CGameMapCell());
        Clear();
    }
}