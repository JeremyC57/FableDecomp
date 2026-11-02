// CAIStateGroup_SkillPractice::Update() -- when the skill practice ran and the group is not stopping, schedule the next practice one minute of frames ahead; then terminate the state group
// Leaf: ?Update@CAIStateGroup_SkillPractice@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092B3A0, 60 bytes (int3 padding to 0x0092B3E0), protected virtual, both exits tail-jump to Terminate.
// Identity: the only reference is slot 12 (Update; CAIStateGroupBase slot 12 is _purecall) of vtable 0x0128AF1C (RTTI
//   .?AVCAIStateGroup_SkillPractice@@ : CAIStateGroup_ApprenticeHero : CAIStateGroupBase : CBaseClass). Ego_r.pdb + FableWin.pdb
//   ?Update@CAIStateGroup_SkillPractice@@MAEXXZ; donor order IsRunnable / Update / PracticeSkills = retail 0x0092B360 / 0x0092B3A0 /
//   0x0092B3E0 (both PDBs). The catalog label is right.
// Callees: the virtual PracticeSkills() (slot 14, +0x38; added by CAIStateGroup_ApprenticeHero, overridden here 0x0092B3E0; both PDBs
//   ?PracticeSkills@CAIStateGroup_SkillPractice@@MAE_NXZ), CAIStateGroupBase::IsStopping() const 0x008FD010 (both PDBs, protected),
//   CWorld::GetFrame() 0x0049D870 (?GetFrame@CWorld@@SIJXZ, static __fastcall), CAIStateGroupBase::Terminate() 0x008FCF40 (both PDBs).
// Global: CMainGameComponent::ConstantFPS 0x01375550 (?ConstantFPS@CMainGameComponent@@1JA, protected static) through the inline static
//   accessor GetConstantFPS() (naming wave 18 convention); `imul edi,edi,3Ch` = one minute of frames.
// Shape: the early-exit form `if (!PracticeSkills() || IsStopping()) { Terminate(); return; }` gives retail's two `mov ecx,esi; pop esi;
//   jmp Terminate` exits with `this` in esi and edi saved only around the frame call; the single `if (..) {..} Terminate();` form keeps
//   `this` in edi and shares one exit (52v60). The landed ApprenticeHero::Update 0x0092B110 writes its exits out the same way.
// Header: engine/CAIStateGroup_ApprenticeHero.h retail opt-in FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO, which selects
//   engine/CAIStateGroupBase.h's retail branch and its FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS virtual list (hdr/ overlay, chunk 707).
// Layout: CWorld / CMainGameComponent declare only the static members this row reads (engine/CWorld.h is a flattened data layout without
//   member functions; no engine header for CMainGameComponent).
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO
#include "engine/CAIStateGroup_ApprenticeHero.h"

class CWorld {
public:
    static long __fastcall GetFrame();                   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CAIStateGroup_SkillPractice : public CAIStateGroup_ApprenticeHero {   // vtable 0x0128AF1C
protected:
    virtual bool IsRunnable();                           // slot 11 0x0092B360
    virtual void Update();                               // slot 12 (this row)
    virtual bool PracticeSkills();                       // slot 14 0x0092B3E0
};

void CAIStateGroup_SkillPractice::Update()
{
    if (!PracticeSkills() || IsStopping()) {
        Terminate();
        return;
    }
    NextFramePractice = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * 60;
    Terminate();
}