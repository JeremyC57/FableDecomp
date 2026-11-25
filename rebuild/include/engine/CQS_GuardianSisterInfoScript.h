#ifndef FABLE_ENGINE_CQS_GUARDIANSISTERINFOSCRIPT_H
#define FABLE_ENGINE_CQS_GUARDIANSISTERINFOSCRIPT_H
// The "QS_GuardianSisterInfo" level script and its CMazeAtTavern entity script (Maze in the Bowerstone tavern).
// Ego_r.pdb CQS_GuardianSisterInfoScript (96 bytes on the donor base: GuardianSpokeToHero +0x5c) is native 0x4C (Alloc 0x00E26780:
// operator new 0x4C; GuardianSpokeToHero +0x48, read by CMazeAtTavern::Main 0x00E25F70 at 0x00E26138), script vtable 0x012E1DAC:
// RegisterMain 0x00E25A10, Main 0x00E25AB0, Init 0x00E25A00, OnPersist 0x00E25F40 (both PDBs).
// Ego_r.pdb CQS_GuardianSisterInfoScript::CMazeAtTavern (40 bytes = the retail new size, Alloc 0x00E25C70): ParentClass +0x14,
// PMasterData +0x18, TalkCounter +0x1c, WavedOver +0x20, HitCount +0x24. Entity vtable 0x012E1E28: scalar deleting dtor 0x00E25D70,
// Main 0x00E25F70, Init 0x00E25D20, GetParentScript 0x00E25D10, OnPredicateFail 0x00E25D30. Same shape as the twin
// engine/CQS_GuardianSisterInfo2_SisterInBanditCampScript.h (its CMazeAtTavern has the same three members).
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_GuardianSisterInfoScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&); // 0x00E26780
    virtual void RegisterMain();                                        // 0x00E25A10
    virtual void Init();                                                // 0x00E25A00
    virtual void OnPersist(CPersistContext&);                           // 0x00E25F40
    virtual void Main();                                                // 0x00E25AB0
    bool GuardianSpokeToHero;                                           // +0x48 (Ego_r +0x5c on the donor base)
    class CMazeAtTavern : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQS_GuardianSisterInfoScript&, CScriptDataBase*, const CScriptThing&); // 0x00E25C70
        virtual CScriptBase* GetParentScript() const;                   // 0x00E25D10
    protected:
        CQS_GuardianSisterInfoScript& ParentClass;                      // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                            // +0x18
    private:
        long TalkCounter;                                               // +0x1c
        bool WavedOver;                                                 // +0x20
        long HitCount;                                                  // +0x24
        virtual void Init();                                            // 0x00E25D20
        virtual void OnPredicateFail();                                 // 0x00E25D30
        virtual void Main();                                            // 0x00E25F70
    };
#ifdef FABLE_RETAIL_CQS_GUARDIANSISTERINFOSCRIPT_ALLOC
    // Retail opt-in (NF436, row 0x00E26780 Alloc): FableWin.pdb ??_7CQS_GuardianSisterInfoScript@NScript@@6B@ / LF_CLASS (all public: the user ctor, static
    //   Alloc, the virtual overrides) for what this header does not declare yet: CQS_GuardianSisterInfoScript. Declarations only, no layout
    //   change; existing includers do not define the macro.
public:
    CQS_GuardianSisterInfoScript(CScriptDataBase* data, CGameScriptInterfaceBase& game); // FableWin.pdb ??0CQS_GuardianSisterInfoScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
#endif
};
}
FABLE_STATIC_ASSERT_AT(41, sizeof(NScript::CQS_GuardianSisterInfoScript)==0x4c);
FABLE_STATIC_ASSERT_AT(42, sizeof(NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern)==0x28);
#endif
