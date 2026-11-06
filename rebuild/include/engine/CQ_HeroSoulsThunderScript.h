#ifndef FABLE_ENGINE_CQ_HEROSOULSTHUNDERSCRIPT_H
#define FABLE_ENGINE_CQ_HEROSOULSTHUNDERSCRIPT_H
// Retail opt-in (chunk 798): NScript::CQ_HeroSoulsThunderScript, new header (no includer before chunk 798; the 10 landed rows keep
// their local models).
// Layout: Ego_r.pdb CQ_HeroSoulsThunderScript (struct_layouts_egor.tsv, 112 bytes) at retail offsets PDB - 0x14 (retail
// CLevelScriptBase is 0x48; the STLport map is 12 bytes): PlayedThunderCS +0x48, QuestStartScreened +0x49, JackSpoken +0x4a,
// csargs +0x4c; 0x58 bytes. Checked against retail: the landed PlayCutscene 0x00D7C470 passes &csargs (+0x4c); CHeroThunder::Main
// 0x00D7B510 reads ParentClass+0x48 / +0x49 and sets +0x49.
// Nested entity script CHeroThunder (Ego_r.pdb, 28 bytes: CEntityScriptBase 0x14, ParentClass +0x14, PMasterData +0x18; the landed
// Alloc 0x00D7B400 news 0x1c): Alloc 0x00D7B400, GetParentScript 0x00D7B4A0, OnPredicateFail 0x00D7B4B0 (landed), Main 0x00D7B510
// (?Main@CHeroThunder@CQ_HeroSoulsThunderScript@NScript@@EAEXXZ; slot 1 of its entity vtable, .rdata ref 0x012D3664).
// Contract: the includer includes stlport/fable_containers.h and stlport/fable_map.h first.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_HeroSoulsThunderScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_map.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HeroSoulsThunderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_HeroSoulsThunderScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00D7ACB0
    virtual ~CQ_HeroSoulsThunderScript();
    virtual void RegisterMain();                                         // 0x00D7AD70
    virtual void Main();                                                 // 0x00D7AE80
    virtual void Init();                                                 // 0x00D7AD10
    virtual void OnPersist(CPersistContext&);
    void PlayCutscene(CCharString);                                      // 0x00D7C470
    bool PlayedThunderCS;                                                // +0x48
    bool QuestStartScreened;                                             // +0x49
    bool JackSpoken;                                                     // +0x4a
    std::map<CCharString, CCharString> csargs;                           // +0x4c
    class CHeroThunder;                                                  // Alloc 0x00D7B400
};
class CQ_HeroSoulsThunderScript::CHeroThunder : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsThunderScript&, CScriptDataBase*, const CScriptThing&); // 0x00D7B400
    virtual CScriptBase* GetParentScript() const;                        // 0x00D7B4A0
private:
    virtual void Main();                                                 // 0x00D7B510
    virtual void OnPredicateFail();                                      // 0x00D7B4B0
protected:
    CQ_HeroSoulsThunderScript& ParentClass;                              // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsThunderScript) == 0x58);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsThunderScript::CHeroThunder) == 0x1c);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_HEROSOULSTHUNDERSCRIPT_H
