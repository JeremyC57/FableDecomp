// CTCCameraPointDefinitionBase::CTCCameraPointDefinitionBase(CThing*) -- CTCBase(owner), vtable, default camera-point settings (FOV 72, identity axes)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075C180, length 91 bytes (catalogued as the propagated label
// "CTCOpinionSource::CTCOpinionSource"; the vtable it stores, 0x01269B04, is CTCCameraPointDefinitionBase's).
// Leaf: ??0CTCCameraPointDefinitionBase
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800.
// PDB CTCCameraPointDefinitionBase (0x40 bytes, packed): CutInto +0x0c, CutOutOf +0x0d, TestAngleBeforeActivation +0x0e,
// FOV +0x10, SelfTerminate +0x14, HeroIsSubject +0x15, CoordBase (C3DVector) +0x16, CoordAxis (CRightHandedSet: Up, Forward)
// +0x22, UsingRelativeCoords +0x3a, UsingRelativeOrientation +0x3b, Activated +0x3c.
// Shape: one zero register for the bytes and the zero floats, 1.0f held in ecx for Up.Z and Forward.X; the four bytes
// before SelfTerminate are scheduled before the vtable store. FOV (72.0f) is stored last as an immediate: a BODY
// assignment (in the initialiser list it is stored right after the vtable).
#pragma pack(push, 1)
struct CThing;

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet() : Up(0.0f, 0.0f, 1.0f), Forward(1.0f, 0.0f, 0.0f) {}
};

struct CTCBase_0075c180 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
    CTCBase_0075c180(CThing* owner);                // 0x00686800
};

struct CTCCameraPointDefinitionBase : CTCBase_0075c180 {
    bool            CutInto;                        // +0x0c
    bool            CutOutOf;                       // +0x0d
    bool            TestAngleBeforeActivation;      // +0x0e
    unsigned char   _pad0f;
    float           FOV;                            // +0x10
    bool            SelfTerminate;                  // +0x14
    bool            HeroIsSubject;                  // +0x15
    C3DVector       CoordBase;                      // +0x16
    CRightHandedSet CoordAxis;                      // +0x22
    bool            UsingRelativeCoords;            // +0x3a
    bool            UsingRelativeOrientation;       // +0x3b
    bool            Activated;                      // +0x3c
    unsigned char   _pad3d[3];
    CTCCameraPointDefinitionBase(CThing* owner);
    virtual void v00();
};
#pragma pack(pop)

CTCCameraPointDefinitionBase::CTCCameraPointDefinitionBase(CThing* owner)
    : CTCBase_0075c180(owner), CutInto(false), CutOutOf(false), TestAngleBeforeActivation(false),
      SelfTerminate(true), HeroIsSubject(false), CoordBase(0.0f, 0.0f, 0.0f),
      UsingRelativeCoords(false), UsingRelativeOrientation(false), Activated(false)
{
    FOV = 72.0f;
}
