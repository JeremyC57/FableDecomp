// CThingPhysical::IsInRange(const C3DVector&, float) -- nonzero when the thing's physics position lies strictly inside `range` of `pos`
// Retail: Fable.exe 0x006A4DF0, 79 bytes, no callees.
// PhysicsTC is CThing +0x60 in retail (PDB +0x70: the retail CThing is 0x10 narrower from +0x48 on, so
// the generated CThingPhysical.h is not used); the TC's position is a C3DVector at +0x0c.
// Shape: the difference is an inline C3DVector temporary held on the x87 stack (popped by three
// trailing `fstp st(0)`), its squared length compared with range*range; a long (`mov eax,1`) result.
struct C3DVector {
    float x, y, z;
    C3DVector() {}
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(x - o.x, y - o.y, z - o.z); }
    float GetLengthSquared() const { return x * x + y * y + z * z; }
};

struct CTCPhysicsBase {
    void*         __vftable;
    unsigned char _pad04[0x8];
    C3DVector     Position;                              // +0x0c
};

struct CThingPhysical {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    long IsInRange(const C3DVector& pos, float range);
};

long CThingPhysical::IsInRange(const C3DVector& pos, float range)
{
    C3DVector d = PhysicsTC->Position - pos;
    return d.GetLengthSquared() < range * range;
}