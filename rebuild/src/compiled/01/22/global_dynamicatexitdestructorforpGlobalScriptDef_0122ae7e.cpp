// `dynamic atexit destructor for 'pGlobalScriptDef'' -- release the global script definition pointer (drop its reference, destroy the def on the last one) and clear it
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0122AE7E, 34 bytes, in `.text$yd` (size-optimised quest-script module:
//   no 16-byte alignment, `and dword ptr [g],0`); its dynamic initializer @ 0x01224C99 only registers it with atexit (the
//   pointer is zero in .bss). VC7.1 names the pair `$E1` / `$E2`.
// Identity: Ego_r.pdb 0x0101B28A / 0x0101EA0E and FableWin.pdb 0x0400C45A / 0x04012CA6 list "`dynamic initializer for
//   'pGlobalScriptDef''" and "`dynamic atexit destructor for 'pGlobalScriptDef''": an unqualified global of class type (it has a
//   destructor), declared by engine/fable_script_def_global.h as CDefPointer<const CScriptDef> pGlobalScriptDef
//   (?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A, 0x0143E90C). agentNF57: the global was defined here as the stand-in
//   NGlobalScriptDefs::pGlobalScriptDef over a local CDefPointer / CScriptDef model; the catalog name
//   NGlobalScriptDefs::DestroyPGlobalScriptDef keeps its descriptive namespace (the PDB gives the bare global name).
// Callee: the def's virtual slot 1 (+4, CDefPointeeBase::Delete) when the reference count at +4 reaches zero.
// Shape: the shared engine/CDefPointer.h destructor with the existing opt-in FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF (the count read
//   through a reference on Object: `add eax,4 ; dec [eax] ; cmp [eax],0`, the shape of the landed atexit destructor 0x01228B70 of
//   CPlayerGui::PGuiDef), under `#pragma optimize("s", on)` (retail's `and [g],0`). The destructor needs only CDefPointeeBase, so
//   CScriptDef stays a forward declaration (engine/CScriptDef.h is not needed for the definition of the global).
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#include "engine/CDefPointer.h"
class CScriptDef;

CDefPointer<const CScriptDef> pGlobalScriptDef;        // 0x0143E90C
