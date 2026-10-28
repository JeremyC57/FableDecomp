// CTCCameraPointDefinitionBase::OnSerialise(CPersistContext& context, bool transfer) -- persist the camera point's flags, FOV, base position (relative to the parent thing) and axes, then the leaf class's own data
//   `mov eax,ebp` for Up then `add ebp,0xc` for Forward) while the reads stay folded on this; C3DVector has a user copy ctor.
// Leaf: OnSerialise
//
// Retail 0x0075C670, 714 bytes (`ret 8`).
// Callees: CThing::GetPos @ 0x004C73D0, CPersistContext::Transfer<bool> @ 0x004045C0, Transfer<float> @ 0x00410620
// ("FOV", default 72), Transfer<C3DVector> @ 0x004859AD (manifest label Transfer<CFloatRange> is a propagated name);
// virtual OnSerialseLeaf (vtable +0x84, slot 33).
// Layout = Ego_r.pdb CTCCameraPointDefinitionBase (64 bytes, packed: CoordBase +0x16, CoordAxis +0x22 unaligned), as the
// landed TransformPosition (0x0075C420): CutInto +0x0c, CutOutOf +0x0d, TestAngleBeforeActivation +0x0e, FOV +0x10,
// SelfTerminate +0x14, HeroIsSubject +0x15, UsingRelativeCoords +0x3a, UsingRelativeOrientation +0x3b.
// Shape: CoordBase is saved relative to the parent's position under an "IsCoordBaseRelativeToParent" flag (always written
// true) and rebuilt on load; the axes are transferred through a local copy (Up default +Z, Forward default +X) and copied
// back.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& b) const { return C3DVector(X - b.X, Y - b.Y, Z - b.Z); }
    C3DVector operator+(const C3DVector& b) const { return C3DVector(X + b.X, Y + b.Y, Z + b.Z); }
};
class CRightHandedSet { public:
    CRightHandedSet() {}
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) { Up = up; Forward = forward; }
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
class CThing { public:
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CPersistContext { public:
    unsigned char _pad00[0x18];
    long Mode;                                  // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, bool& value, const bool& defaultValue);             // 0x004045C0
    void Transfer(const char* name, float& value, const float& defaultValue);           // 0x00410620
    void Transfer(const char* name, C3DVector& value, const C3DVector& defaultValue);   // 0x004859AD
};
#pragma pack(push, 1)
class CTCCameraPointDefinitionBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void OnSerialseLeaf(CPersistContext& context);   // slot 33 (+0x84)
    CThing* ParentThing;                        // +0x04
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
    CRightHandedSet CoordAxis;                  // +0x22
    bool    UsingRelativeCoords;                // +0x3a
    bool    UsingRelativeOrientation;           // +0x3b
    bool    Activated;                          // +0x3c
    void OnSerialise(CPersistContext& context, bool transfer);
};
#pragma pack(pop)

void CTCCameraPointDefinitionBase::OnSerialise(CPersistContext& context, bool transfer)
{
    C3DVector parentPos = ParentThing->GetPos();
    context.Transfer("CutInto", CutInto, false);
    context.Transfer("CutOutOf", CutOutOf, false);
    context.Transfer("TestAngleBeforeActivation", TestAngleBeforeActivation, false);
    context.Transfer("SelfTerminate", SelfTerminate, true);
    context.Transfer("HeroIsSubject", HeroIsSubject, false);
    context.Transfer("FOV", FOV, 72.0f);
    bool relative = true;
    context.Transfer("IsCoordBaseRelativeToParent", relative, false);
    if (context.IsSaving()) {
        C3DVector base = CoordBase - parentPos;
        context.Transfer("CoordBase", base, C3DVector(0.0f, 0.0f, 0.0f));
    } else {
        C3DVector base;
        context.Transfer("CoordBase", base, C3DVector(0.0f, 0.0f, 0.0f));
        if (relative)
            CoordBase = base + parentPos;
        else
            CoordBase = base;
    }
    C3DVector up = CoordAxis.Up;
    CRightHandedSet* axis = &CoordAxis;
    context.Transfer("CoordAxisUp", up, C3DVector(0.0f, 0.0f, 1.0f));
    C3DVector forward = CoordAxis.Forward;
    context.Transfer("CoordAxisFwd", forward, C3DVector(1.0f, 0.0f, 0.0f));
    axis->Up = up;
    axis->Forward = forward;
    context.Transfer("UsingRelativeCoords", UsingRelativeCoords, false);
    context.Transfer("UsingRelativeOrientation", UsingRelativeOrientation, false);
    OnSerialseLeaf(context);
}