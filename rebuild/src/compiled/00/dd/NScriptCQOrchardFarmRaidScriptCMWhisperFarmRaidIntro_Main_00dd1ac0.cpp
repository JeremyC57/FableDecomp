// NScript::CQ_OrchardFarmRaidScript::CM_WhisperFarmRaidIntro::Main() -- when the parent raid script (+0x14) has not run its cutscene (+0x88) and this script is not aborted (0x00F35B30), mark the cutscene done and run the parent's multiplier cutscene (0x00DD1AF0)
// Leaf: ?Main@CM_WhisperFarmRaidIntro@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD1AC0, length 48 bytes.
// Callees: the abort test @ 0x00F35B30 (on `this`, bool; propagated manifest label), CQ_OrchardFarmRaidScript::DoMultiplierCutscene @ 0x00DD1AF0 (a tail jump). Shape: the parent re-read after the call, `ret`.
// Naming wave 35: identity reg 00dcca86 "M_WhisperFarmRaidIntro" in 00dcc770; Alloc 00dcf540; vtable 012db4d4 slot 1; PDB ?Main@CM_WhisperFarmRaidIntro@CQ_OrchardFarmRaidScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CM_WhisperFarmRaidIntro::Main (wave3/labels_alloc.tsv); PDB ?Main@CM_WhisperFarmRaidIntro@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CM_WhisperFarmRaidIntro@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic); outer stand-in CQ_OrchardFarmRaidScript moved into NScript); same bytes.
namespace NScript { class CPredicateBase; }
namespace NScript {
class CQ_OrchardFarmRaidScript {
public:                  // the parent quest script (+0x14)
    unsigned char _pad[0x78];
    void*         m_pRaidThing;                    // +0x78
    unsigned char _pad7c[0xc];
    bool          m_cutsceneDone;                  // +0x88
    void DoMultiplierCutscene();                   // 0x00DD1AF0
public:
    class CM_WhisperFarmRaidIntro;   // the entity class below (naming wave 35)
};
}
using NScript::CQ_OrchardFarmRaidScript;
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

namespace NScript {
class CQ_OrchardFarmRaidScript::CM_WhisperFarmRaidIntro : public CEntityScriptBase {
public:
        unsigned char             _pad04[0x10];
    CQ_OrchardFarmRaidScript* ParentClass;         // +0x14
private:
    virtual void Main();                          // PDB ?Main@CM_WhisperFarmRaidIntro@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ
};
}

void NScript::CQ_OrchardFarmRaidScript::CM_WhisperFarmRaidIntro::Main()
{
    if (!ParentClass->m_cutsceneDone && !IsActiveThreadTerminating()) {
        ParentClass->m_cutsceneDone = true;
        ParentClass->DoMultiplierCutscene();
    }
}