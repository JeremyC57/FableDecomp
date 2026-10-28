// CMap::GetSoundAt(const C2DCoordI& pos) const -- the sound theme of the game-map cell under a map position (the position divided by 4 into game-map cells)
// Leaf: ?GetSoundAt@CMap@@QBEJABVC2DCoordI@@@Z
// Retail: Fable.exe @ 0x0081F590, 65 bytes, thiscall `ret 4`.
// Identity (proven): FableWin.pdb ?GetSoundAt@CMap@@QBEJABVC2DCoordI@@@Z (the catalog name); the body reads
//   CGameMapCell::SoundTheme (+5 of a 7-byte cell, signed char) through the CMap's GameMap.
// Body: GameMap (+0x1a78, scoped_ptr<CGameMap>; Ego_r.pdb +0x1a88) -> PeekCell(C2DCoordI(pos.X / 4, pos.Y / 4)) (inline:
//   Cells[(SizeY - y - 1) * SizeX + x], rows stored bottom-up; FableWin.pdb ?PeekCell@CGameMap@@QBEABVCGameMapCell@@ABVC2DCoordI@@@Z)
//   .GetSoundTheme() (inline, ?GetSoundTheme@CGameMapCell@@QBECXZ). Retail CGameMap: Cells +0x04 (12-byte STLport vector),
//   SizeX +0x1c, SizeY +0x20 (Ego_r.pdb +0x20 / +0x24).
class C2DCoordI {
public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
class CGameMapCell {
public:
    unsigned char AtmosThemes[3];                        // +0
    unsigned char AtmosBlends[2];                        // +3
    signed char   SoundTheme;                            // +5
    unsigned char Music;                                 // +6
    signed char GetSoundTheme() const { return SoundTheme; }
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef const _Tp* const_iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    const_iterator begin() const { return _M_start; }
    const _Tp& operator[](unsigned int __n) const { return *(begin() + __n); }
};
}
class CGameMap {
public:
    virtual void v00();
    std::vector<CGameMapCell> Cells;                     // +0x04
    void*          Map;                                  // +0x10
    void*          World;                                // +0x14
    unsigned long* WorldSeed;                            // +0x18
    long           SizeX;                                // +0x1c
    long           SizeY;                                // +0x20
    const CGameMapCell& PeekCell(const C2DCoordI& pos) const { return Cells[(SizeY - pos.Y - 1) * SizeX + pos.X]; }
};
class CMap {
public:
    unsigned char _pad00[0x1a78];
    CGameMap* GameMap;                                   // +0x1a78
    long GetSoundAt(const C2DCoordI& pos) const;
};
long CMap::GetSoundAt(const C2DCoordI& pos) const
{
    return GameMap->PeekCell(C2DCoordI(pos.X / 4, pos.Y / 4)).GetSoundTheme();
}