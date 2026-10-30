// CTCAssassinRush::ResetCameraOnStick() -- hand the game camera manager a new camera-on-a-stick script module built from the rushing thing and the two camera vectors (not preloaded)
// Grade: RELOCATION_MATCH (wave 2 sec3 retry: the PDB inline accessor GetParentThing() gives retail's ParentThing-first argument load; local CCountedPointer moved onto engine/CCountedPointer.h, module classes keyed `class` as the bound ctor 0x005ED270 spells them)
// Leaf: ?ResetCameraOnStick@CTCAssassinRush@@AAEXXZ
// Retail: Fable.exe @ 0x007A9B10, 91 bytes, thiscall `ret`.
// Identity (probable): the only caller is the landed CTCAssassinRush::SetupCameraOnStick 0x007A9C80 (tail call on
//   `this` after it fills LocalRelativeVectorForCamera / RelativeVectorForCamera); the body calls the protected
//   CTCBase::GetWorld 0x00686A70 and reads ParentThing +4 and the two vectors (Ego_r.pdb CTCAssassinRush +0x78 / +0x84,
//   retail +0x74 / +0x80), so it is a CTCAssassinRush member. Of the PDB's CTCAssassinRush void() members not yet bound
//   (ResetCameraOnStick, RushForwards, ResumeForCutscene, SetFakeCollideWithSomething) only ResetCameraOnStick fits a
//   body that only (re)installs the on-a-stick camera. The catalog label "CCreatureAction_ReleasePhysicalShieldSpell::
//   FinishAction" is propagated; the landed caller spells it `SetupCameraFromStick_007a9b10` (descriptive).
// Callees: CTCBase::GetWorld() const 0x00686A70, the current game camera's slot 9 GetGameCameraManager(out) (world +0x34,
//   the landed CTCDRumble 0x007F1930 model), operator new 0x00BFEA1A (0x68 bytes), NCameraModes::
//   CScriptModuleCameraOnAStick ctor 0x007AB450 (the landed `void*` spelling), CCountedPointer<CScriptModuleBase>(T*)
//   0x005ED270, CGameCameraManager::SetGameScriptedCameraMode(CCountedPointer<CScriptModuleBase>, bool) 0x00697B70.
class C3DVector { public:
    float X, Y, Z;
};
class CThing;
namespace NCameraModes {
class CScriptModuleBase { public:
    virtual void v00();
    unsigned char m_state[0x4C - 4];
};
class CScriptModuleCameraOnAStick : public CScriptModuleBase { public:
    void*     m_pStickTarget;                    // +0x4C
    C3DVector m_offset;                          // +0x50
    C3DVector m_lookAt;                          // +0x5C
    CScriptModuleCameraOnAStick(CThing* pStickTarget, const C3DVector& offset, const C3DVector& lookAt);   // 0x007AB450
};
}
#include "engine/CCountedPointer.h"
class CGameCameraManager { public:
    void SetGameScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> module, bool preload);   // 0x00697B70
};
class CAIGameCameraBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetGameCameraManager(CGameCameraManager** out);   // slot 9 (+0x24)
};
class CWorld { public:
    unsigned char      _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;       // +0x34
    bool GetGameCameraManager(CGameCameraManager** out) { return PCurrentGameCamera->GetGameCameraManager(out); }
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                         // +0x04
    CThing& GetParentThing() { return *ParentThing; }   // Ego_r.pdb ?GetParentThing@CTCBase@@QAEAAVCThing@@XZ (inline)
protected:
    CWorld& GetWorld() const;                    // 0x00686A70
};
class CTCAssassinRush : public CTCBase {
public:
    unsigned char _pad08[0x74 - 0x08];
    C3DVector LocalRelativeVectorForCamera;      // +0x74
    C3DVector RelativeVectorForCamera;           // +0x80
private:
    void ResetCameraOnStick();
};
void CTCAssassinRush::ResetCameraOnStick()
{
    CGameCameraManager* manager;
    if (GetWorld().GetGameCameraManager(&manager))
        manager->SetGameScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(
            new NCameraModes::CScriptModuleCameraOnAStick(&GetParentThing(), LocalRelativeVectorForCamera, RelativeVectorForCamera)), false);
}