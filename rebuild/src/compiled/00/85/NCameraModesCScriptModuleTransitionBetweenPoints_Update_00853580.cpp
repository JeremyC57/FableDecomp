// NCameraModes::CScriptModuleTransitionBetweenPoints::Update() -- until EndFrame step the camera position by Increment and track the thing's focal position (or drift the look-at by LookAtIncrement), then aim the camera and apply the FOV
// Leaf: ?Update@CScriptModuleTransitionBetweenPoints
// Grade: MATCH (retry 9) -- the LookAtPos drift is written per component in the function body
// (`LookAtPos.X = LookAtIncrement.X + LookAtPos.X;` ...): retail loads the increment first (`fld LookAtIncrement ; fadd
// LookAtPos ; fstp LookAtPos`). Through an inline C3DVector operator+= (`X += o.X`, `X = X + o.X`, `X = o.X + X`) VC7.1
// ranks the `this` member above the parameter and loads LookAtPos first whatever the spelling; in the body both are
// members of the same object and either spelling gives retail's order. Focal position as a named local.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00853580, length 233 bytes (`ret`).
// PDB: ?Update@CScriptModuleTransitionBetweenPoints@NCameraModes@@UAEXXZ (vtable 0x012759F4 slot 1).
// Callees: GFGetCurrentFrame @ 0x0049D870 (`__fastcall`), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on
// PThing), CTCGraphicAppearance::GetCameraFocalPos(float) @ 0x004C12D0 (C3DVector by hidden pointer).
// Layout: Ego_r.pdb NCameraModes::CScriptModuleTransitionBetweenPoints (0x90 bytes): PThing +0x4c, Increment +0x54,
// LookAtIncrement +0x60, LookAtPos +0x6c, StartPos +0x78, EndFrame +0x84, FOV +0x88, PreventSelfTermination +0x8c, over
// the 0x4c-byte CScriptModuleBase (POwner +0x38: camera LookAt +0x10, Position +0x54, FOV +0x84); retail CThing keeps
// PTCGraphicAppearance at +0x64 and the dead bit in byte +0x91.
// Shape: `Position = Position + Increment` through the landed member operator+ (right operand first, the sum in a frame
// temporary copied by integer moves); the alive test is the GetPItem null test plus the raw dead bit; the focal position
// lands in a named local (same frame slot) copied to LookAtPos (read back from the frame; the bare assignment copies through
// the returned pointer and loads the component into eax); the drift is an inline `+=` straight into the member.
long __fastcall GFGetCurrentFrame();                 // 0x0049D870

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector operator+(const C3DVector& other) const
    {
        C3DVector result;
        result.X = X + other.X;
        result.Y = Y + other.Y;
        result.Z = Z + other.Z;
        return result;
    }
    C3DVector& operator+=(const C3DVector& other) { X += other.X; Y += other.Y; Z += other.Z; return *this; }
};

class CTCGraphicAppearance {
public:
    C3DVector GetCameraFocalPos(float height);       // 0x004C12D0
};
class CThing {
public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64 (retail)
    unsigned char         _pad68[0x29];
    unsigned char         m_dead : 1;                // +0x91 bit 0
    unsigned char         m_91_1 : 7;
    CTCGraphicAppearance* GetPTCGraphicAppearance() const { return PTCGraphicAppearance; }
};
struct CIntelligentPointerThing {                    // CIntelligentPointer<const CThing>
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem();                              // 0x00A01B50 CBaseIntelligentPointer::GetPItem
};
struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                          // +0x54
    unsigned char _pad60[0x24];
    float         FOV;                               // +0x84
};

namespace NCameraModes {

class CScriptModuleTransitionBetweenPoints {
public:
    void*                    __vftable;              // +0x00
    unsigned char            _pad04[0x34];
    CCameraState*            POwner;                 // +0x38
    unsigned char            _pad3c[0x10];
    CIntelligentPointerThing PThing;                 // +0x4c
    C3DVector                Increment;              // +0x54
    C3DVector                LookAtIncrement;        // +0x60
    C3DVector                LookAtPos;              // +0x6c
    C3DVector                StartPos;               // +0x78
    long                     EndFrame;               // +0x84
    float                    FOV;                    // +0x88
    bool                     PreventSelfTermination; // +0x8c
    void Update();
};

void CScriptModuleTransitionBetweenPoints::Update()
{
    if (GFGetCurrentFrame() < EndFrame) {
        POwner->Position = POwner->Position + Increment;
        if (PThing.GetPItem() && !PThing.GetPItem()->m_dead) {
            C3DVector focal = PThing.GetPItem()->GetPTCGraphicAppearance()->GetCameraFocalPos(1.0f);
            LookAtPos = focal;
        }
        else {
            LookAtPos.X = LookAtIncrement.X + LookAtPos.X;
            LookAtPos.Y = LookAtIncrement.Y + LookAtPos.Y;
            LookAtPos.Z = LookAtIncrement.Z + LookAtPos.Z;
        }
    }
    POwner->LookAt = LookAtPos;
    POwner->FOV = FOV;
}

}