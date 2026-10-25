// CWorldMap::SetGroundSizeZAtBlock(const C2DCoordI&, float) -- set the ground height of a world block on a changeable map
// Leaf: ?SetGroundSizeZAtBlock@CWorldMap@@QAEXABVC2DCoordI@@M@Z
// Retail: Fable.exe @ 0x0063A910, 96 bytes, `ret 8`. PDB ?SetGroundSizeZAtBlock@CWorldMap@@QAEXABVC2DCoordI@@M@Z
// Callees: CWorldMap::IsPosInMap(const C2DCoordI&) const 0x004FC5B0, primary virtuals (vtable 0x01244AEC) slot 17
// GetMapNumberAt(const C2DCoordI&) 0x004FC080 (+0x44), slot 25 IsPosChangeable(const C2DCoordI&) 0x004FB2F0 (+0x64),
// slot 18 GetMap(long) 0x0051CE20 (+0x48); CWorldMap::ConvertWorldMapCoordsToLocalCoords(long, const C2DCoordI&) const
// 0x004FC3C0 (out of line: this TU is not CWorldMap.cpp), CMap::SetGroundSizeZAt(const C2DCoordI&, float) 0x008201D0
// (PDB ?SetGroundSizeZAt@CMap@@IAEXABVC2DCoordI@@M@Z; manifest label "_Cons_val<...>" is a propagation).
// Shape: the local block is a named frame C2DCoordI filled through the hidden result before the setter's arguments
// are pushed; the float is forwarded as a dword and the map object is fetched last (`GetMap(map).Set(local, size)`).
class C2DCoordI {
public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};

class CMap {
public:
    void SetGroundSizeZAt(const C2DCoordI& pos, float size);   // 0x008201D0
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;     // slot 17 (+0x44) 0x004FC080
    virtual CMap& GetMap(long map);                              // slot 18 (+0x48) 0x0051CE20
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual bool IsPosChangeable(const C2DCoordI& pos) const;    // slot 25 (+0x64) 0x004FB2F0
    bool IsPosInMap(const C2DCoordI& pos) const;                 // 0x004FC5B0
    C2DCoordI ConvertWorldMapCoordsToLocalCoords(long map, const C2DCoordI& pos) const;    // 0x004FC3C0
    void SetGroundSizeZAtBlock(const C2DCoordI& pos, float size);
};

void CWorldMap::SetGroundSizeZAtBlock(const C2DCoordI& pos, float size)
{
    if (IsPosInMap(pos))
    {
        long map = GetMapNumberAt(pos);
        if (map && IsPosChangeable(pos))
        {
            C2DCoordI local = ConvertWorldMapCoordsToLocalCoords(map, pos);
            GetMap(map).SetGroundSizeZAt(local, size);
        }
    }
}