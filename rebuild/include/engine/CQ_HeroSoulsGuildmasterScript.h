#ifndef FABLE_ENGINE_CQ_HEROSOULSGUILDMASTERSCRIPT_H
#define FABLE_ENGINE_CQ_HEROSOULSGUILDMASTERSCRIPT_H
// Retail opt-in (chunk 788): NScript::CQ_HeroSoulsGuildmasterScript, new header (no includer before chunk 788).
// Layout: Ego_r.pdb CQ_HeroSoulsGuildmasterScript (struct_layouts_egor.tsv, 0x7c bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48), then -4 for the 12-byte STLport map: PlayedBriarCS +0x48 .. GiveWarningCutscene +0x4c,
// GuardInWoodsCount +0x50, DistanceReached +0x54, cs_flags map<CCharString,bool> +0x58, sizeof 0x64 -- the layout the landed ctor
// 0x00D84B50 builds and the landed PlayCutscene 0x00D872D0 reads (cs_flags `lea ecx,[esi+58h]`). Main 0x00D84D60 reads PlayedGuildMasterCS
// +0x49, PlayedGuildCS +0x4a, GuardInWoodsCount +0x50 and looks up cs_flags["GMKILLED"].
// Retail methods: Main 0x00D84D60 (slot 2 of the script vtable 0x012D4804), Init 0x00D84BB0 (slot 3), RegisterMain 0x00D84C20 (slot 1),
// PlayCutscene(CCharString) 0x00D872D0 (landed, Ego_r.pdb ?PlayCutscene@CQ_HeroSoulsGuildmasterScript@NScript@@QAEXVCCharString@@@Z).
// Nested entity scripts: the seven classes Main binds (Ego_r.pdb ??0<Class>@CQ_HeroSoulsGuildmasterScript@NScript@@...; the binding-name
// literal is the class name less its `C`), forward-declared only.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_HeroSoulsGuildmasterScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_map.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HeroSoulsGuildmasterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00D84C20 (slot 1)
    virtual void Main();                                                 // 0x00D84D60 (slot 2)
    virtual void Init();                                                 // 0x00D84BB0 (slot 3)
    void PlayCutscene(CCharString);                                      // 0x00D872D0
    bool PlayedBriarCS;                                                  // +0x48
    bool PlayedGuildMasterCS;                                            // +0x49
    bool PlayedGuildCS;                                                  // +0x4a
    bool OutsideWoodGuardsAttack;                                        // +0x4b
    bool GiveWarningCutscene;                                            // +0x4c
    long GuardInWoodsCount;                                              // +0x50
    long DistanceReached;                                                // +0x54
    std::map<CCharString,bool> cs_flags;                                 // +0x58
    class CGuildmaster_HS;                                               // Alloc 0x00D85A30
    class CGuardOutsideWoods;                                            // Alloc 0x00D85FA0
    class CMuteGuardOutsideWoods;                                        // Alloc 0x00D86080
    class CGuardInWoods;                                                 // Alloc 0x00D86390
    class CWaitingGuardInWoods;                                          // Alloc 0x00D86560
    class CHSG_DistanceChecker;                                          // Alloc 0x00D86980
    class CSoulInfoAcolyte;                                              // Alloc 0x00D86B60
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsGuildmasterScript) == 0x64);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_HEROSOULSGUILDMASTERSCRIPT_H
