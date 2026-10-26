// NCameraModes::CScriptModuleThingLookAtUpperBody::Update() -- while the thing exists, put the owning camera in front of its upper body (camera focal position plus 1.25 x its facing) looking at that focal position
// Leaf: ?Update@CScriptModuleThingLookAtUpperBody
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852C40, length 199 bytes (`ret`).
// PDB: ?Update@CScriptModuleThingLookAtUpperBody@NCameraModes@@UAEXXZ. Label correction: the catalog calls this CScriptModuleThingLookAtFace like
// 0x00852B70; the PDB's only other ThingLookAt Update is CScriptModuleThingLookAtUpperBody, taken for the wider framing
// (1.25; the pairing is not proven by a caller -- neither body is referenced).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on Thing), the physics component's virtual slot 70
// GetFacing (+0x118, C3DVector by hidden pointer), CTCGraphicAppearance::GetCameraFocalPos(float) @ 0x004C12D0 (C3DVector by
// hidden pointer). Float pool 0x01265BB8 = 1.25f.
// Layout: the CScriptModuleBase (0x4c bytes, POwner +0x38: camera LookAt +0x10, Position +0x54) plus Thing
// (CIntelligentPointer<CThing>) at +0x4c; retail CThing keeps PhysicsTC at +0x60 and PTCGraphicAppearance at +0x64.
// Shape: the facing and the focal position are named locals filled by the by-value getters (the components read through
// inline accessors straight into ecx); the position is `focal + facing * 1.25f` through inline C3DVector operators that
// RETURN A CONSTRUCTED TEMPORARY (`return C3DVector(X * s, ...)`): VC keeps the scaled x/y on the x87 stack (z spilled)
// and builds the sum in the frame; operators filling a named `result` local copy it through memory instead. The sum
// goes to the owner by integer moves, then the look-at copy.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
    C3DVector operator+(const C3DVector& other) const { return C3DVector(X + other.X, Y + other.Y, Z + other.Z); }
};

class CTCPhysicsBase {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetFacing();                   // slot 70 (+0x118), by value through the hidden pointer
};
class CTCGraphicAppearance {
public:
    C3DVector GetCameraFocalPos(float height);       // 0x004C12D0
};
class CThing {
public:
    unsigned char         _pad00[0x60];
    CTCPhysicsBase*       PhysicsTC;                 // +0x60 (retail)
    CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64 (retail)
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    CTCGraphicAppearance* GetPTCGraphicAppearance() const { return PTCGraphicAppearance; }
};
struct CIntelligentPointerThing {                    // CIntelligentPointer<CThing>
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem();                              // 0x00A01B50 CBaseIntelligentPointer::GetPItem
};
struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                          // +0x54
};

namespace NCameraModes {

class CScriptModuleThingLookAtUpperBody {
public:
    void*                    __vftable;              // +0x00
    unsigned char            _pad04[0x34];
    CCameraState*            POwner;                 // +0x38
    unsigned char            _pad3c[0x10];
    CIntelligentPointerThing Thing;                  // +0x4c
    void Update();
};

void CScriptModuleThingLookAtUpperBody::Update()
{
    if (Thing.GetPItem()) {
        C3DVector facing = Thing.GetPItem()->GetPhysicsTC()->GetFacing();
        C3DVector focal = Thing.GetPItem()->GetPTCGraphicAppearance()->GetCameraFocalPos(1.0f);
        POwner->Position = focal + facing * 1.25f;
        POwner->LookAt = focal;
    }
}

}