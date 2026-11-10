// CPersistContext::TransferVectorOfSubComponents<CEnvironmentThemeDef>(std::vector<CEnvironmentThemeDef>& value) -- binary persist of a vector of CEnvironmentThemeDef sub-defs: a binary load reads the element count, resizes and transfers each element; any other mode writes the count, then each element's own virtual Transfer
// Inline-budget: /d2inlS55 -- the one inline-expansion decision of this row: retail expands STLport's inline vector<CEnvironmentThemeDef>::resize(n) (= resize(n, _Tp()))
//   at the binary-load site (the default element is built in the frame, resize(n, const T&) 0x00476F4F is called, the element destroyed); VC7.1's
//   size-mode inliner (expand iff cost < S, S = 9 by default) keeps it a call at S9 and at the chunk-1015 S40; a compile-only sweep of
//   this TU (chunk 1018) shows S55..S89 and S98 expand it, S40..S54, S90..S97 and S130..S160 keep the call (the forwarder's cost moves with the
//   nested resize(n, x) decision), so 55 is the smallest budget that reproduces retail.
// Leaf: ??$TransferVectorOfSubComponents@VCEnvironmentThemeDef@@@CPersistContext@@QAEXAAV?$vector@VCEnvironmentThemeDef@@V?$allocator@VCEnvironmentThemeDef@@@std@@@std@@@Z
// Flags: /d2inlS55
// Retail: Fable.exe (TLC, VC7.1) @ 0x00476e3e, 273 bytes, thiscall `ret 4` (size-optimised def TU: biased frame `push ebp ; lea ebp,[esp-74h]` from `optimize("s", on)` alone (no `y` off), `dec eax ; dec eax`, the stride 0xC4 held in esi for `idiv` and both loops).
// Identity (proven): CPersistContext::TransferVectorOfSubComponents<CEnvironmentThemeDef>; (1) Ego_r.pdb / FableWin.pdb hold the template instance
//   ??$TransferVectorOfSubComponents@VCEnvironmentThemeDef@@@CPersistContext@@ (TLC's release persist macro drops the name argument: the body checks the
//   pooled empty key 0x0122D70E and returns `ret 4`); (2) the element stride is 0xC4 (the save arm's `idiv` and both loops' `add reg,esi`)
//   = sizeof(CEnvironmentThemeDef) (Ego_r.pdb minus the 4-byte retail CParentDefClassBase shrink), and the default element vtable 0x01236244 is .?AVCEnvironmentThemeDef@@ (RTTI);
//   (3) the caller is CEnvironmentThemeDaySetDef::Transfer 0x00476DC3 (call 0x00476DD1, member TimeTheme).
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   CParentDefClassBase::CParentDefClassBase() 0x00430370, std::vector<CEnvironmentThemeDef>::resize(unsigned int, const CEnvironmentThemeDef&) 0x00476F4F, CParentDefClassBase::~CParentDefClassBase() 0x00430300,
//   the element's virtual Transfer(CPersistContext&) (CDefClassBase slot 18, +0x48).
// Shape: the shared engine/CPersistContext.h body (FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS) instantiated for CEnvironmentThemeDef
//   (engine/CEnvironmentThemeDef.h retail model); the stride above 0x7F is what puts the constant in esi (no `push imm8 ; pop ecx`).
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
#include "engine/CEnvironmentThemeDef.h"
#include "engine/CPersistContext.h"

template void CPersistContext::TransferVectorOfSubComponents<CEnvironmentThemeDef>(std::vector<CEnvironmentThemeDef>& value);