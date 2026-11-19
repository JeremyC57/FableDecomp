#ifndef FABLE_ENGINE_NCAMERAMODES_CSCRIPTMODULEBASE_H
#define FABLE_ENGINE_NCAMERAMODES_CSCRIPTMODULEBASE_H
// NCameraModes::CScriptModuleBase (hand4a w24, new header; rows 0x00853670 / 0x008539C0 OnInitLeaf): Ego_d.pdb LF_CLASS 0x3C015
// (field list 0x3C014, sizeof 76) and Ego_r.pdb struct_layouts_egor.tsv (0x4c): PCamPoint +0x04, PThingOrigin +0x0C, CoordOrigin
// +0x14, CoordAxis +0x20, POwner +0x38, PCameraManager +0x3C, SelfTerminate +0x40, ShortenOnCollision +0x41, CutOutOverride +0x44,
// FOVOverride +0x48. Virtual slots (Ego_d vftable offsets; landed GetModuleFOV 0x00851DD0 slot map, vtable 0x01255F74): 0 dtor,
// 1 Update (pure), 2 IsFinished, 3 IsShortenOnOcclusion, 4 IsStretchAroundHero, 5 GetModuleFOV, 6 GetModuleRollAngle,
// 7 ResetCameraOnEnd, 8 OnInitLeaf (protected). GetOwner: FableWin.pdb ?GetOwner@CScriptModuleBase@NCameraModes@@IAEAAVCBase@2@XZ
// (protected, inline in retail: OnInitLeaf reads [this+38h] then calls CBase::GetPWorldSeed 0x0084D2B0).
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CIntelligentPointer.h"
class CThing;
class CGameCameraManager;
namespace NCameraModes {
class CBase;
class CScriptModuleBase {
public:
    virtual ~CScriptModuleBase();                                 // slot 0
    virtual void Update() = 0;                                    // slot 1
    virtual bool IsFinished() const;                              // slot 2
    virtual bool IsShortenOnOcclusion() const;                    // slot 3
    virtual bool IsStretchAroundHero() const;                     // slot 4
    virtual bool GetModuleFOV(float& fov) const;                  // slot 5
    virtual bool GetModuleRollAngle(float& angle) const;          // slot 6
    virtual bool ResetCameraOnEnd() const;                        // slot 7
protected:
    CBase& GetOwner() { return *POwner; }
    virtual void OnInitLeaf();                                    // slot 8
private:
    CIntelligentPointer<CThing>       PCamPoint;                  // +0x04
    CIntelligentPointer<const CThing> PThingOrigin;               // +0x0C
    C3DVector                         CoordOrigin;                // +0x14
    CRightHandedSet                   CoordAxis;                  // +0x20
    CBase*                            POwner;                     // +0x38
    const CGameCameraManager*         PCameraManager;             // +0x3C
    bool                              SelfTerminate;              // +0x40
    bool                              ShortenOnCollision;         // +0x41
    long                              CutOutOverride;             // +0x44
    float                             FOVOverride;                // +0x48
    friend struct CScriptModuleBaseLayout;
};
struct CScriptModuleBaseLayout {
    FABLE_STATIC_ASSERT(sizeof(CScriptModuleBase) == 0x4c);
};

// NCameraModes::CScriptModuleSelfTerminateBase: Ego_d.pdb LF_CLASS 0x3C07C (field list 0x3C07B, sizeof 84): WFDuration +0x4C,
// EndFrame +0x50; GetDuration: FableWin.pdb ?GetDuration@CScriptModuleSelfTerminateBase@NCameraModes@@IBEJXZ (protected const,
// inline in retail: the OnInitLeaf rows read [this+4Ch] once).
class CScriptModuleSelfTerminateBase : public CScriptModuleBase {
public:
    virtual bool IsFinished() const;
protected:
    long GetDuration() const { return WFDuration; }
private:
    long WFDuration;                                              // +0x4C
    long EndFrame;                                                // +0x50
    friend struct CScriptModuleSelfTerminateBaseLayout;
};
struct CScriptModuleSelfTerminateBaseLayout {
    FABLE_STATIC_ASSERT(sizeof(CScriptModuleSelfTerminateBase) == 0x54);
};
}
#endif
