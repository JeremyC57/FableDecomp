#ifndef FABLE_ENGINE_CQ_PRISONWARDENGAMESCRIPT_H
#define FABLE_ENGINE_CQ_PRISONWARDENGAMESCRIPT_H
// Retail opt-in (chunk 803): NScript::CQ_PrisonWardenGameScript, new header (no includer before chunk 803).
// Layout: Ego_r.pdb CQ_PrisonWardenGameScript (struct_layouts_egor.tsv, 112 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): SecretDiaryOpened +0x48, HeroHasSecretDiaryKey +0x49, BookOpened +0x4c, SThingBookOpened +0x50 --
// the offsets CPrisonWarden::Main 0x00DDF090 reads (+0x48 tested, +0x4c compared, +0x50 GetPos / GetAngleXY / RemoveThing) and the
// landed CSecretBook::Main 0x00DDE940 writes. Class vtable 0x012DCB40 (reloc_bindings ??_7CQ_PrisonWardenGameScript@NScript@@6B@).
// Retail methods (Ego_r.pdb ?<Name>@CQ_PrisonWardenGameScript@NScript@@...): Main 0x00DDDE20 (landed), WardenKeyMiniGame
// 0x00DDE230, RegisterMain, Init.
// Nested entity scripts (Ego_r.pdb ?Alloc@<Class>@CQ_PrisonWardenGameScript@NScript@@SI...), forward-declared only.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (Game +0x40, MasterScriptData +0x44).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_PrisonWardenGameScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonWardenGameScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Main();                                                 // 0x00DDDE20
    virtual void Init();
    void WardenKeyMiniGame();                                            // 0x00DDE230
    bool SecretDiaryOpened;                                              // +0x48
    bool HeroHasSecretDiaryKey;                                          // +0x49
    long BookOpened;                                                     // +0x4c
    CScriptThing SThingBookOpened;                                       // +0x50
    class CPrisonWarden;                                                 // Alloc 0x00DDE630
    class CSecretBook;                                                   // Alloc 0x00DDE720
    class CExitWardensOffice;                                            // Alloc 0x00DDEC30
    class CDoNotTarget;                                                  // Alloc 0x00DDEDF0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_PrisonWardenGameScript) == 0x5c);
#ifdef FABLE_RETAIL_CQ_PRISONWARDENGAMESCRIPT_ENTITIES
// Retail opt-in (chunk 803): the prison warden entity script. Ego_r.pdb CPrisonWarden (struct_layouts_egor.tsv, 28 bytes:
// ParentClass +0x14, PMasterData +0x18). Entity vtable 0x012DCC50: 0 scalar deleting dtor 0x00DDE700, 1 Main 0x00DDF090, 2 Init
// 0x00DDE6E0, 3 GetParentScript 0x00DDE6D0, 5 OnPredicateFail 0x00DDE6F0; Main / Init / OnPredicateFail private (PDB EAEXXZ).
class CScriptDataBase;
namespace NScript {
class CQ_PrisonWardenGameScript::CPrisonWarden : public CEntityScriptBase {
public:
#ifdef FABLE_RETAIL_CQ_PRISONWARDENGAMESCRIPT_MASTER_ACCESSOR
    // Retail opt-in (Codex pass 21): Ego_r FF14 and FableWin100CE public member; donor0387C0E0 returns PMasterData at+18.
    CQ_SunnyvaleMasterData* GetMasterData() { return PMasterData; }
#endif
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_PrisonWardenGameScript&, CScriptDataBase*, const CScriptThing&); // 0x00DDE630
    virtual CScriptBase* GetParentScript() const;                        // 0x00DDE6D0
private:
    virtual void Main();                                                 // 0x00DDF090
    virtual void Init();                                                 // 0x00DDE6E0
    virtual void OnPredicateFail();                                      // 0x00DDE6F0
protected:
    CQ_PrisonWardenGameScript& ParentClass;                              // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_PrisonWardenGameScript::CPrisonWarden) == 0x1c);
#endif
#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_PRISONWARDENGAMESCRIPT_H
