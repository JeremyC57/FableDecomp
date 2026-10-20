// CTCCameraPointDefinitionBase::TransformPosition(const C3DVector& position, const C3DVector* origin, const CRightHandedSet* axes) const -- for a camera point using relative coordinates: express the position relative to the point's base in the given axes and move it to the origin; otherwise return it unchanged
// Grade: DIFFER(ours v retail) -- scheduling only: in the origin copy (offset = *origin) ours stores Y before loading Z, retail loads Y and Z then stores both
// Leaf: TransformPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075C420, length 205 bytes (+ int3 padding), `ret 10h` (hidden result + 3).
// Callees: CTCCameraPointDefinitionBase::TransformVector(const C3DVector&, const CRightHandedSet*) const @ 0x0075C280 (the
// orientation step; C3DVector by hidden pointer).
// Layout = Ego_r.pdb CTCCameraPointDefinitionBase (64 bytes): CoordBase +0x16 and CoordAxis +0x22 are unaligned in the PDB, so
// the class is packed; UsingRelativeCoords +0x3A, UsingRelativeOrientation +0x3B.
// Shape: the origin defaults to a zero vector (immediate float zeros) copied from the argument when given; the delta to the base
// is a local that the rotated vector is assigned back into (dword copy from the call's hidden result); the sum is built member by member into the result.
struct C3DVector {
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
struct CRightHandedSet;
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
