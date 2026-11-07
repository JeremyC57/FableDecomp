#ifndef FABLE_ENGINE_CQ_BOWERSTONETOWNLIFEINTROSCRIPT_H
#define FABLE_ENGINE_CQ_BOWERSTONETOWNLIFEINTROSCRIPT_H
// Retail opt-in (chunk 837): NScript::CQ_BowerstoneTownLifeIntroScript, new header (no includer before chunk 837).
// Layout: Ego_r.pdb CQ_BowerstoneTownLifeIntroScript (struct_layouts_egor.tsv, 96 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): ArenaFinished +0x48, DoneGuardSpeech +0x49, PoshGuardComplete +0x4a; retail sizeof 0x4C (the landed
// Alloc 0x00D253B0 `push 4Ch`; the landed Guard Main 0x00D246C0 / GuardToPosh Init 0x00D243E0 read the same offsets, and the Main
// 0x00D23DC0 waits on +0x49 / +0x48 / +0x4a). Script vtable 0x012CB52C (RegisterMain 0x00D23D20, Main 0x00D23DC0, Init
// 0x00D23D10, OnPersist 0x00D24660; ctor 0x00D23CE0, Alloc 0x00D253B0 -- all landed but Main).
// Nested entity scripts (Ego_r.pdb ?Alloc@<Class>@CQ_BowerstoneTownLifeIntroScript@NScript@@SI...; the binding-name literal is the
// class name less its `C`): CBowerstoneIntroGuard (Alloc 0x00D24210, Main 0x00D246C0) and CBowerstoneIntroGuardToPosh (Alloc
// 0x00D24330, Main 0x00D24A40), forward-declared only.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> as the other Sunnyvale quest scripts (engine/CQ_PrisonRaceScript.h).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_BowerstoneTownLifeIntroScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BowerstoneTownLifeIntroScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00D23D20
    virtual void Main();                                                 // 0x00D23DC0
    virtual void Init();                                                 // 0x00D23D10
    virtual void OnPersist(CPersistContext&);                            // 0x00D24660
    bool ArenaFinished;                                                  // +0x48
    bool DoneGuardSpeech;                                                // +0x49
    bool PoshGuardComplete;                                              // +0x4a
    class CBowerstoneIntroGuard;                                         // Alloc 0x00D24210
    class CBowerstoneIntroGuardToPosh;                                   // Alloc 0x00D24330
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BowerstoneTownLifeIntroScript) == 0x4c);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_BOWERSTONETOWNLIFEINTROSCRIPT_H
