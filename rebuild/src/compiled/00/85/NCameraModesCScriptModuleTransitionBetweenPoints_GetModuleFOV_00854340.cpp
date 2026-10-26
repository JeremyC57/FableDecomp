// NCameraModes::CScriptModuleTransitionBetweenPoints::GetModuleFOV(float& fov) const -- a transition always supplies its FOV
// Retail: Fable.exe (TLC, VC7.1) @ 0x00854340, 18 bytes, `ret 4`.
// Leaf: ?GetModuleFOV@CScriptModuleTransitionBetweenPoints
// Name: vtable 0x012759F4 slot 5 (CScriptModuleBase::GetModuleFOV 0x00851DD0 is the base slot); FableWin.pdb
//       ?GetModuleFOV@CScriptModuleTransitionBetweenPoints@NCameraModes@@UBE_NAAM@Z.
// Layout: Ego_r.pdb CScriptModuleTransitionBetweenPoints (0x90) as the landed ctor 0x008524C0: FOV +0x88.
// Shape: the float copied through eax, `mov al,1`.
class CBaseObject;
class CGameCameraManager;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
struct CIntelligentPointer_CThingConst {             // CIntelligentPointer<const CThing>: vptr + link
    virtual void v00();
    void* m_pItem;
};
namespace NCameraModes {
class CBase;
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
    virtual bool GetModuleFOV(float& fov) const;
};
bool CScriptModuleTransitionBetweenPoints::GetModuleFOV(float& fov) const
{
    fov = FOV;
    return true;
}
}