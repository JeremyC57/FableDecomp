// Grade: DIFFER(ours v retail) -- 144v146: x87 schedule only: retail forms BOTH point-Pos lever arms (second one spilled to [esp+4]/point.Y) before the dot product and multiplies force.X from memory (fxch st(2); fmul [force.X]; faddp st(2)); ours takes the dot right after the first arm (fld [force.X]; fmul st(3); faddp st(1)). Same residue family as 008eea80 ApplyImpulse.
// Mass::ForceToVel(C2DVector point, C2DVector force) -- velocity change a force at a world point produces: force * IMass plus perp(point - Pos) * ((perp(point - Pos) . force) * AIMass)
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
    C2DVector r = point - Pos;
    float torque = C2DVector(r.Y, -r.X) * force;
    C2DVector arm = point - Pos;
    return force * IMass + C2DVector(arm.Y, -arm.X) * torque * AIMass;
}
