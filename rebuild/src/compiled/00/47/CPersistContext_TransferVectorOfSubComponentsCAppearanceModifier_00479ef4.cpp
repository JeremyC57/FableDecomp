// CPersistContext::TransferVectorOfSubComponents<CAppearanceModifierScalingDef>(std::vector<CAppearanceModifierScalingDef>& value) -- binary persist of a vector of CAppearanceModifierScalingDef sub-defs: a binary load reads the element count, resizes and transfers each element; any other mode writes the count, then each element's own virtual Transfer
// Leaf: ??$TransferVectorOfSubComponents@VCAppearanceModifierScalingDef@@@CPersistContext@@QAEXAAV?$vector@VCAppearanceModifierScalingDef@@V?$allocator@VCAppearanceModifierScalingDef@@@std@@@std@@@Z
// Flags: /d2inlS40
// Inline-budget: /d2inlS40 -- the one inline-expansion decision of this row: retail expands STLport's inline vector<CAppearanceModifierScalingDef>::resize(n) (= resize(n, _Tp()))
//   at the binary-load site (the default element is built in the frame, resize(n, const T&) 0x00479FF3 is called, the element destroyed);
//   VC7.1's size-mode inliner (`#pragma optimize("s")`: expand iff cost < S, S = 9 by default) rates that forwarder above S here
//   (c2 0x1078D1AA trace: this site costs 39), so the smallest S that expands it, 40, is the flag. The 223-byte instances
//   (0x004ECF6E, 0x004ED953, 0x004EDE2B) keep the default S and call their one-argument resize COMDAT, as retail does.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00479EF4, 255 bytes, thiscall `ret 4` (size-optimised def TU: ebp frame, `leave`, `dec eax ; dec eax`, `push 48h ; pop ecx ; cdq ; idiv`).
// Identity (proven): CPersistContext::TransferVectorOfSubComponents<CAppearanceModifierScalingDef>; (1) Ego_r.pdb / FableWin.pdb hold the template instance
//   ??$TransferVectorOfSubComponents@VCAppearanceModifierScalingDef@@@CPersistContext@@ (TLC's release persist macro drops the name argument: the body
//   checks the pooled empty key 0x0122D70E and returns `ret 4`); (2) the element stride is 0x48 (the save arm's `idiv` by 0x48 and
//   the `add reg,48h` of both loops) = sizeof(CAppearanceModifierScalingDef), and the default element vtable 0x01236434 is .?AVCAppearanceModifierScalingDef@@ (RTTI);
//   (3) the caller is CHeroStatsDef::Transfer 0x00479CBA (site 0x00479D57, AppearanceScaling).
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   CParentDefClassBase::CParentDefClassBase() 0x00430370, std::vector<CAppearanceModifierScalingDef>::resize(unsigned int, const CAppearanceModifierScalingDef&) 0x00479FF3, CParentDefClassBase::~CParentDefClassBase() 0x00430300, the element's virtual Transfer(CPersistContext&) (CDefClassBase slot 18, +0x48).
// Shape: the shared engine/CPersistContext.h body (FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS) instantiated for CAppearanceModifierScalingDef
//   (engine/CAppearanceModifierScalingDef.h retail model); its `count > 0` test ahead of the load loop gives retail's inline-read registers.
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
#include "engine/CAppearanceModifierScalingDef.h"
#include "engine/CPersistContext.h"

template void CPersistContext::TransferVectorOfSubComponents<CAppearanceModifierScalingDef>(std::vector<CAppearanceModifierScalingDef>& value);