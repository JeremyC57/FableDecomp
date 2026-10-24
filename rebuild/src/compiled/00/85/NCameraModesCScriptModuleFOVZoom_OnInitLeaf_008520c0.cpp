// NCameraModes::CScriptModuleFOVZoom::OnInitLeaf() -- start the zoom: per-frame FOV step = (FOVFraction - 1) * the owner's FOV / frame count, and put the owning camera at the start position
// Retail: Fable.exe (TLC, VC7.1) @ 0x008520C0, 44 bytes, plain `ret`.
// Leaf: ?OnInitLeaf@CScriptModuleFOVZoom
// Name: vtable 0x01275A1C slot 8; FableWin.pdb ?OnInitLeaf@CScriptModuleFOVZoom@NCameraModes@@MAEXXZ.
// Layout: Ego_r.pdb CScriptModuleFOVZoom as the landed Update 0x00852160: WorldFrames +0x5c, FOVIncrement +0x60, FOVFraction +0x64,
//         StartPos +0x74; the owner (POwner +0x38) has GoTo.Position +0x54 and the FOV +0x84.
// Shape: `fld FOVFraction ; fsub 1.0 ; fmul FOV ; fidiv WorldFrames` (a long divisor), then the SetPosition copy; VC7.1 folds
//        the owner pointer to owner+0x54 once and reaches the FOV as [eax+30h], and walks `this` up to StartPos (+0x74).
struct CBaseObject;
struct CGameCameraManager;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CIntelligentPointer_CThingConst {             // CIntelligentPointer<const CThing>: vptr + link
    virtual void v00();
    void* m_pItem;
};
namespace NCameraModes {
struct CBase {                                       // the owning camera mode (retail offsets, landed CBase dtor 0x0056E0E0)
    virtual void v00();
    unsigned char _pad04[0x10 - 0x04];
    C3DVector     LookAt;                            // +0x10 (SetObservedPos)
    unsigned char _pad1c[0x54 - 0x1c];
    C3DVector     Position;                          // +0x54 GoTo.Position (SetPosition)
    unsigned char _pad60[0x84 - 0x60];
    float         FOV;                               // +0x84 (SetFOV)
    void SetPosition(const C3DVector& position) { Position = position; }
    void SetObservedPos(const C3DVector& position) { LookAt = position; }
    void SetFOV(float fov) { FOV = fov; }
    float GetFOV() const { return FOV; }
};
struct CScriptModuleBase {                           // vtable 0x01255F74, 0x4c bytes
    virtual ~CScriptModuleBase();                    // slot 0
    virtual void Update() = 0;                       // slot 1
    virtual bool IsFinished() const;                 // slot 2
    virtual bool IsShortenOnOcclusion() const;       // slot 3
    virtual bool IsStretchAroundHero() const;        // slot 4
    virtual bool GetModuleFOV(float& fov) const;     // slot 5
    virtual bool GetModuleRollAngle(float& angle) const;   // slot 6
    virtual bool ResetCameraOnEnd() const;           // slot 7
    virtual void OnInitLeaf();                       // slot 8
    CIntelligentPointer_CThingConst PCamPoint;       // +0x04
    CIntelligentPointer_CThingConst PThingOrigin;    // +0x0c
    C3DVector           CoordOrigin;                 // +0x14
    CRightHandedSet     CoordAxis;                   // +0x20
    CBase*              POwner;                      // +0x38
    CGameCameraManager* PCameraManager;              // +0x3c
    bool                SelfTerminate;               // +0x40
    bool                ShortenOnCollision;          // +0x41
    long                CutOutOverride;              // +0x44
    float               FOVOverride;                 // +0x48
};

struct CScriptModuleFOVZoom : CScriptModuleBase {   // vtable 0x01275A1C
    C3DVector     LookAtPos;                         // +0x4c
    long          EndFrame;                          // +0x58
    long          WorldFrames;                       // +0x5c
    float         FOVIncrement;                      // +0x60
    float         FOVFraction;                       // +0x64
    C3DVector     Increment;                         // +0x68
    C3DVector     StartPos;                          // +0x74
    bool          ShortenOnOcclusion;                // +0x80
    bool          ResetCamera;                       // +0x81
    virtual void OnInitLeaf();
};
void CScriptModuleFOVZoom::OnInitLeaf()
{
    FOVIncrement = (FOVFraction - 1.0f) * POwner->GetFOV() / WorldFrames;
    POwner->SetPosition(StartPos);
}
}