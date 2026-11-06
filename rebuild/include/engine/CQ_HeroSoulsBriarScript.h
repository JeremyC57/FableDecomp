#ifndef FABLE_ENGINE_CQ_HEROSOULSBRIARSCRIPT_H
#define FABLE_ENGINE_CQ_HEROSOULSBRIARSCRIPT_H
// Retail opt-in (chunk 789): NScript::CQ_HeroSoulsBriarScript, new header (no includer before chunk 789; the 29 landed sibling rows
// keep their local models).
// Layout: Ego_r.pdb CQ_HeroSoulsBriarScript (struct_layouts_egor.tsv, 140 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48), then -4 after the 12-byte STLport map: PlayedBriarCS +0x48, seh_BriarRose +0x4c, csargs
// map<CCharString,CCharString> +0x5c, BriarRoseImposterTauntTimer +0x68, BriarRoseImposterHereTimer +0x6c, KickOffQuestStartScreen
// +0x70, sizeof 0x74 (the landed Alloc 0x00D83300 news 0x74 bytes). Checked against retail: CBriarRose::Main 0x00D82640 reads /
// writes PlayedBriarCS +0x48, assigns and releases seh_BriarRose +0x4c, calls map::operator[] 0x009AC700 on csargs +0x5c and sets
// KickOffQuestStartScreen +0x70.
// Nested CBriarRose (Ego_r.pdb, 32 bytes): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18, FirstSpeakAndWaitDone
// +0x1c (the Alloc 0x00D81630 news 0x20; Main 0x00D82640 tests and sets +0x1c).
// Retail methods: the Ego_r.pdb name set ?<Name>@CQ_HeroSoulsBriarScript@NScript@@...; PlayCutscene 0x00D823A0
// (?PlayCutscene@CQ_HeroSoulsBriarScript@NScript@@QAEXVCCharString@@@Z).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the entity scripts' PMasterData is CQ_SunnyvaleMasterData*).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_HeroSoulsBriarScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CTimer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"   // needs FABLE_RETAIL_BASE_OBJECT
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HeroSoulsBriarScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00D80D50 (slot 1)
    virtual void Main();                                                 // 0x00D80E40 (slot 2)
    virtual void Init();                                                 // 0x00D80CC0 (slot 3)
    virtual void OnPersist(CPersistContext&);                            // 0x00D80E10
    void PlayCutscene(CCharString);                                      // 0x00D823A0
    bool PlayedBriarCS;                                                  // +0x48
    CScriptGameResourceObjectScriptedThingBase seh_BriarRose;            // +0x4c
    std::map<CCharString,CCharString> csargs;                            // +0x5c
    CTimer BriarRoseImposterTauntTimer;                                  // +0x68
    CTimer BriarRoseImposterHereTimer;                                   // +0x6c
    bool KickOffQuestStartScreen;                                        // +0x70
    class CBriarRose : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsBriarScript&, CScriptDataBase*, const CScriptThing&); // 0x00D81630
        virtual CScriptBase* GetParentScript() const;                    // 0x00D816D0
    protected:
        CQ_HeroSoulsBriarScript& ParentClass;                            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        bool FirstSpeakAndWaitDone;                                      // +0x1c
        virtual void Init();                                             // 0x00D816E0
        virtual void OnPredicateFail();                                  // 0x00D816F0
        virtual void Main();                                             // 0x00D82640
    };
    class CBRIAR_ROSE_IMPOSTER;                                          // Alloc 0x00D81750
    class CHSB_MagicBarrier;                                             // Alloc 0x00D81B60
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsBriarScript) == 0x74);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsBriarScript::CBriarRose) == 0x20);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_HEROSOULSBRIARSCRIPT_H
