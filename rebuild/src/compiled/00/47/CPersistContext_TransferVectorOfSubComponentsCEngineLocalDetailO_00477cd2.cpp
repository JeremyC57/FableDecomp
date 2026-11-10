// CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailObjectDef>(std::vector<CEngineLocalDetailObjectDef>& value) -- binary persist of a vector of CEngineLocalDetailObjectDef sub-defs: a binary load reads the element count, resizes and transfers each element; any other mode writes the count, then each element's own virtual Transfer
// Inline-budget: /d2inlS98 -- the one inline-expansion decision of this row: retail expands STLport's inline vector<CEngineLocalDetailObjectDef>::resize(n) (= resize(n, _Tp()))
//   at the binary-load site (the default element is built in the frame, resize(n, const T&) 0x00477DEA is called, the element destroyed); VC7.1's
//   size-mode inliner (expand iff cost < S, S = 9 by default) keeps it a call at S9 and at the chunk-1015 S40; a compile-only sweep of
//   this TU (chunk 1018) shows S98..S120 expand it, S40..S96 and S140 keep the call (the forwarder's cost moves with the
//   nested resize(n, x) decision), so 98 is the smallest budget that reproduces retail.
// Leaf: ??$TransferVectorOfSubComponents@VCEngineLocalDetailObjectDef@@@CPersistContext@@QAEXAAV?$vector@VCEngineLocalDetailObjectDef@@V?$allocator@VCEngineLocalDetailObjectDef@@@std@@@std@@@Z
// Flags: /d2inlS98
// Retail: Fable.exe (TLC, VC7.1) @ 0x00477cd2, 280 bytes, thiscall `ret 4` (size-optimised def TU: `dec eax ; dec eax`, the stride 0x88 held in esi for `idiv` and both loops).
// Identity (proven): CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailObjectDef>; (1) Ego_r.pdb / FableWin.pdb hold the template instance
//   ??$TransferVectorOfSubComponents@VCEngineLocalDetailObjectDef@@@CPersistContext@@ (TLC's release persist macro drops the name argument: the body checks the
//   pooled empty key 0x0122D70E and returns `ret 4`); (2) the element stride is 0x88 (the save arm's `idiv` and both loops' `add reg,esi`)
//   = sizeof(CEngineLocalDetailObjectDef) (Ego_r.pdb minus the 4-byte retail CParentDefClassBase shrink), and the default element vtable 0x0123634C is .?AVCEngineLocalDetailObjectDef@@ (RTTI);
//   (3) the caller is CEngineLocalDetailLayerDef::Transfer 0x00477CB1 (member Objects).
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   CParentDefClassBase::CParentDefClassBase() 0x00430370, std::vector<CEngineLocalDetailObjectDef>::resize(unsigned int, const CEngineLocalDetailObjectDef&) 0x00477DEA, CParentDefClassBase::~CParentDefClassBase() 0x00430300,
//   the element's virtual Transfer(CPersistContext&) (CDefClassBase slot 18, +0x48).
// Shape: the shared engine/CPersistContext.h body (FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS) instantiated for CEngineLocalDetailObjectDef
//   (engine/CEngineLocalDetailObjectDef.h retail model); the stride above 0x7F is what puts the constant in esi (no `push imm8 ; pop ecx`).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CEngineLocalDetailObjectDef.h"
#include "engine/CPersistContext.h"

template void CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailObjectDef>(std::vector<CEngineLocalDetailObjectDef>& value);