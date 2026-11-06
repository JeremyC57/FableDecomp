#ifndef FABLE_ENGINE_CQ_HEROSOULSSCRIPT_H
#define FABLE_ENGINE_CQ_HEROSOULSSCRIPT_H
// Retail opt-in (chunk 790): NScript::CQ_HeroSoulsScript, new header (no includer before chunk 790; the 11 landed sibling rows
// keep their local models).
// Layout: Ego_r.pdb CQ_HeroSoulsScript (struct_layouts_egor.tsv, 100 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48; no STLport container in the class): HeardFirstBriarRoseMessage +0x48, HeardSecondBriarRoseMessage
// +0x49, HeardGMMessage +0x4a, HeardJackMessage +0x4b, PlayedOpeningCS +0x4c, AmLoadingGame +0x4d, sizeof 0x50 (the landed
// Alloc 0x00D7AC50 news 0x50 bytes). Checked against retail: the landed OnPersist 0x00D785E0 transfers +0x48..+0x4c by name;
// Main 0x00D78670 tests and sets all six.
// Retail methods: RegisterMain 0x00D78520 (slot 1), Main 0x00D78670 (slot 2 of the script vtable 0x012D2F48 that the landed ctor
// 0x00D784D0 stores), Init 0x00D78500 (slot 3), OnPersist 0x00D785E0 (slot 4); PlayCutscene 0x00D7AA30
// (FableWin.pdb ?PlayCutscene@CQ_HeroSoulsScript@NScript@@QAEXVCCharString@@@Z).
// Nested entity scripts (Ego_r.pdb ?Alloc@<Class>@CQ_HeroSoulsScript@NScript@@...; Main binds each under its class name less the
// `C`): CNotForODG (Alloc 0x00D79B90), CEndScythe (0x00D79C80), CEndBriarRose (0x00D7A290), forward-declared only.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (Main reads the master data's hero-souls members through +0x44).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_HeroSoulsScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CLevelScriptBase.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HeroSoulsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00D78520 (slot 1)
    virtual void Main();                                                 // 0x00D78670 (slot 2)
    virtual void Init();                                                 // 0x00D78500 (slot 3)
    virtual void OnPersist(CPersistContext&);                            // 0x00D785E0 (slot 4)
    void PlayCutscene(CCharString);                                      // 0x00D7AA30
    bool HeardFirstBriarRoseMessage;                                     // +0x48
    bool HeardSecondBriarRoseMessage;                                    // +0x49
    bool HeardGMMessage;                                                 // +0x4a
    bool HeardJackMessage;                                               // +0x4b
    bool PlayedOpeningCS;                                                // +0x4c
    bool AmLoadingGame;                                                  // +0x4d
    class CNotForODG;                                                    // Alloc 0x00D79B90
    class CEndScythe;                                                    // Alloc 0x00D79C80
    class CEndBriarRose;                                                 // Alloc 0x00D7A290
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsScript) == 0x50);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_HEROSOULSSCRIPT_H
