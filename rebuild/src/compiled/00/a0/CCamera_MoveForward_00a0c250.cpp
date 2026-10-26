// CCamera::MoveForward(float distance) -- moves the world position along the forward axis; with HeightLocked
// only X and Y move, otherwise the scaled forward vector (an inline C3DVector temporary) is added.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0C250, length 86 bytes.
// Callees: none.
// PDB CCamera: +0x00 WorldPosition, +0x18 RHSet.Forward, +0x24 HeightLocked. The locked branch adds the two
// products straight into the members; the free branch builds `Forward * distance` in a 12-byte stack
// temporary (only its Z lands in memory, `fstp [esp+8]`) and adds it with the inline operator+=.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};

class CCamera { public:
    C3DVector       WorldPosition;               // +0x00
    C3DVector       Up;                          // +0x0C
    C3DVector       Forward;                     // +0x18
    bool            HeightLocked;                // +0x24
    bool            ViewVecZLocked;              // +0x25
    unsigned long   FOVFlags;                    // +0x28
    float           HorizontalFOV;               // +0x2C
    float           VerticalFOV;                 // +0x30
    float           Zoom;                        // +0x34
    void MoveForward(float distance);
};

void CCamera::MoveForward(float distance)
{
    if (HeightLocked) {
        WorldPosition.X += Forward.X * distance;
        WorldPosition.Y += Forward.Y * distance;
    } else {
        WorldPosition += Forward * distance;
    }
}