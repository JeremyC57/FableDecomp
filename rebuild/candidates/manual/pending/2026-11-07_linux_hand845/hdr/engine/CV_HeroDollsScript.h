#ifndef FABLE_ENGINE_CV_HERODOLLSSCRIPT_H
#define FABLE_ENGINE_CV_HERODOLLSSCRIPT_H
// Retail opt-in (chunk 845): NScript::CV_HeroDollsScript, new header (no includer before chunk 845; the landed sibling rows
// keep their local models).
// Layout: Ego_r.pdb CV_HeroDollsScript at retail offsets = PDB - 0x14 (retail CLevelScriptBase is 0x48): HeroDollScriptEnded
// +0x48, KidIntroDone +0x49 (the bytes CBS_Teacher::Main 0x00E97070 reads through ParentClass).
// Retail methods (Ego_r.pdb / FableWin.pdb ?<Name>@CV_HeroDollsScript@NScript@@QAE..): RemoveHeroDolls 0x00E97EA0, HeroHasAllDolls
// 0x00E97FD0, HeroHasAnyDolls 0x00E98200 (all landed).
// Nested CBS_Teacher (Ego_r.pdb 40 bytes = the landed Alloc 0x00E96FB0 new size): CEntityScriptBase 0x14, ParentClass +0x14,
// PMasterData +0x18, KidIntro +0x1c; vtable 0x012ECEB4: 0 scalar deleting dtor 0x00E98430, 1 Main 0x00E97070, 2 Init
// 0x00E97060, 3 GetParentScript 0x00E97050 (Ego_r.pdb / FableWin.pdb ?<Name>@CBS_Teacher@CV_HeroDollsScript@NScript@@).
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_HeroDollsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void RemoveHeroDolls();                                              // 0x00E97EA0
    bool HeroHasAllDolls();                                              // 0x00E97FD0
    bool HeroHasAnyDolls();                                              // 0x00E98200
    bool HeroDollScriptEnded;                                            // +0x48
    bool KidIntroDone;                                                   // +0x49
    class CBS_Teacher : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_HeroDollsScript&, CScriptDataBase*, const CScriptThing&);   // 0x00E96FB0
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00E97050
    private:
        virtual void Main();                                             // slot 1 0x00E97070
        virtual void Init();                                             // slot 2 0x00E97060
    protected:
        CV_HeroDollsScript* ParentClass;                                 // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
        CScriptThing KidIntro;                                           // +0x1c
    };
};
FABLE_STATIC_ASSERT(sizeof(CV_HeroDollsScript::CBS_Teacher) == 0x28);
} // namespace NScript
#endif // FABLE_RETAIL_NSCRIPT
#endif
