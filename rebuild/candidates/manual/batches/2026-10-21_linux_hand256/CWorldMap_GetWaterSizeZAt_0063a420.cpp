// CWorldMap::GetWaterSizeZAt(const C3DVector& pos) const -- water height above the ground at a world position: the
// position and its +1 neighbours along X and Y must all lie on the same loaded map; the map's water offset at the
// local position counts only above 0.05, else 0
// Leaf: GetWaterSizeZAt
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063A420, 275 bytes, `ret 4`. PDB ?GetWaterSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z.
// Callees: __ftol2 0x00BFEA70 (x6), IsPosInMap(const C3DVector&) 0x004FC600, IsPosInMap(const C2DCoordI&) 0x004FC5B0,
//   GetMapNumberAt(const C2DCoordI&) virtual slot 17 (+0x44), IsMapLoaded virtual slot 10 (+0x28),
//   ConvertWorldMapCoordsToLocalCoords(long, const C3DVector&) 0x004FC2A0, PeekMap virtual slot 19 (+0x4c),
//   CMap::GetWaterSizeZOffsetAt(const C3DVector&) 0x0081FEC0. Constants 1.0f 0x0122DED8, 0.0f 0x0122DEDC,
//   0.05f 0x0125A15C.
// Layout: the local retail CWorldMap model of the landed sibling GetThemeMinCameraSizeZOffsetAt 0x0063A540 (vtable
//   0x01244AEC; no data members are read here). Shape: the map reference is a named local (PeekMap runs before
//   &local is pushed), as in the sibling.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
};
struct C2DCoordI {
    long x, y;
    C2DCoordI() { }
    C2DCoordI(long ax, long ay) : x(ax), y(ay) { }
};
class CMap {
public:
    float GetWaterSizeZOffsetAt(const C3DVector& localPos) const;   // 0x0081FEC0
};
class CWorldMap {
public:
    virtual ~CWorldMap();                                           // slot 0
    virtual void DeleteAllLevels();                                 // slot 1
    virtual void DeleteAllRegions();                                // slot 2
    virtual void LoadFromFile();                                    // slot 3
    virtual void SaveToFile();                                      // slot 4
    virtual void RemoveLevel(long map);                             // slot 5
    virtual void LoadTopologyFromFile();                            // slot 6
    virtual void PostLoadTopology(long map);                        // slot 7
    virtual void ForceLoad(long map);                               // slot 8
    virtual void ForceUnload(long map);                             // slot 9
    virtual bool IsMapLoaded(long map) const;                       // slot 10 (+0x28) 0x004FC050
    virtual bool IsMapUsed(long map) const;                         // slot 11
    virtual long GetGranularity() const;                            // slot 12
    virtual const void* GetDimensions() const;                      // slot 13
    virtual CMap& GetMapAt(const C2DCoordI& pos);                   // slot 14
    virtual const CMap& PeekMapAt(const C2DCoordI& pos) const;      // slot 15
    // overloaded virtuals are laid out in reverse declaration order (C3DVector form is slot 16)
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;        // slot 17 (+0x44) 0x004FC080
    virtual long GetMapNumberAt(const C3DVector& pos) const;        // slot 16 (+0x40) 0x004FC0C0
    virtual CMap& GetMap(long map);                                 // slot 18 (+0x48) 0x0051CE20
    virtual const CMap& PeekMap(long map) const;                    // slot 19 (+0x4C) 0x0051CE40
    bool IsPosInMap(const C2DCoordI& pos) const;                    // 0x004FC5B0
    bool IsPosInMap(const C3DVector& pos) const;                    // 0x004FC600
    C3DVector ConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const;   // 0x004FC2A0
    float GetWaterSizeZAt(const C3DVector& pos) const;
};
float CWorldMap::GetWaterSizeZAt(const C3DVector& pos) const
{
    C2DCoordI posX1((long)(pos.X + 1.0f), (long)pos.Y);
    C2DCoordI posY1((long)pos.X, (long)(pos.Y + 1.0f));
    if (IsPosInMap(pos) && IsPosInMap(posX1) && IsPosInMap(posY1)) {
        long map = GetMapNumberAt(C2DCoordI((long)pos.X, (long)pos.Y));
        if (map != 0 && GetMapNumberAt(posX1) == map && GetMapNumberAt(posY1) == map && IsMapLoaded(map)) {
            C3DVector local = ConvertWorldMapCoordsToLocalCoords(map, pos);
            const CMap& theMap = PeekMap(map);
            float height = theMap.GetWaterSizeZOffsetAt(local);
            if (height > 0.05f)
                return height;
        }
    }
    return 0.0f;
}
