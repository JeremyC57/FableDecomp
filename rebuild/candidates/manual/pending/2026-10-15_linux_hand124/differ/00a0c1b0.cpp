// CCamera::MoveSideways(float distance) -- strafe the camera along its right axis (Forward x Up); with HeightLocked only X and Y move, otherwise the whole scaled right vector is added to the world position.
// Grade: DIFFER(158v152) -- x87 operand load order of the cross-product products in the free arm is reversed vs retail (every product but Forward.Z*Up.Y), so the common first product Forward.Y*Up.Z is not hoisted above the HeightLocked test; locked arm and the add/scale tail match
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0C1B0, length 152 bytes. No callees.
// Layout (Ego_r.pdb CCamera): WorldPosition +0x00, RHSet.Up +0x0c, RHSet.Forward +0x18, HeightLocked +0x24.
// Shape: the sibling of the landed CCamera::MoveForward (0x00A0C250) with the right vector of the landed
// CRightHandedSet::GetRight (0x0049CE00, the inline CrossProduct(Forward, Up)) in place of Forward. The first
// product (Up.Z * Forward.Y) is common to both arms and hoisted above the HeightLocked test. The locked arm uses
// only the X and Y components of the inlined right vector (Z is dead); the free arm builds GetRight() and then
// `right * distance` in the 0x18-byte frame (two C3DVector temporaries) and adds it with the inline operator+=.
struct C3DVector {
    float X, Y, Z;                            // +0x00 / +0x04 / +0x08
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};

inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y,
                     a.Z * b.X - a.X * b.Z,
                     a.X * b.Y - a.Y * b.X);
}

struct CRightHandedSet {
    C3DVector Up;                             // +0x00
    C3DVector Forward;                        // +0x0c
    C3DVector GetRight() const { return CrossProduct(Forward, Up); }
};

class CCamera {
public:
    C3DVector       WorldPosition;            // +0x00
    CRightHandedSet RHSet;                    // +0x0c
    bool            HeightLocked;             // +0x24
    bool            ViewVecZLocked;           // +0x25
    unsigned long   FOVFlags;                 // +0x28
    float           HorizontalFOV;            // +0x2c
    float           VerticalFOV;              // +0x30
    float           Zoom;                     // +0x34
    void MoveSideways(float distance);
};

void CCamera::MoveSideways(float distance)
{
    if (HeightLocked) {
        WorldPosition.X += RHSet.GetRight().X * distance;
        WorldPosition.Y += RHSet.GetRight().Y * distance;
    } else {
        WorldPosition += RHSet.GetRight() * distance;
    }
}
