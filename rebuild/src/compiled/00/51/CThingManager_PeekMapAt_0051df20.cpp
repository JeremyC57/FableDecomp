// CThingManager::PeekMapAt(const C3DVector& pos) const -- truncates the position's x/y to a cell coordinate and returns the
// world map's CMap there (the const twin of GetMapAt 0x0051DEE0)
// Leaf: ?PeekMapAt@CThingManager@@QBEABVCMap@@ABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051DF20, length 55 bytes.
// Callees: __ftol2 @ 0x00BFEA70 (x2), CWorldMap::PeekMapAt(const C2DCoordI&) const virtual slot 15 (+0x3c).
// Identity (sweep3; was "CThingManager::GetMapAt", the const overload of a name the PDB does not overload): Ego_r.pdb
//   lists ?GetMapAt@CThingManager@@QAEAAVCMap@@ABVC3DVector@@@Z then ?PeekMapAt@CThingManager@@QBEABVCMap@@ABVC3DVector@@@Z,
//   0x40 apart as retail 0x0051DEE0 / 0x0051DF20; the CThing forwarders in the same Ego_r / FableWin order (CThing::GetMapAt
//   0x004C79B0 jumps to 0x0051DEE0, CThing::PeekMapAt 0x004C79C0 to this body); the world-map call is likewise the const
//   Ego_r ?PeekMapAt@CWorldMap@@UBEABVCMap@@ABVC2DCoordI@@@Z (declared after ?GetMapAt@CWorldMap@@UAE..., slots 14 / 15).
// Layout: the world map pointer sits at +0x28 in retail (PDB CThingManager::WorldMap +0x34; the
// retail base is 0xc smaller). Shape: the coordinate is a frame local filled member by member
// (x converted and stored first -- a two-argument constructor would evaluate y first), its
// address passed to the virtual call, whose result is returned unchanged.
class C3DVector {
public:
    float X, Y, Z;
};
class C2DCoordI {
public:
    long X;
    long Y;
};
class CMap;
class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual CMap& GetMapAt(const C2DCoordI& pos);               // slot 14 (+0x38)
    virtual const CMap& PeekMapAt(const C2DCoordI& pos) const;  // slot 15 (+0x3c)
};
class CThingManager {
public:
    unsigned char _pad00[0x28];
    CWorldMap*    WorldMap;                    // +0x28
    const CMap& PeekMapAt(const C3DVector& pos) const;
};

const CMap& CThingManager::PeekMapAt(const C3DVector& pos) const
{
    C2DCoordI cell;
    cell.X = (long)pos.X;
    cell.Y = (long)pos.Y;
    return static_cast<const CWorldMap*>(WorldMap)->PeekMapAt(cell);
}