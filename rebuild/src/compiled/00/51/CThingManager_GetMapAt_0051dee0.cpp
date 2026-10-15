// CThingManager::GetMapAt(const C3DVector& pos) -- truncates the position's x/y to a cell coordinate and returns the world map's CMap there
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051DEE0, length 55 bytes.
// Callees: __ftol2 @ 0x00BFEA70 (x2), CWorldMap::GetMapAt(const C2DCoordI&) virtual slot 14 (+0x38).
// Reached by the CThing::GetMapAt thunk 0x004C79B0 (`mov ecx,[0x013B8A1C] ; jmp`, the global thing manager).
// FableWin symbol: ?GetMapAt@CThingManager@@QAEAAVCMap@@ABVC3DVector@@@Z (returns CMap&).
// Layout: the world map pointer sits at +0x28 in retail (PDB CThingManager::WorldMap +0x34; the
// retail base is 0xc smaller). Shape: the coordinate is a frame local filled member by member
// (x converted and stored first -- a two-argument constructor would evaluate y first), its
// address passed to the virtual call, whose result is returned unchanged.
struct C3DVector {
    float X, Y, Z;
};
struct C2DCoordI {
    long X;
    long Y;
};
struct CMap;
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    // MSVC lays overloaded virtuals out in REVERSE declaration order: the const overload
    // declared first lands in slot 15, the non-const one in slot 14.
    virtual const CMap& GetMapAt(const C2DCoordI& pos) const;   // slot 15 (+0x3c)
    virtual CMap& GetMapAt(const C2DCoordI& pos);               // slot 14 (+0x38)
};
struct CThingManager {
    unsigned char _pad00[0x28];
    CWorldMap*    WorldMap;                    // +0x28
    CMap& GetMapAt(const C3DVector& pos);
};

CMap& CThingManager::GetMapAt(const C3DVector& pos)
{
    C2DCoordI cell;
    cell.X = (long)pos.X;
    cell.Y = (long)pos.Y;
    return WorldMap->GetMapAt(cell);
}