// CGameMap::GetMemoryFootprint() -- report seven bytes per cell in the map's rectangular grid.
// Retail: 0x006B74B0, 11 bytes; no callees. Ego_r.pdb names SizeX/SizeY and CGameMapCell's seven bytes.
// The landed ctor 0x006B7760 confirms retail offsets +0x1c/+0x20 (the donor vector is four bytes larger).
// No shared CGameMap header exists. Only the field types needed by this method are complete here.
class CGameMapCell { public:
    unsigned char AtmosThemes[3];
    unsigned char AtmosBlends[2];
    signed char SoundTheme;
    unsigned char Music;
};
class CMap;
class CWorld;
class CGameMap { public:
    void* __vftable;
    CGameMapCell* CellsFirst;
    CGameMapCell* CellsLast;
    CGameMapCell* CellsEnd;
    CMap* Map;
    CWorld* World;
    unsigned long* WorldSeed;
    long SizeX;
    long SizeY;
    unsigned long GetMemoryFootprint() const;
};
unsigned long CGameMap::GetMemoryFootprint() const
{
    return SizeY * SizeX * sizeof(CGameMapCell);
}