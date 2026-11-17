// `dynamic initializer for 'pGlobalScriptDef'' -- register the atexit destructor 0x0122AE7E of the global script definition pointer (nothing to construct: the pointer is zero in .bss)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224C99, 12 bytes (`push 0x0122AE7E ; call _atexit ; pop ecx ; ret`), in the size-optimised
//   quest-script module; VC7.1 emits it as `$E1` with the atexit destructor `$E2` 0x0122AE7E from the one definition below.
// Identity: Ego_r.pdb 0x0101B28A / FableWin.pdb 0x0400C45A "`dynamic initializer for 'pGlobalScriptDef''" (unqualified global of class
//   type), declared by engine/fable_script_def_global.h as CDefPointer<const CScriptDef> pGlobalScriptDef
//   (?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A, 0x0143E90C). agentNF60 (q10542) trial: was the descriptive
//   g_CStatic_0122ae7e_0122ae7e over a local CStatic_0122ae7e stand-in; the definition is the same as row 0x0122AE7E's.
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#include "engine/CDefPointer.h"
class CScriptDef;

CDefPointer<const CScriptDef> pGlobalScriptDef;        // 0x0143E90C
