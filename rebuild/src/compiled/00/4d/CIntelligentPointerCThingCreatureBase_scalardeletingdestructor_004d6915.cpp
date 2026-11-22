// CIntelligentPointer<CThingCreatureBase>::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the implicit ~CIntelligentPointer<CThingCreatureBase> expanded,
//   then operator delete when bit 0 of the flags is set; returns this
// Leaf: ??_G?$CIntelligentPointer@VCThingCreatureBase@@@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D6915, 28 bytes. Identity: slot 0 of vtable 0x0123E7D8, RTTI CIntelligentPointer (manifest rtti-vtables.tsv, no template argument; the
//   catalog name agrees).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10, operator delete 0x00BFE9BC. T: vtable 0x0123E7D8 is
//   CIntelligentPointer<CThingCreatureBase> (the landed 0x00900647 binds ??_7?$CIntelligentPointer@VCThingCreatureBase@@@@6B@ there).
//   The size-optimised body (28 bytes, `pop ecx` for the delete argument) keeps the HEAD row's optimize("s", on).
// Names (agentNF250, 2026-11-22, CODEXP27 L32): was a free __fastcall stand-in (no Leaf line) over a row-local model; now the member the
//   compiler emits for the shared header class, so the leaf is the ??_G spelling (no landed code referencer: the address is reached only
//   through the vtable slot).
#pragma optimize("s", on)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThingCreatureBase;
namespace {
// Fidelity: scaffolding, not retail: constructing one makes VC7.1 emit this class's vftable and its scalar deleting destructor (the
// row's leaf). Internal to this TU (anonymous namespace, so no two rows define the same harness symbol); never called.
void EmitIntelligentPointerDeletingDestructor()
{
    delete new CIntelligentPointer<CThingCreatureBase>;
}
}
