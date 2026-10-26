// NCameraModes::CScriptModuleTransitionBetweenPoints::OnInitLeaf() -- start the transition: put the owning camera at the start position, aim it at the start look-at point and apply the transition FOV
// Retail: Fable.exe (TLC, VC7.1) @ 0x00852040, 68 bytes, plain `ret`.
// Leaf: ?OnInitLeaf@CScriptModuleTransitionBetweenPoints
// Name: vtable 0x012759F4 slot 8 (CScriptModuleBase::OnInitLeaf 0x005ED110 is empty); FableWin.pdb
//       ?OnInitLeaf@CScriptModuleTransitionBetweenPoints@NCameraModes@@MAEXXZ.
// Layout: Ego_r.pdb CScriptModuleTransitionBetweenPoints (landed ctor 0x008524C0): LookAtPos +0x6c, StartPos +0x78, FOV +0x88;
//         the owner (POwner +0x38) is the NCameraModes::CBase with LookAt +0x10, GoTo.Position +0x54, the FOV +0x84.
// Shape: three inline CBase setters (FableWin.pdb SetPosition/SetObservedPos/SetFOV); POwner is re-read for each store because
//        the C3DVector copies may alias it.
class CBaseObject;
class CGameCameraManager;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
struct CIntelligentPointer_CThingConst {             // CIntelligentPointer<const CThing>: vptr + link
    virtual void v00();
    void* m_pItem;
};
namespace NCameraModes {
class CBase { public:                                       // the owning camera mode (retail offsets, landed CBase dtor 0x0056E0E0)
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
class CScriptModuleBase { public:                           // vtable 0x01255F74, 0x4c bytes
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

struct CScriptModuleTransitionBetweenPoints : CScriptModuleBase {   // vtable 0x012759F4
    CIntelligentPointer_CThingConst PThing;          // +0x4c
    C3DVector Increment;                             // +0x54
    C3DVector LookAtIncrement;                       // +0x60
    C3DVector LookAtPos;                             // +0x6c
    C3DVector StartPos;                              // +0x78
    long      EndFrame;                              // +0x84
    float     FOV;                                   // +0x88
    bool      PreventSelfTermination;                // +0x8c
    virtual void OnInitLeaf();
};
void CScriptModuleTransitionBetweenPoints::OnInitLeaf()
{
    POwner->SetPosition(StartPos);
    POwner->SetObservedPos(LookAtPos);
    POwner->SetFOV(FOV);
}
}