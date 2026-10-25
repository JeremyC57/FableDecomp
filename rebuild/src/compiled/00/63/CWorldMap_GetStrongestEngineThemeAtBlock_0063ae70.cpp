// CWorldMap::GetStrongestEngineThemeAtBlock(const C2DCoordI&, long&) const -- the strongest engine theme on a loaded map's block (0 elsewhere)
// Leaf: ?GetStrongestEngineThemeAtBlock@CWorldMap@@QBEJABVC2DCoordI@@AAJ@Z
// Retail: Fable.exe @ 0x0063AE70, 95 bytes, `ret 8`. PDB ?GetStrongestEngineThemeAtBlock@CWorldMap@@QBEJABVC2DCoordI@@AAJ@Z
// Callees: primary virtuals (vtable 0x01244AEC) slot 17 GetMapNumberAt(const C2DCoordI&) 0x004FC080 (+0x44), slot 10
// IsMapLoaded(long) 0x004FC050 (+0x28), slot 19 PeekMap(long) 0x0051CE40 (+0x4C);
// CWorldMap::ConvertWorldMapCoordsToLocalCoords(long, const C2DCoordI&) const 0x004FC3C0 (out of line in this TU),
// CMap::GetStrongestEngineThemeAt(const C2DCoordI&, long&) const 0x0081F910 (PDB
// ?GetStrongestEngineThemeAt@CMap@@IBEJABVC2DCoordI@@AAJ@Z; manifest label "CMap::CopyPos" is a propagation).
// Shape: as SetGroundSizeZAtBlock 0x0063A910 (named frame block, setter arguments pushed before the map fetch); the
// two exits keep their own epilogues, the miss path returns 0.
class C2DCoordI {
public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};

class CMap {
public:
    long GetStrongestEngineThemeAt(const C2DCoordI& pos, long& strength) const;   // 0x0081F910
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long map) const;                    // slot 10 (+0x28) 0x004FC050
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;     // slot 17 (+0x44) 0x004FC080
    virtual CMap& GetMap(long map);                              // slot 18 (+0x48) 0x0051CE20
    virtual const CMap& PeekMap(long map) const;                 // slot 19 (+0x4C) 0x0051CE40
    C2DCoordI ConvertWorldMapCoordsToLocalCoords(long map, const C2DCoordI& pos) const;    // 0x004FC3C0
    long GetStrongestEngineThemeAtBlock(const C2DCoordI& pos, long& strength) const;
};

long CWorldMap::GetStrongestEngineThemeAtBlock(const C2DCoordI& pos, long& strength) const
{
    long map = GetMapNumberAt(pos);
    if (map && IsMapLoaded(map))
    {
        C2DCoordI local = ConvertWorldMapCoordsToLocalCoords(map, pos);
        return PeekMap(map).GetStrongestEngineThemeAt(local, strength);
    }
    return 0;
}