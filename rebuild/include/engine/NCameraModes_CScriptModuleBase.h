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
#ifdef FABLE_RETAIL_NCAMERAMODES_CSCRIPTMODULEBASE_SETTERS
    // p25E opt-in (row 0x00859EF0 CCombatActionBase::OnAnimationLoaded): FableWin.pdb ?SetPCameraPoint@CScriptModuleBase@NCameraModes@@QAEXAAVCThing@@@Z
    // (0x0253DF60; Ego_r.pdb too, retail 0x0072A4A0) and ?SetPThingOrigin@CScriptModuleBase@NCameraModes@@QAEXABVCThing@@@Z (0x02839730;
    // retail 0x00852750), both out of line in retail. Declarations only.
    void SetPCameraPoint(CThing& camera_point);                   // 0x0072A4A0
    void SetPThingOrigin(const CThing& thing);                    // 0x00852750
#endif
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
    FABLE_STATIC_ASSERT_AT(46, sizeof(CScriptModuleBase) == 0x4c);
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
    FABLE_STATIC_ASSERT_AT(63, sizeof(CScriptModuleSelfTerminateBase) == 0x54);
};
#ifdef FABLE_RETAIL_NCAMERAMODES_CSCRIPTMODULESPLINE
// p25E opt-in (row 0x00859EF0 CCombatActionBase::OnAnimationLoaded): NCameraModes::CScriptModuleSpline, Ego_d.pdb LF_CLASS 0x3C172 (field
// list 0x3C171: public base CScriptModuleBase, sizeof 100) and Ego_r.pdb (struct_layouts_egor.tsv, 0x64; no STL member, so the offsets
// hold in retail: `push 64h ; call operator new` at 0x0085A2D1). Methods: Ego_r.pdb / FableWin.pdb ??0CScriptModuleSpline@NCameraModes@@QAE@JJ@Z
// (retail 0x00852710, out of line) and the virtual overrides IsFinished (landed 0x00853260), Update, OnInitLeaf (landed 0x008540F0),
// GetModuleFOV (landed 0x008543B0), GetModuleRollAngle (landed 0x008543C0), the compiler-generated virtual dtor. Declarations only.
class CScriptModuleSpline : public CScriptModuleBase {
public:
    CScriptModuleSpline(long node_to_start_on, long node_to_end_on); // 0x00852710
    virtual ~CScriptModuleSpline();
    virtual bool IsFinished() const;
    virtual void Update();
    virtual void OnInitLeaf();
    virtual bool GetModuleFOV(float& fov) const;
    virtual bool GetModuleRollAngle(float& angle) const;
private:
    float Index;                                                  // +0x4C
    float CurrentSplineFOV;                                       // +0x50
    float CurrentSplineRollAngle;                                 // +0x54
    bool ReallyFinished;                                          // +0x58
    bool FinishedSpline;                                          // +0x59
    long NodeToStartOn;                                           // +0x5C
    long NodeToEndOn;                                             // +0x60
};
FABLE_STATIC_ASSERT(sizeof(CScriptModuleSpline) == 0x64);
#endif
}
#endif
