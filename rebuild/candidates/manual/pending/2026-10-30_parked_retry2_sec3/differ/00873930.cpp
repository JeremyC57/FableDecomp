// Grade: DIFFER(231v221) -- float height read into a named local and the position bound to a const& before the sum: frame now 0x3c as retail (25->19 diff lines); residue: VC7.1 reads HeightOffsetZ after the GetPosition call and homes it in the dead argument slot (retail reads it first into [esp+38h]), the x/y sum still goes through a frame temporary
// CLookAtFrontInit::Alloc(CGameCameraManager* manager, const CDefPointer<const CCameraModeDef>& def) -- camera-mode factory: place a look-at-front camera ahead of the observed thing (facing * string length, raised by the height offset) looking at its graphic focal point
// Leaf: ?Alloc@CLookAtFrontInit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00873930, length 221 bytes (`ret 8`, `this` unused).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the manager's PObservedThing), the physics TC's virtual slot 70 (a C3DVector
// facing vector returned by hidden pointer), CThing::GetPosition @ 0x004C73D0, CTCGraphicAppearance's focal-point getter @ 0x004C10E0
// (C3DVector by hidden pointer; labelled CTCGraphicAppearance_UpdateHelperPointOffset), operator new @ 0x00BFEA1A (0xCC bytes),
// NCameraModes::CSimplePointBase::CSimplePointBase @ 0x00873A10 (labelled CLookAtFront::CLookAtFront).
// Layout: retail offsets below Ego_r.pdb: CGameCameraManager::PObservedThing +0x100 (PDB +0x110); CThing::PhysicsTC +0x60 and
// PTCGraphicAppearance +0x64 (PDB +0x70/+0x74); CCameraModeDef StringLengthXY +0x40 and HeightOffsetZ +0x48 (PDB - 4).
// NCameraModes::CLookAtFront (vtable 0x01277714) adds no members to the 0xCC-byte CSimplePointBase; its inline ctor stores the vtable
// after the base call.
// Shape: the height is read into a float local before the GetPosition call (an integer copy into the frame); `offset + position` with the
// offset on the left, then `.Z += height`.
struct CGameCameraManager;
struct CCameraModeDef {
    void*         __vftable;
    unsigned char _base[0x3c];
    float         StringLengthXY;               // +0x40
    float         CageRadius;                   // +0x44
    float         HeightOffsetZ;                // +0x48
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& operator=(const C3DVector& o) { X = o.X; Y = o.Y; Z = o.Z; return *this; }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CTCPhysicsBase {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual C3DVector GetFacingVector() const;  // slot 70 (+0x118)
};
struct CTCGraphicAppearance {
    C3DVector GetFocalPoint() const;            // 0x004C10E0
};
struct CThing {
    unsigned char         _pad[0x60];
    CTCPhysicsBase*       PhysicsTC;            // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance; // +0x64
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    void*        __vftable;
    CBaseObject* PItem;
    CBaseObject* GetPItem() const;              // 0x00A01B50
};
struct CGameCameraManager {
    unsigned char           _pad[0x100];
    CBaseIntelligentPointer PObservedThing;     // +0x100
};
namespace NCameraModes {
struct CBase {
    virtual void v00();
    unsigned char _body[0xa0];
    CBase(CGameCameraManager* manager, const CDefPointer<const CCameraModeDef>& def);   // 0x0084E420
};
struct CSimplePointBase : CBase {               // vtable 0x01277644
    C3DVector StartPos;                         // +0xa4
    C3DVector Position;                         // +0xb0
    C3DVector LookAtPos;                        // +0xbc
    bool      AllowLookAround;                  // +0xc8
    virtual void v00();
    CSimplePointBase(CGameCameraManager* manager, const CDefPointer<const CCameraModeDef>& def,
                     const C3DVector& position, const C3DVector& lookAt, bool allowLookAround);   // 0x00873A10
};
struct CLookAtFront : CSimplePointBase {        // vtable 0x01277714
    CLookAtFront(CGameCameraManager* manager, const CDefPointer<const CCameraModeDef>& def,
                 const C3DVector& position, const C3DVector& lookAt)
        : CSimplePointBase(manager, def, position, lookAt, true) { }
    virtual void v00();
};
}
struct CLookAtFrontInit {
    NCameraModes::CBase* Alloc(CGameCameraManager* manager, const CDefPointer<const CCameraModeDef>& def) const;
};

NCameraModes::CBase* CLookAtFrontInit::Alloc(CGameCameraManager* manager, const CDefPointer<const CCameraModeDef>& def) const
{
    CThing* thing = (CThing*)manager->PObservedThing.GetPItem();
    C3DVector facing = thing->PhysicsTC->GetFacingVector();
    C3DVector offset = facing * def->StringLengthXY;
    float height = def->HeightOffsetZ;
    const C3DVector& pos = thing->GetPosition();
    offset = offset + pos + C3DVector(0.0f, 0.0f, height);
    C3DVector focus = thing->PTCGraphicAppearance->GetFocalPoint();
    return new NCameraModes::CLookAtFront(manager, def, offset, focus);
}
