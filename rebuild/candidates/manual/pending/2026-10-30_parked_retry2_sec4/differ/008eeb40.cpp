// Mass::ForceToVel(C2DVector point, C2DVector force) -- velocity change a force at a world point produces: force * IMass plus perp(point - Pos) * ((perp(point - Pos) . force) * AIMass)
// Grade: DIFFER(140v146) -- retry wave 2 (sec4): with the perpendiculars taken on (point - Pos) temporaries (the 0x008EEA80 lever) the dot product is now retail's (fxch st(2); fmul [force.X]; faddp st(2)); left: retail forms the second lever arm (spilling arm.X to the dead point.Y slot and arm.Y to [esp+4]) BEFORE the dot product, ours after. Tried: arm/r named locals in both orders, one-expression form. Next: an inline helper that returns the arm and takes the torque, or the GetVel 0x008EEA30 shape.
// Leaf: ForceToVel
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EEB40, 146 bytes (+ int3 padding), `ret 14h` (hidden result pointer + two by-value
// C2DVectors). Ego_r.pdb ?ForceToVel@Mass@@QAE?AVC2DVector@@V2@0@Z. No callees.
// Layout: Ego_r.pdb Mass (36 bytes, = retail): Pos +4, IMass +0x14, AIMass +0x20.
// Shape: point - Pos is evaluated twice (once for the torque, once for the lever arm); AIMass is copied into the dead
// point.X slot (the scale parameter of an inlined operator*); the angular term is computed before force * IMass.
class C2DVector {
public:
    float X;                                       // +0x00
    float Y;                                       // +0x04
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
    C2DVector operator+(const C2DVector& other) const { return C2DVector(X + other.X, Y + other.Y); }
    C2DVector operator-(const C2DVector& other) const { return C2DVector(X - other.X, Y - other.Y); }
    C2DVector operator*(float scale) const { return C2DVector(X * scale, Y * scale); }
    float operator*(const C2DVector& other) const { return X * other.X + Y * other.Y; }
    C2DVector& operator+=(const C2DVector& other) { X += other.X; Y += other.Y; return *this; }
    C2DVector GetPerpendicular() const { return C2DVector(Y, -X); }
};

class Mass {                                       // vtable 0x01288230, 0x24 bytes (landed Mass::Mass 0x008EE9B0)
public:
    C2DVector Pos;                                 // +0x04
    C2DVector Vel;                                 // +0x0C
    float     IMass;                               // +0x14
    float     APos;                                // +0x18
    float     AVel;                                // +0x1C
    float     AIMass;                              // +0x20
    virtual ~Mass();
    C2DVector ForceToVel(C2DVector point, C2DVector force);
};

C2DVector Mass::ForceToVel(C2DVector point, C2DVector force)
{
    float torque = (point - Pos).GetPerpendicular() * force;
    return force * IMass + (point - Pos).GetPerpendicular() * torque * AIMass;
}
