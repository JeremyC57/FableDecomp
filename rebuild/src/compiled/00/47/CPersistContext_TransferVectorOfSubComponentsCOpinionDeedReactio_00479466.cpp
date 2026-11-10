// CPersistContext::TransferVectorOfSubComponents<COpinionDeedReactionDef>(std::vector<COpinionDeedReactionDef>& value) -- binary persist of a vector of COpinionDeedReactionDef sub-defs: a binary load reads the element count, resizes and transfers each element; any other mode writes the count, then each element's own virtual Transfer
// Leaf: ??$TransferVectorOfSubComponents@VCOpinionDeedReactionDef@@@CPersistContext@@QAEXAAV?$vector@VCOpinionDeedReactionDef@@V?$allocator@VCOpinionDeedReactionDef@@@std@@@std@@@Z
// Flags: /d2inlS30
// Inline-budget: /d2inlS30 -- the one inline-expansion decision of this row: retail expands STLport's inline vector<COpinionDeedReactionDef>::resize(n) (= resize(n, _Tp()))
//   at the binary-load site (the default element is built in the frame, resize(n, const T&) 0x00479595 is called, the element destroyed);
//   VC7.1's size-mode inliner (`#pragma optimize("s")`: expand iff cost < S, S = 9 by default) rates that forwarder above S here
//   (c2 0x1078D1AA trace: this site costs 29), so the smallest S that expands it, 30, is the flag. The 223-byte instances
//   (0x004ECF6E, 0x004ED953, 0x004EDE2B) keep the default S and call their one-argument resize COMDAT, as retail does.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00479466, 245 bytes, thiscall `ret 4` (size-optimised def TU: ebp frame, `leave`, `dec eax ; dec eax`, `push 50h ; pop ecx ; cdq ; idiv`).
// Identity (proven): CPersistContext::TransferVectorOfSubComponents<COpinionDeedReactionDef>; (1) Ego_r.pdb / FableWin.pdb hold the template instance
//   ??$TransferVectorOfSubComponents@VCOpinionDeedReactionDef@@@CPersistContext@@ (TLC's release persist macro drops the name argument: the body
//   checks the pooled empty key 0x0122D70E and returns `ret 4`); (2) the element stride is 0x50 (the save arm's `idiv` by 0x50 and
//   the `add reg,50h` of both loops) = sizeof(COpinionDeedReactionDef), and the out-of-line ctor / dtor 0x00479574 / 0x0047955B are COpinionDeedReactionDef's (its vtable 0x012361DC);
//   (3) the caller is 0x00478F36 (the opinion-reaction def Transfer) and an address reference at 0x0054FC06.
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   COpinionDeedReactionDef::COpinionDeedReactionDef() 0x00479574, std::vector<COpinionDeedReactionDef>::resize(unsigned int, const COpinionDeedReactionDef&) 0x00479595, COpinionDeedReactionDef::~COpinionDeedReactionDef() 0x0047955B, the element's virtual Transfer(CPersistContext&) (CDefClassBase slot 18, +0x48).
// Shape: the shared engine/CPersistContext.h body (FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS) instantiated for COpinionDeedReactionDef
//   (engine/COpinionDeedReactionDef.h retail model); its `count > 0` test ahead of the load loop gives retail's inline-read registers.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/COpinionDeedReactionDef.h"
#include "engine/CPersistContext.h"

template void CPersistContext::TransferVectorOfSubComponents<COpinionDeedReactionDef>(std::vector<COpinionDeedReactionDef>& value);