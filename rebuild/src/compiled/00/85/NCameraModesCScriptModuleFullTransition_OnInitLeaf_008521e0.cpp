// NCameraModes::CScriptModuleFullTransition::OnInitLeaf() -- start the transition: put the owning camera at the start position and aim it at the start look-at point
// Retail: Fable.exe (TLC, VC7.1) @ 0x008521E0, 53 bytes, plain `ret`.
// Leaf: ?OnInitLeaf@CScriptModuleFullTransition
// Name: vtable 0x01275A44 slot 8; FableWin.pdb ?OnInitLeaf@CScriptModuleFullTransition@NCameraModes@@UAEXXZ.
// Layout: Ego_r.pdb CScriptModuleFullTransition (landed ctor 0x008525F0): LookAtPos +0x64, StartPos +0x70; the owner (POwner
//         +0x38) is the NCameraModes::CBase with LookAt +0x10 and GoTo.Position +0x54.
// Shape: two inline CBase setters (SetPosition, SetObservedPos); POwner re-read after the first copy.
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

struct CScriptModuleFullTransition : CScriptModuleBase {   // vtable 0x01275A44
    C3DVector IncrementForLookAtPoints;              // +0x4c
    C3DVector Increment;                             // +0x58
    C3DVector LookAtPos;                             // +0x64
    C3DVector StartPos;                              // +0x70
    long      EndFrame;                              // +0x7c
    virtual void OnInitLeaf();
};
void CScriptModuleFullTransition::OnInitLeaf()
{
    POwner->SetPosition(StartPos);
    POwner->SetObservedPos(LookAtPos);
}
}