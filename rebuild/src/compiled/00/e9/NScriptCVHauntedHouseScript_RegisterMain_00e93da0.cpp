// NScript::CV_HauntedHouseScript::RegisterMain() -- spawn the script's "Main" thread: AddSpawnedFunction(new CSpawnedFunc<CV_HauntedHouseScript>("Main", 0, this, &CV_HauntedHouseScript::Main), "")
// Leaf: ?RegisterMain@CV_HauntedHouseScript@NScript@@UAEXXZ
// Naming wave 29 (family sfbh): engine/CSpawnedFuncBase.h now carries retail's implicit destructor and the real CCharString /
//   CCountedPointer<const NScript::CPredicateBase> members; header contract met here: engine/CCountedPointer.h included before it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E93DA0, 144 bytes, plain `ret`. Slot 1 (RegisterMain) of the script vtable @ 0x012ECB94.
// Identity: the vtable is stored by the script's Alloc, which CScriptInfoManager::RegisterScripts (0x00CD52D0) registers under
//   the name "V_HauntedHouse" (site 0x00CD8B33); script classes are C<registered name>Script in NScript (the PDB name sets hold
//   ?RegisterMain@CV_HauntedHouseScript@NScript@@UAEXXZ). Family F001 (163 byte-identical bodies) -- the manifest label of every member was
//   propagated from one body; this one is CV_HauntedHouseScript's.
// Callees: scalar operator new @ 0x00BFEA1A (0x3c bytes), CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0,
//   CSpawnedFuncBase::CSpawnedFuncBase(const CCharString&, unsigned long) @ 0x00CDD450, CScriptBase::AddSpawnedFunction @ 0x00CB7E50.
// Data: CSpawnedFunc<CV_HauntedHouseScript> vtable @ 0x012ECBA8; the thread name literal "Main" @ 0x012C2F94; the empty region name @ 0x0122D70E;
//   the member pointer is the vcall thunk ??_9@$B7AE @ 0x00CDD440 (`mov eax,[ecx]; jmp [eax+8]`: Main is virtual slot 2;
//   ICF folded every class's thunk into one body).
// Layout: CSpawnedFuncBase is 0x34 bytes (vptr + 0x30), CSpawnedFunc<T> adds the member pointer Func +0x34 and PClass +0x38
//   (0x3c); CScriptBase is retail 0x40 bytes with the virtuals ~CScriptBase, RegisterMain, Main, Init, OnPersist (PDB order;
//   the retail vtable has exactly these five slots).
// Shape: the new-expression's CCharString temporary is conditional (built only when new succeeded), so VC7.1 keeps a flag
//   in ebx and destroys the temporary at the end only if bit 0 is set; the region name is the CCharString built by the inline
//   SpawnFunction helper after the new-expression (arguments of AddSpawnedFunction would otherwise build it first).
// NF336 (2026-11-24): moved off the row-local class model onto the shared engine headers (acctB Gate 0 prerequisite). The family
//   preamble below is shared by the CV_HauntedHouseScript object-mate rows (a per-row departure is marked where it occurs).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_DECLS
#define FABLE_RETAIL_CM_UNDEADHERE_MAIN
#define FABLE_RETAIL_CM_QUESTCONTROL_MAIN_PASS26
#define FABLE_RETAIL_CHHUNDEAD_MEMBERS
#define FABLE_RETAIL_CM_HEROINSIDEHOUSE_MEMBERS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CV_HauntedHouseScript.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
void NScript::CV_HauntedHouseScript::RegisterMain()
{
    AddSpawnedFunction(new CSpawnedFunc<CV_HauntedHouseScript>("Main", 0, this, &CV_HauntedHouseScript::Main), CCharString(""));
}
