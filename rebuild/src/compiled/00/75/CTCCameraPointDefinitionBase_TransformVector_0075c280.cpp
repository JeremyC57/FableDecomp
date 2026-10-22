// CTCCameraPointDefinitionBase::TransformVector(const C3DVector& v, const CRightHandedSet* axes) -- with relative orientation on, turn the vector about world Z by the flat angle from the given axes' forward (else the point's own) to the point's own forward; otherwise return it unchanged
// Leaf: ?TransformVector@CTCCameraPointDefinitionBase@@IBE?AVC3DVector@@ABV2@PBVCRightHandedSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075C280, 410 bytes, `ret 0Ch`.
// Callees: C3DVector::Normalise 0x00A14440 (x2; catalog "GetScaled" is propagated), _CIacos 0x00BFEBA2 (intrinsic acos),
//   C3DVector::Rotate(const C3DVector&, float) 0x00A13C60; constants -1.0 0x0122DEE0, 1.0 0x0122DED8, 0.0 0x0122DEDC,
//   1/(2*pi) 0x01237F08.
// Layout: CTCCameraPointDefinitionBase is byte-packed (Ego_r.pdb): CoordAxis CRightHandedSet +0x22 (Forward +0x2e),
//   UsingRelativeOrientation +0x3b.
// Shape: the axes are a CRightHandedSet copy (scalarised from CoordAxis: Forward.X through a slot, Forward.Y in st0, the
//   "dead default load"; the copy from the argument is a full 6-dword copy); the first flat vector is a copy of the set's
//   forward with Z cleared, the second the (x, y, 0) constructor; the dot product is written Z + X + Y (VC7.1 emits it as
//   Y, X, add, Z, add); the sign test is the Z of from x to.
extern "C" double __cdecl acos(double);
#pragma intrinsic(acos)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void Normalise();                                        // 0x00A14440
    void Rotate(const C3DVector& axis, float angle);         // 0x00A13C60
};
struct CRightHandedSet {
    C3DVector Up;                                            // +0x00
    C3DVector Forward;                                       // +0x0c
};

#pragma pack(push, 1)
class CTCCameraPointDefinitionBase {
public:
    void*           __vftable;                               // +0x00
    unsigned char   _pad04[0x0c - 0x04];
    bool            CutInto;                                 // +0x0c
    bool            CutOutOf;                                // +0x0d
    bool            TestAngleBeforeActivation;               // +0x0e
    unsigned char   _pad0f;
    float           FOV;                                     // +0x10
    bool            SelfTerminate;                           // +0x14
    bool            HeroIsSubject;                           // +0x15
    C3DVector       CoordBase;                               // +0x16
    CRightHandedSet CoordAxis;                               // +0x22
    bool            UsingRelativeCoords;                     // +0x3a
    bool            UsingRelativeOrientation;                // +0x3b
    bool            Activated;                               // +0x3c
    C3DVector TransformVector(const C3DVector& v, const CRightHandedSet* axes) const;
};
#pragma pack(pop)

C3DVector CTCCameraPointDefinitionBase::TransformVector(const C3DVector& v, const CRightHandedSet* axes) const
{
    if (!UsingRelativeOrientation)
        return v;
    C3DVector result = v;
    CRightHandedSet set = CoordAxis;
    if (axes)
        set = *axes;
    C3DVector from = set.Forward;
    from.Z = 0.0f;
    from.Normalise();
    C3DVector to(CoordAxis.Forward.X, CoordAxis.Forward.Y, 0.0f);
    to.Normalise();
    float dot = from.Z * to.Z + from.X * to.X + from.Y * to.Y;
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    float angle = (float)acos(dot) * 0.15915493667125702f;
    float sign = 1.0f;
    if (from.X * to.Y - from.Y * to.X < 0.0f)
        sign = -1.0f;
    result.Rotate(C3DVector(0.0f, 0.0f, 1.0f), sign * angle);
    return result;
}