// ResetGlobalScriptDef() -- release the global script def (the def's count drops; its virtual Destroy runs at zero) and null the pointer
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?ResetGlobalScriptDef@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBEEB0, 34 bytes, `__fastcall` global with no arguments, plain `ret`
//   (PDB ?ResetGlobalScriptDef@@YIXXZ). No frame: nothing lives on the stack, so the size TU's frame pointer is not built.
// Data: the global script def pointer @ 0x0143E90C (pGlobalScriptDef in the landed quest scripts).
// Callees: the def's virtual Destroy (slot 1, +4) only.
// Shape: CDefPointer<const CScriptDef>::Reset(0) on the global (engine/fable_script_def_global.h; existing opt-ins
//   FABLE_RETAIL_CDEFPOINTER_RESET and FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF): the release block (the count read through a
//   reference on Object: `mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0`, as the atexit destructor 0x0122AE7E of the same global),
//   then Reset's unconditional store of the new (null) pointer, the size TU's `and [g],0`. Release() alone stores 0 only on the
//   non-null path (`je` to the `ret`: DIFFER).
//   agentNF60: was a local CScriptDef / CRefCounted / CDefVirtual stand-in and `CScriptDef* pGlobalScriptDef`.
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#define FABLE_RETAIL_CDEFPOINTER_RESET
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>)

void __fastcall ResetGlobalScriptDef()
{
    pGlobalScriptDef.Reset(0);
}
