// Mass::ApplyImpulse(C2DVector point, C2DVector impulse) -- applies an impulse at a world point: the angular velocity gains (perp(point - Pos) . impulse) * AIMass, the linear velocity impulse * IMass
// Leaf: ApplyImpulse
// Grade: MATCH -- retry wave 2 (sec4): the perpendicular is taken on the (point - Pos) temporary; with the parked named
//   r local VC7.1 kept r.Y on the x87 stack (fld/fmul st(3) + an extra fstp st(0)) where retail consumes it (fxch st(2);
//   fmul [impulse.X])
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EEA80, 85 bytes, `ret 10h` (two by-value C2DVectors). Ego_r.pdb
// ?ApplyImpulse@Mass@@QAEXVC2DVector@@0@Z. No callees.
// Layout: Ego_r.pdb Mass (36 bytes, = retail): Pos +4, Vel +0x0C, IMass +0x14, AVel +0x1C, AIMass +0x20.
// Shape: perp(r) = (r.Y, -r.X) as in GetVel 0x008EEA30, taken on the (point - Pos) temporary (inline GetPerpendicular, named after C3DVector::GetPerpendicularXY; a named r local keeps r.Y alive on the x87 stack); the dot product, then the scaled impulse with its Y homed in the
// dead impulse.Y slot before the two component adds.
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
    void ApplyImpulse(C2DVector point, C2DVector impulse);
};

void Mass::ApplyImpulse(C2DVector point, C2DVector impulse)
{
    AVel += ((point - Pos).GetPerpendicular() * impulse) * AIMass;
    Vel += impulse * IMass;
}