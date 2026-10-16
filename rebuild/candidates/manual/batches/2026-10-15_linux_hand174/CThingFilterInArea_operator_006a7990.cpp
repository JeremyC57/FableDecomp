// CThingFilter_InArea::operator()(CThing* thing) -- true when the thing has a physics component whose XY position lies inside the box Area (TL inclusive, BR exclusive)
// Leaf: ??RCThingFilter_InArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7990, length 100 bytes.
// Callees: none (x87 only).
// Layout: PDB CThingFilter_InArea { C2DBoxF Area; } with C2DBoxF { TLX, TLY, BRX, BRY } (16 bytes); retail CThing
// PhysicsTC +0x60, the component's position C3DVector at +0x0C.
// Shape: the position is taken into a by-value C2DVector (x stays on the FPU, y goes through the 8-byte frame
// local) and tested by an inline C2DBoxF::Contains; `x >= TLX` gives `fcom ; test ah,1 ; jne`, `x < BRX` gives
// `fcomp ; test ah,5 ; jp`; the && chain materialises an int, the absent component the byte `xor al,al`.
struct C3DVector {
    float x, y, z;
};

struct C2DVector {
    float x, y;
    C2DVector(float ax, float ay) : x(ax), y(ay) {}
};

struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    bool Contains(const C2DVector& p) const
    {
        return p.x >= TLX && p.x < BRX && p.y >= TLY && p.y < BRY;
    }
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

struct CThingFilter_InArea {
    C2DBoxF Area;                                   // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_InArea::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (physics)
        return Area.Contains(C2DVector(physics->m_position.x, physics->m_position.y));
    return false;
}
