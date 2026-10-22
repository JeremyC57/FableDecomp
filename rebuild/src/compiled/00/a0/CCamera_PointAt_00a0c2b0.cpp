// CCamera::PointAt(const C3DVector& target) -- aims the camera: Forward = target - WorldPosition (Z flattened to 0 when ViewVecZLocked), then Up = world Z minus its projection on Forward, both normalised
// Leaf: PointAt
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0C2B0, 208 bytes incl. padding (ret 4).
// Callees: C3DVector::Normalise @ 0x00A14440 (manifest "GetScaled" is propagated), four times through the inline
//          CRightHandedSet::Normalise (Up then Forward). Float pool: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Layout (Ego_r.pdb CCamera, 56 bytes): WorldPosition +0, RHSet +0xC {Up +0xC, Forward +0x18}, HeightLocked +0x24,
//          ViewVecZLocked +0x25.
// Shape: the flag is an if/else whose arms both assign through a Forward reference taken first (lea esi before the
//          x87 work): locked builds C3DVector(dx, dy, 0), unlocked uses target - WorldPosition; VC7.1 merges the common X/Y
//          head above the branch. C3DVector has a user ctor + user copy ctor and no operator=, so each assignment builds a frame temporary and
//          copies it through a pointer register; Dot with (0,0,1) keeps the 0.0f products (x87, no fast-math).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void Normalise();                           // 0x00A14440
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline C3DVector operator*(const C3DVector& a, float s) { return C3DVector(a.X * s, a.Y * s, a.Z * s); }
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

struct CRightHandedSet {
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0C
    void Normalise() { Up.Normalise(); Forward.Normalise(); }
};

struct CCamera {
    C3DVector       WorldPosition;              // +0x00
    CRightHandedSet RHSet;                      // +0x0C
    bool            HeightLocked;               // +0x24
    bool            ViewVecZLocked;             // +0x25
    unsigned long   FOVFlags;                   // +0x28
    float           HorizontalFOV;              // +0x2C
    float           VerticalFOV;                // +0x30
    float           Zoom;                       // +0x34
    void PointAt(const C3DVector& target);
};

void CCamera::PointAt(const C3DVector& target)
{
    C3DVector& forward = RHSet.Forward;
    if (ViewVecZLocked)
        forward = C3DVector(target.X - WorldPosition.X, target.Y - WorldPosition.Y, 0.0f);
    else
        forward = target - WorldPosition;
    RHSet.Normalise();
    C3DVector worldUp(0.0f, 0.0f, 1.0f);
    RHSet.Up = worldUp - RHSet.Forward * Dot(RHSet.Forward, worldUp);
    RHSet.Normalise();
}