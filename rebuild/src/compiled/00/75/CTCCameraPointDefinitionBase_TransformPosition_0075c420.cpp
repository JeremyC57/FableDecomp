// CTCCameraPointDefinitionBase::TransformPosition(const C3DVector& position, const C3DVector* origin, const CRightHandedSet* axes) const -- for a camera point using relative coordinates: express the position relative to the point's base in the given axes and move it to the origin; otherwise return it unchanged
// Leaf: TransformPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075C420, length 205 bytes (+ int3 padding), `ret 10h` (hidden result + 3).
// Callees: CTCCameraPointDefinitionBase::TransformVector(const C3DVector&, const CRightHandedSet*) const @ 0x0075C280 (the
// orientation step; C3DVector by hidden pointer).
// Layout = Ego_r.pdb CTCCameraPointDefinitionBase (64 bytes): CoordBase +0x16 and CoordAxis +0x22 are unaligned in the PDB, so
// the class is packed; UsingRelativeCoords +0x3A, UsingRelativeOrientation +0x3B.
// Shape: the origin defaults to a zero vector (immediate float zeros) copied from the argument when given; the delta to the base
// is a local that the rotated vector is assigned back into (dword copy from the call's hidden result); the sum is built member by member into the result.
// Fix (retry 10): TransformVector (0x0075C280, same TU, C3DVector by hidden pointer) has a same-TU stand-in body; with only
// the declaration VC7.1 scheduled the result's word copies differently around the first fadd.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& b) const
    {
        return C3DVector(X - b.X, Y - b.Y, Z - b.Z);
    }
    C3DVector operator+(const C3DVector& b) const
    {
        return C3DVector(X + b.X, Y + b.Y, Z + b.Z);
    }
};
class CRightHandedSet;
#pragma pack(push, 1)
class CTCCameraPointDefinitionBase {
public:
    virtual void v00();
    void*   ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    bool    CutInto;                            // +0x0c
    bool    CutOutOf;                           // +0x0d
    bool    TestAngleBeforeActivation;          // +0x0e
    unsigned char _pad0f;
    float   FOV;                                // +0x10
    bool    SelfTerminate;                      // +0x14
    bool    HeroIsSubject;                      // +0x15
    C3DVector CoordBase;                        // +0x16
    unsigned char CoordAxis[0x3a - 0x22];       // +0x22 CRightHandedSet
    bool    UsingRelativeCoords;                // +0x3a
    bool    UsingRelativeOrientation;           // +0x3b
    bool    Activated;                          // +0x3c
    C3DVector TransformVector(const C3DVector& vector, const CRightHandedSet* axes) const;   // 0x0075C280
    C3DVector TransformPosition(const C3DVector& position, const C3DVector* origin, const CRightHandedSet* axes) const;
};
#pragma pack(pop)

float RotateComponent_s7(const void* axes, float x, float y, float z, long row);   // stand-in: one row of the rotation
#pragma auto_inline(off)
// Same-TU stand-in for TransformVector (0x0075C280): unchanged when the orientation is not relative, otherwise each
// component rotated by the axes (the row products done outside the graded bytes); reads the vector through its const
// reference and writes only the hidden result.
__declspec(noinline) C3DVector CTCCameraPointDefinitionBase::TransformVector(const C3DVector& vector, const CRightHandedSet* axes) const
{
    if (!UsingRelativeOrientation)
        return vector;
    float x = RotateComponent_s7(axes, vector.X, vector.Y, vector.Z, 0);
    float y = RotateComponent_s7(axes, vector.X, vector.Y, vector.Z, 1);
    float z = RotateComponent_s7(axes, vector.X, vector.Y, vector.Z, 2);
    return C3DVector(x, y, z);
}
#pragma auto_inline(on)
C3DVector CTCCameraPointDefinitionBase::TransformPosition(const C3DVector& position, const C3DVector* origin, const CRightHandedSet* axes) const
{
    if (!UsingRelativeCoords)
        return position;
    C3DVector offset(0.0f, 0.0f, 0.0f);
    if (origin)
        offset = *origin;
    C3DVector local = position - CoordBase;
    local = TransformVector(local, axes);
    return local + offset;
}