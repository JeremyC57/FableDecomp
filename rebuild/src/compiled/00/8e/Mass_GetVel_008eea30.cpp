// Mass::GetVel(C2DVector point) const -- velocity of the rigid 2D body at a world point: the linear velocity plus the angular velocity times the perpendicular of (point - Pos)
// Leaf: GetVel
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EEA30, 67 bytes (+ int3 padding), `ret 0Ch` (hidden result pointer + the by-value
// C2DVector). Ego_r.pdb ?GetVel@Mass@@QBE?AVC2DVector@@V2@@Z. No callees.
// Layout: Ego_r.pdb Mass (36 bytes, = retail, landed Mass::Mass 0x008EE9B0): vtable +0, Pos +4, Vel +0x0C, IMass +0x14,
// APos +0x18, AVel +0x1C, AIMass +0x20.
// Shape: retail result X = Vel.X + r.Y * AVel, Y = Vel.Y - r.X * AVel (r = point - Pos), with AVel copied through eax into the
// dead point.X slot (the inlined operator* scale parameter) and -r.X homed in an 8-byte frame temporary.
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
    C2DVector GetVel(C2DVector point) const;
};

C2DVector Mass::GetVel(C2DVector point) const
{
    C2DVector r = point - Pos;
    return Vel + C2DVector(r.Y, -r.X) * AVel;
}