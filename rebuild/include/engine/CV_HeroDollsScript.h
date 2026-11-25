#ifndef FABLE_ENGINE_CV_HERODOLLSSCRIPT_H
#define FABLE_ENGINE_CV_HERODOLLSSCRIPT_H
// Retail opt-in (chunk 845): NScript::CV_HeroDollsScript, new header (no includer before chunk 845; the landed sibling rows
// keep their local models).
// Layout: Ego_r.pdb CV_HeroDollsScript at retail offsets = PDB - 0x14 (retail CLevelScriptBase is 0x48): HeroDollScriptEnded
// +0x48, KidIntroDone +0x49 (the bytes CBS_Teacher::Main 0x00E97070 reads and writes through ParentClass: 0x00E970F6 / 0x00E9754D,
// 0x00E97582 / 0x00E97AE8); sizeof 0x4C (Alloc 0x00E98640 `push 4Ch`).
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
#ifdef FABLE_RETAIL_CV_HERODOLLSSCRIPT_ALLOC
    // Retail opt-in (NF436, row 0x00E98640 Alloc): FableWin.pdb ??_7CV_HeroDollsScript@NScript@@6B@ / LF_CLASS (all public: the user ctor, static
    //   Alloc, the virtual overrides) for what this header does not declare yet: CV_HeroDollsScript, Alloc, RegisterMain, Main, Init, OnPersist. Declarations only, no layout
    //   change; existing includers do not define the macro.
public:
    CV_HeroDollsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game); // FableWin.pdb ??0CV_HeroDollsScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00E98640
    virtual void RegisterMain();                            // slot 1 0x00E96E70
    virtual void Main();                                    // slot 2 0x00E96F10
    virtual void Init();                                    // slot 3 0x00E96E60
    virtual void OnPersist(CPersistContext&);               // slot 4 0x00E98610
#endif
};
FABLE_STATIC_ASSERT_AT(41, sizeof(CV_HeroDollsScript) == 0x4c);                 // Alloc 0x00E98640 `push 4Ch` (0x00E98643); Ego_r.pdb 96 - 0x14
FABLE_STATIC_ASSERT_AT(42, sizeof(CV_HeroDollsScript::CBS_Teacher) == 0x28);
} // namespace NScript
#endif // FABLE_RETAIL_NSCRIPT
#endif
