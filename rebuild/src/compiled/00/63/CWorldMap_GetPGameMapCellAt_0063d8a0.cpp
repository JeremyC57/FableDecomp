// CWorldMap::GetPGameMapCellAt(const C3DVector&) const -- the game-map cell (atmos/sound/music themes) under a world position, or 0 when the position is outside the world or its map is not loaded
// Leaf: GetPGameMapCellAt
// Retail: Fable.exe @ 0x0063D8A0, 182 bytes, `ret 4`. PDB ?GetPGameMapCellAt@CWorldMap@@QBEPBVCGameMapCell@@ABVC3DVector@@@Z
// Callees: CWorldMap::IsPosInMap(const C3DVector&) const 0x004FC600, _ftol2 0x00BFEA70, CWorldMap virtual slots 17
// GetMapNumberAt(const C2DCoordI&) const (+0x44), 10 IsMapLoaded(long) const (+0x28), 19 PeekMap(long) const (+0x4C);
// ConvertWorldMapCoordsToLocalCoords(long, const C3DVector&) const 0x004FC2A0 (C3DVector by hidden pointer).
// Float pool 0x0123078C = 0.25f (four world units per game-map cell).
// Retail offsets: CMap GameMap (scoped_ptr<CGameMap>) +0x1A78 (PDB +0x1A88); CGameMap Cells (vector<CGameMapCell>,
// 7-byte cells) +0x04, SizeX +0x1C, SizeY +0x20 (PDB +4 after the 16-byte vector).
// Shape: the world coordinate is a C2DCoordI temporary (X truncated first) whose slot the local C3DVector reuses; the
// cell is an inline CGameMap accessor taking a C2DCoordI temporary (gives the X-first truncation order) and indexing
// rows bottom-up (`(SizeY - y - 1) * SizeX + x`).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};
class C2DCoordI { public: long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };

class CGameMapCell { public:                                        // 7 bytes
    unsigned char AtmosThemes[3];
    unsigned char AtmosBlends[2];
    signed char   SoundTheme;
    unsigned char Music;
};
template <class T> struct CVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T& operator[](unsigned long n) const { return *(begin() + n); }
};
class CGameMap { public:
    void*                 _vptr;                             // +0x00
    CVector<CGameMapCell> Cells;                             // +0x04
    void*                 Map;                               // +0x10
    void*                 World;                             // +0x14
    unsigned long*        WorldSeed;                         // +0x18
    long                  SizeX;                             // +0x1C
    long                  SizeY;                             // +0x20
    const CGameMapCell* PeekCell(const C2DCoordI& pos) const { return &Cells[(SizeY - pos.Y - 1) * SizeX + pos.X]; }
};
class CMap { public:
    unsigned char _pad[0x1a78];
    CGameMap*     GameMap;                                   // +0x1A78 scoped_ptr<CGameMap>
};

class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long map) const;                          // slot 10 (+0x28)
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;           // slot 17 (+0x44)
    virtual long GetMapNumberAt(const C3DVector& pos) const;           // slot 16 (reverse declaration order)
    virtual void v18();
    virtual const CMap& PeekMap(long map) const;                       // slot 19 (+0x4C)

    bool IsPosInMap(const C3DVector& pos) const;                       // 0x004FC600
    C3DVector ConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const;   // 0x004FC2A0
    const CGameMapCell* GetPGameMapCellAt(const C3DVector& pos) const;
};

const CGameMapCell* CWorldMap::GetPGameMapCellAt(const C3DVector& pos) const
{
    if (IsPosInMap(pos))
    {
        long map = GetMapNumberAt(C2DCoordI((long)pos.X, (long)pos.Y));
        if (map && IsMapLoaded(map))
        {
            C3DVector local = ConvertWorldMapCoordsToLocalCoords(map, pos);
            const CMap& gameMap = PeekMap(map);
            return gameMap.GameMap->PeekCell(C2DCoordI((long)(local.X * 0.25f), (long)(local.Y * 0.25f)));
        }
    }
    return 0;
}