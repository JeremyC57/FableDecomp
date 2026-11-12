// CScriptThing::SetImplementation(CCountedPointer<CScriptThing> imp) -- assign PImp (+4/+8) from the pointer passed BY VALUE (when the count blocks differ: the old block released, the object and block stored, the new block's references incremented), then the by-value argument's own destructor runs (its block released)
// Leaf: ?SetImplementation@CScriptThing@@QAEXV?$CCountedPointer@VCScriptThing@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007046D0, length 96 bytes.
// Callees: operator delete @ 0x00BFE9BC. Shape: the argument's two words read from the stack, its block in edi, `ret 8`
// (the callee cleans the by-value argument).
// Naming (agentNF31, 2026-11-12): was CMeshDataBank_007046d0::SetAnimationMemoryPool (propagated chunk label) over local
//   CCountedPointer models. Ego_r.pdb 0x00724AA0 + FableWin.pdb 0x027C7220 ?SetImplementation@CScriptThing@@QAEXV?$CCountedPointer@VCScriptThing@@@@@Z;
//   the landed caller CreateScriptThing 0x00704860 binds this symbol (reloc_bindings) and passes info.PScriptThing by value;
//   engine/CScriptThing.h declares it with PImp at +4. Body: PImp = imp (engine/CCountedPointer.h operator= / ShareData).
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

void CScriptThing::SetImplementation(CCountedPointer<CScriptThing> imp)
{
    PImp = imp;
}
