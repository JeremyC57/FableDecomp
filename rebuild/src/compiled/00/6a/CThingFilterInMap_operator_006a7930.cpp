// CThingFilter_InMap::operator()(CThing* thing) -- true when the thing has a physics component and the world map reports its position as lying in map MapNo
// Leaf: ??RCThingFilter_InMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7930, length 43 bytes (+ int3 padding).
// Callees: CWorldMap virtual slot 16 (`call [edx+40h]`, the map number at a position).
// Layout: PDB CThingFilter_InMap { CWorldMap* WorldMap; long MapNo; }; retail CThing PhysicsTC +0x60, the component's
// position C3DVector at +0x0C (`add eax,0Ch ; push eax`).
// Shape: `return call == MapNo;` gives `sub eax,[esi+4] ; neg ; sbb ; inc`; an absent component returns `xor al,al`.
struct C3DVector {
    float x, y, z;
};

struct CTCPhysicsBase {
    void*         __vftable;
    unsigned char _pad04[8];
    C3DVector     m_position;                       // +0x0c
};

struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
};

struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector* pos);   // slot 16 (+0x40)
};

struct CThingFilter_InMap {
    CWorldMap* WorldMap;                            // +0x00
    long       MapNo;                               // +0x04
    bool operator()(CThing* thing) const;
};

bool CThingFilter_InMap::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (physics)
        return WorldMap->GetMapNumberAt(&physics->m_position) == MapNo;
    return false;
}