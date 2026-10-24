// NCameraModes::CScriptModuleBase::GetModuleFOV(float& fov) const -- a script module's FOV override, when one was set (positive)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00851DD0, 35 bytes, `ret 4`.
// Leaf: ?GetModuleFOV@CScriptModuleBase
// Name: vtable 0x01255F74 slot 5; FableWin.pdb ?GetModuleFOV@CScriptModuleBase@NCameraModes@@UBE_NAAM@Z (SetFOVOverride stores
//       FOVOverride). Slot map from the base vtable and its overrides: 2 IsFinished (0x005ED0E0 false), 3 IsShortenOnOcclusion
//       (0x005ED0F0 = ShortenOnCollision; CScriptModuleOpenChest overrides it), 4 IsStretchAroundHero, 5 GetModuleFOV, 6
//       GetModuleRollAngle (0x005ED100 false, `ret 4`), 7 ResetCameraOnEnd (CScriptModuleCameraOnAStick overrides it), 8 OnInitLeaf.
// Layout: Ego_r.pdb CScriptModuleBase (0x4c), as the landed CScriptModuleSpline::IsFinished 0x00853260: FOVOverride +0x48.
// Shape: `FOVOverride > 0.0f` against the literal pool zero (`fcomp ; test ah,41h ; jne`), then the float copied as a dword.
struct CBaseObject;
struct CGameCameraManager;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CIntelligentPointer_CThingConst {             // CIntelligentPointer<const CThing>: vptr + link
    virtual void v00();
    void* m_pItem;
};
namespace NCameraModes {
struct CBase;
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

bool CScriptModuleBase::GetModuleFOV(float& fov) const
{
    if (FOVOverride > 0.0f) {
        fov = FOVOverride;
        return true;
    }
    return false;
}
}