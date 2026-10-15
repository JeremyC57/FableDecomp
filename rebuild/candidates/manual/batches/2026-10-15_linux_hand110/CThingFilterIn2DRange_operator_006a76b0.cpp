// CThingFilter_In2DRange::operator()(CThing* thing) -- true when the thing has a physics component and its position lies strictly within Range of Pos in the XY plane
// Leaf: ??RCThingFilter_In2DRange
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A76B0, length 72 bytes (+ int3 padding).
// Callees: none (x87 only).
// Layout: PDB CThingFilter_In2DRange { C3DVector Pos; float Range; } (16 bytes). Retail CThing holds PhysicsTC at +0x60;
// the component's position is the C3DVector at +0x0C.
// Shape: an absent component returns the byte constant (`xor al,al`); the float compare materialises an int
// (`mov eax,1` / `xor eax,eax`); Range is read into a local first (retail loads it before the deltas).
struct C3DVector {
    float x, y, z;
};

struct CTCPhysicsBase {
    void*     __vftable;
    unsigned char _pad04[8];
    C3DVector m_position;                           // +0x0c
};

struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
};

struct CThingFilter_In2DRange {
    C3DVector Pos;                                  // +0x00
    float     Range;                                // +0x0c
    bool operator()(CThing* thing) const;
};

bool CThingFilter_In2DRange::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (physics) {
        float range = Range;
        float dx = Pos.x - physics->m_position.x;
        float dy = Pos.y - physics->m_position.y;
        return dx * dx + dy * dy < range * range;
    }
    return false;
}
