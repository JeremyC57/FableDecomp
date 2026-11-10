// CPersistContext::TransferVectorOfSubComponents<CMultiStaticMeshEntryDef>(std::vector<CMultiStaticMeshEntryDef>& value) -- binary persist of a vector of CMultiStaticMeshEntryDef sub-defs: a binary load reads the element count, resizes and transfers each element; any other mode writes the count, then each element's own virtual Transfer
// Leaf: ??$TransferVectorOfSubComponents@VCMultiStaticMeshEntryDef@@@CPersistContext@@QAEXAAV?$vector@VCMultiStaticMeshEntryDef@@V?$allocator@VCMultiStaticMeshEntryDef@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EDE2B, 223 bytes, thiscall `ret 4` (size-optimised def TU: ebp frame, `leave`, `dec eax ; dec eax`, `push 38h ; pop ecx ; cdq ; idiv`).
// Identity (proven): CPersistContext::TransferVectorOfSubComponents<CMultiStaticMeshEntryDef>; (1) Ego_r.pdb / FableWin.pdb hold the template instance
//   ??$TransferVectorOfSubComponents@VCMultiStaticMeshEntryDef@@@CPersistContext@@ (TLC's release persist macro drops the name argument: the body
//   checks the pooled empty key 0x0122D70E and returns `ret 4`); (2) the element stride is 0x38 (the save arm's `idiv` by 0x38 and
//   the `add reg,38h` of both loops) = sizeof(CMultiStaticMeshEntryDef), and the one-argument resize 0x004EDF0A builds a default CMultiStaticMeshEntryDef (vtable 0x012438A4);
//   (3) the caller is CMultiStaticMeshDef::Transfer 0x004EDE1B (site 0x004EDE23, Meshes).
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   std::vector<CMultiStaticMeshEntryDef>::resize(unsigned int) 0x004EDF0A (STLport's one-argument forwarder, called), the element's virtual Transfer(CPersistContext&) (CDefClassBase slot 18, +0x48).
// Shape: the shared engine/CPersistContext.h body (FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS) instantiated for CMultiStaticMeshEntryDef
//   (engine/CMultiStaticMeshEntryDef.h retail model); its `count > 0` test ahead of the load loop gives retail's inline-read registers.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMULTISTATICMESHENTRYDEF
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CMultiStaticMeshEntryDef.h"
#include "engine/CPersistContext.h"

template void CPersistContext::TransferVectorOfSubComponents<CMultiStaticMeshEntryDef>(std::vector<CMultiStaticMeshEntryDef>& value);