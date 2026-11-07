#ifndef FABLE_ENGINE_CBANDIT_LEVEL_CUTSCENESCRIPT_H
#define FABLE_ENGINE_CBANDIT_LEVEL_CUTSCENESCRIPT_H
// Retail opt-in (chunk 837): NScript::Cbandit_level_cutsceneScript, new header (no includer before chunk 837).
// Layout: no members of its own (Ego_r.pdb has no data member for the class; the landed Alloc 0x00EFAF30 `push 48h` = retail
// CLevelScriptBase size). Base CLevelScriptBase<Cjoss_test_scriptsData> (the landed ctor 0x00EFA420 / Alloc 0x00EFAF30).
// Script vtable 0x012F69E8 (5 slots, no RTTI): 0 scalar deleting dtor 0x00EFAF70, 1 RegisterMain 0x00EFA450, 2 Main 0x00EFA4F0,
// 3 Init 0x00CBD4D0 and 4 OnPersist 0x00CBD4E0 (both CScriptBase's, not overridden). Ego_r.pdb name set
// ?RegisterMain@ / ?Main@Cbandit_level_cutsceneScript@NScript@@UAEXXZ.
// Nested entity script: Cwelcome_guy (Ego_r.pdb ?Alloc@Cwelcome_guy@Cbandit_level_cutsceneScript@NScript@@SI...; landed Alloc
// 0x00EFAA60, Main 0x00EFAB40; binding-name literal "welcome_guy"), forward-declared only.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/Cbandit_level_cutsceneScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class Cjoss_test_scriptsData;
class Cbandit_level_cutsceneScript : public CLevelScriptBase<Cjoss_test_scriptsData> {
public:
    virtual void RegisterMain();                                         // 0x00EFA450
    virtual void Main();                                                 // 0x00EFA4F0
    class Cwelcome_guy;                                                  // Alloc 0x00EFAA60
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::Cbandit_level_cutsceneScript) == 0x48);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CBANDIT_LEVEL_CUTSCENESCRIPT_H
