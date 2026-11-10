// CPersistContext::TransferVectorOfSubComponents<CCardDef>(std::vector<CCardDef>& value) -- binary persist of a vector of CCardDef sub-defs: a binary load reads the element count, resizes and transfers each element; any other mode writes the count, then each element's own virtual Transfer
// Leaf: ??$TransferVectorOfSubComponents@VCCardDef@@@CPersistContext@@QAEXAAV?$vector@VCCardDef@@V?$allocator@VCCardDef@@@std@@@std@@@Z
// Flags: /d2inlS40
// Inline-budget: /d2inlS40 -- the one inline-expansion decision of this row: retail expands STLport's inline vector<CCardDef>::resize(n) (= resize(n, _Tp()))
//   at the binary-load site (the default element is built in the frame, resize(n, const T&) 0x004FA1A8 is called, the element destroyed);
//   VC7.1's size-mode inliner (`#pragma optimize("s")`: expand iff cost < S, S = 9 by default) rates that forwarder above S here
//   (c2 0x1078D1AA trace: this site costs 39), so the smallest S that expands it, 40, is the flag. The 223-byte instances
//   (0x004ECF6E, 0x004ED953, 0x004EDE2B) keep the default S and call their one-argument resize COMDAT, as retail does.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FA0A9, 255 bytes, thiscall `ret 4` (size-optimised def TU: ebp frame, `leave`, `dec eax ; dec eax`, `push 44h ; pop ecx ; cdq ; idiv`).
// Identity (proven): CPersistContext::TransferVectorOfSubComponents<CCardDef>; (1) Ego_r.pdb / FableWin.pdb hold the template instance
//   ??$TransferVectorOfSubComponents@VCCardDef@@@CPersistContext@@ (TLC's release persist macro drops the name argument: the body
//   checks the pooled empty key 0x0122D70E and returns `ret 4`); (2) the element stride is 0x44 (the save arm's `idiv` by 0x44 and
//   the `add reg,44h` of both loops) = sizeof(CCardDef), and the default element vtable 0x01243A44 is .?AVCCardDef@@ (RTTI);
//   (3) the caller is CTavernGameCardBaseDef::Transfer 0x004F9A90 (site 0x004F9B59, CardPack).
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   CParentDefClassBase::CParentDefClassBase() 0x00430370, std::vector<CCardDef>::resize(unsigned int, const CCardDef&) 0x004FA1A8, CParentDefClassBase::~CParentDefClassBase() 0x00430300, the element's virtual Transfer(CPersistContext&) (CDefClassBase slot 18, +0x48).
// Shape: the shared engine/CPersistContext.h body (FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS) instantiated for CCardDef
//   (engine/CCardDef.h retail model); its `count > 0` test ahead of the load loop gives retail's inline-read registers.
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
#include "engine/CCardDef.h"
#include "engine/CPersistContext.h"

template void CPersistContext::TransferVectorOfSubComponents<CCardDef>(std::vector<CCardDef>& value);