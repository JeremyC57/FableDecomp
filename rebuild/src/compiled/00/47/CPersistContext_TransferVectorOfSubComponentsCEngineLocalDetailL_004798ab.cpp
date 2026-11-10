// CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailLayerDef>(std::vector<CEngineLocalDetailLayerDef>& value) -- binary persist of a vector of CEngineLocalDetailLayerDef sub-defs: a binary load reads the element count, resizes and transfers each element; any other mode writes the count, then each element's own virtual Transfer
// Leaf: ??$TransferVectorOfSubComponents@VCEngineLocalDetailLayerDef@@@CPersistContext@@QAEXAAV?$vector@VCEngineLocalDetailLayerDef@@V?$allocator@VCEngineLocalDetailLayerDef@@@std@@@std@@@Z
// Flags: /d2inlS30
// Inline-budget: /d2inlS30 -- the one inline-expansion decision of this row: retail expands STLport's inline vector<CEngineLocalDetailLayerDef>::resize(n) (= resize(n, _Tp()))
//   at the binary-load site (the default element is built in the frame, resize(n, const T&) 0x004799C6 is called, the element destroyed);
//   VC7.1's size-mode inliner (`#pragma optimize("s")`: expand iff cost < S, S = 9 by default) rates that forwarder above S here
//   (c2 0x1078D1AA trace: this site costs 29), so the smallest S that expands it, 30, is the flag. The 223-byte instances
//   (0x004ECF6E, 0x004ED953, 0x004EDE2B) keep the default S and call their one-argument resize COMDAT, as retail does.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004798AB, 245 bytes, thiscall `ret 4` (size-optimised def TU: ebp frame, `leave`, `dec eax ; dec eax`, `push 54h ; pop ecx ; cdq ; idiv`).
// Identity (proven): CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailLayerDef>; (1) Ego_r.pdb / FableWin.pdb hold the template instance
//   ??$TransferVectorOfSubComponents@VCEngineLocalDetailLayerDef@@@CPersistContext@@ (TLC's release persist macro drops the name argument: the body
//   checks the pooled empty key 0x0122D70E and returns `ret 4`); (2) the element stride is 0x54 (the save arm's `idiv` by 0x54 and
//   the `add reg,54h` of both loops) = sizeof(CEngineLocalDetailLayerDef), and the out-of-line ctor / dtor 0x004799A0 / 0x0047747C are CEngineLocalDetailLayerDef's (its vtable 0x012362E4);
//   (3) the caller is CEngineLocalDetailGeneratorDef::Transfer 0x0047989B (site 0x004798A3, Layers).
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   CEngineLocalDetailLayerDef::CEngineLocalDetailLayerDef() 0x004799A0, std::vector<CEngineLocalDetailLayerDef>::resize(unsigned int, const CEngineLocalDetailLayerDef&) 0x004799C6, CEngineLocalDetailLayerDef::~CEngineLocalDetailLayerDef() 0x0047747C, the element's virtual Transfer(CPersistContext&) (CDefClassBase slot 18, +0x48).
// Shape: the shared engine/CPersistContext.h body (FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS) instantiated for CEngineLocalDetailLayerDef
//   (engine/CEngineLocalDetailLayerDef.h retail model); its `count > 0` test ahead of the load loop gives retail's inline-read registers.
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
#include "engine/CEngineLocalDetailLayerDef.h"
#include "engine/CPersistContext.h"

template void CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailLayerDef>(std::vector<CEngineLocalDetailLayerDef>& value);