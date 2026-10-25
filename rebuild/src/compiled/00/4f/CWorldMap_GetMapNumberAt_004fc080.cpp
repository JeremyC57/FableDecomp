// CWorldMap::GetMapNumberAt(const C2DCoordI&) const -- map number under a world block (0 outside the world area)
// Leaf: ?GetMapNumberAt@CWorldMap@@UBEJABVC2DCoordI@@@Z
// Retail: Fable.exe @ 0x004FC080, 64 bytes, `ret 4`. PDB ?GetMapNumberAt@CWorldMap@@UBEJABVC2DCoordI@@@Z (primary
// vtable 0x01244AEC slot 17, +0x44; the C3DVector overload 0x004FC0C0 is slot 16).
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers, so a local model is used):
//   +0x54 GranularityShift (PDB +0x64), +0x58 MapPlacementPitch (PDB +0x68), +0x5C Area {TLX, TLY, BRX, BRY}
//   (PDB +0x6C), +0x6C MapPlacement (vector<unsigned short> start, PDB +0x7C).
// Shape: the same half-open area test as the landed C3DVector overload, written coordinate-first (`cmp esi,ecx`),
// then the coordinates scaled down by GranularityShift index the 16-bit placement grid.
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class C2DCoordI { public: long X, Y; };

class CWorldMap {
public:
    virtual void v00();
    unsigned char   _pad04[0x50];
    long            GranularityShift;           // +0x54
    long            MapPlacementPitch;          // +0x58
    C2DBoxI         Area;                       // +0x5C
    unsigned short* MapPlacement;               // +0x6C vector<unsigned short> start
    unsigned short* MapPlacementFinish;         // +0x70
    unsigned short* MapPlacementEnd;            // +0x74
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;
};

long CWorldMap::GetMapNumberAt(const C2DCoordI& pos) const
{
    if (pos.X >= Area.TLX && pos.X < Area.BRX && pos.Y >= Area.TLY && pos.Y < Area.BRY)
        return MapPlacement[(pos.Y >> GranularityShift) * MapPlacementPitch + (pos.X >> GranularityShift)];
    return 0;
}