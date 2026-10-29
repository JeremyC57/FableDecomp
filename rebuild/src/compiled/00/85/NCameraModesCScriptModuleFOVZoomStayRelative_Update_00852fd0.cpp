// NCameraModes::CScriptModuleFOVZoomStayRelative::Update() -- keep the camera at a fixed offset from the thing's facing (rotated about Axis by Angle, scaled by Mag) around LookAtPos, gliding there until EndFrame, then run the FOV zoom step
// Leaf: ?Update@CScriptModuleFOVZoomStayRelative
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852FD0, length 211 bytes (`ret`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (Thing), the physics component's virtual slot 70 GetFacing
// (+0x118, C3DVector by hidden pointer), C3DVector::Rotate(const C3DVector&, float) @ 0x00A13C60, CWorld::GetFrame
// @ 0x0049D870 (`__fastcall`, twice), CScriptModuleFOVZoom::CalculateIncrementVector(const C3DVector&, const C3DVector&,
// long) @ 0x008520F0 and CScriptModuleFOVZoom::Update @ 0x00852160 (both landed, called non-virtually on `this`).
// Layout: Ego_r.pdb NCameraModes::CScriptModuleFOVZoomStayRelative (0xa0 bytes) over CScriptModuleFOVZoom (0x84):
// LookAtPos +0x4c, EndFrame +0x58; Thing +0x84 (CIntelligentPointer<CThing>), Axis +0x8c, Angle +0x98, Mag +0x9c;
// POwner +0x38 (camera Position +0x54); retail CThing keeps PhysicsTC at +0x60.
// Shape: the facing is a named local filled by the virtual getter, rotated in place, then scaled and offset through
// inline C3DVector `*=` / `+=` (the scaled X forwarded on the x87 stack into its add); before EndFrame the glide is set
// up to the new position over the remaining frames (the owner taken into a local first, so it rides in edi across
// the second frame call), otherwise the position is copied; the base Update is ONE statement after the if/else (VC
// tail-duplicates it, popping edi first), the physics component is read through an inline accessor straight into ecx.
class CWorld { public: static long __fastcall GetFrame(); };                 // 0x0049D870

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    void Rotate(const C3DVector& axis, float angle); // 0x00A13C60
    C3DVector& operator*=(float scale) { X *= scale; Y *= scale; Z *= scale; return *this; }
    C3DVector& operator+=(const C3DVector& other) { X += other.X; Y += other.Y; Z += other.Z; return *this; }
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
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                       // +0x60 (retail)
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};
struct CIntelligentPointerThing {                    // CIntelligentPointer<CThing>
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem();                              // 0x00A01B50 CBaseIntelligentPointer::GetPItem
};

struct CCameraState {
    unsigned char _pad00[0x54];
    C3DVector     Position;                          // +0x54
};

namespace NCameraModes {

class CScriptModuleFOVZoom {
public:
    void*         __vftable;                         // +0x00
    unsigned char _pad04[0x34];
    CCameraState* POwner;                            // +0x38
    unsigned char _pad3c[0x10];
    C3DVector     LookAtPos;                         // +0x4c
    long          EndFrame;                          // +0x58
    unsigned char _pad5c[0x28];
    void Update();                                   protected: // 0x00852160
    void CalculateIncrementVector(const C3DVector& from, const C3DVector& to, long frames); public:   // 0x008520F0
};

class CScriptModuleFOVZoomStayRelative : public CScriptModuleFOVZoom {
public:
    CIntelligentPointerThing Thing;                  // +0x84
    C3DVector                Axis;                   // +0x8c
    float                    Angle;                  // +0x98
    float                    Mag;                    // +0x9c
    void Update();
};

void CScriptModuleFOVZoomStayRelative::Update()
{
    C3DVector position = Thing.GetPItem()->GetPhysicsTC()->GetFacing();
    position.Rotate(Axis, Angle);
    position *= Mag;
    position += LookAtPos;
    if (CWorld::GetFrame() < EndFrame) {
        CCameraState* owner = POwner;
        CalculateIncrementVector(owner->Position, position, EndFrame - CWorld::GetFrame());
    }
    else
        POwner->Position = position;
    CScriptModuleFOVZoom::Update();
}

}