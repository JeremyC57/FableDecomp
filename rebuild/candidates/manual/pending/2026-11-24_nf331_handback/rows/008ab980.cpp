// CScriptThing::operator=(const CScriptThing& other) -- assign the other handle's implementation: release ours (destroy + free the count block at zero) unless both share one block, adopt theirs and add a reference
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AB980, 74 bytes.
// Callees: the count block's destroy function (block +4, `__fastcall` with the object at block +8 in ecx),
// operator delete @ 0x00BFE9BC.
//
// Shape: the source's two words are read into ebx/edi BEFORE the block compare (object first, then the
// block -- written in load order), the destination block is re-read after the decrement and after the
// destroy call; the stores of both words are scheduled around the null test of the new block. This is
// CCountedPointer<CScriptThing>::operator= inlined into the handle's assignment (the vptr is untouched).
// Model: CScriptThing (PDB: 0xc bytes) = CBase (vptr; virtual destructor 0x0099A2E0) +
// CCountedPointer<CScriptThing> PImp {Data +4, Info +8}; the count block (CCPPointerInfo) holds the
// references at +0, the destroy function at +4 and the object it destroys at +8 (same block as the landed
// deleting destructor 0x004AB170). The engine header flattens the vptr and the counted pointer into plain
// words, so the class is modelled locally with its member names.
// Leaf: ??4CScriptThing@@QAEAAV0@ABV0@@Z
// One definition (NF331, 2026-11-24, the operator= unit after NF326 / NF328): retail expands this assignment inline in the script TUs,
//   and 90 landed script / STLport objects define it `inline` and emit it pick-any (NF331 census of 497 objects). A strong definition here
//   is LNK2005 against every such copy (90 duplicate lines in the census link), so this row defines the same inline member and emits it
//   pick-any through the helper below (retail's 0x008AB980 is the out-of-line copy).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).

class CScriptThing;

#include "engine/CCountedPointer.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (the row's local CBase)

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }

inline CScriptThing& CScriptThing::operator=(const CScriptThing& other)
{
    PImp = other.PImp;
    return *this;
}

namespace {
// Inline-budget: scaffolding, not retail: an assignment through the inline operator= above, with inline expansion switched off for this
// one function, so this object emits ??4CScriptThing@@QAEAAV0@ABV0@@Z out of line as a pick-any COMDAT (retail's 0x008AB980, the
// out-of-line copy) like every script TU that defines the assignment inline (the nf86 / nf88 / NF326 / NF328 pattern). Internal to this
// TU; never called.
#pragma inline_depth(0)
void ReferenceScriptThingAssignment(CScriptThing& thing, const CScriptThing& other)
{
    thing = other;
}
#pragma inline_depth()
}