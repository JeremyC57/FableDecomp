// CIntelligentPointer<CThing>::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the implicit ~CIntelligentPointer<CThing> expanded,
//   then operator delete when bit 0 of the flags is set; returns this
// Leaf: ??_G?$CIntelligentPointer@VCThing@@@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A9710, 30 bytes. Identity: slot 0 of vtable 0x01238C6C, RTTI CIntelligentPointer (manifest rtti-vtables.tsv, no template argument; the
//   catalog name agrees).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   operator delete 0x00BFE9BC. T: the vtable RTTI type descriptor .?AV?$CIntelligentPointer@VCThing@@@@ (the CIntelligentPointer<CThing>
//   constructors 0x004A96F0 / 0x004A9730 store it). Model: the shared engine/CIntelligentPointer.h (retail branch), as the landed
//   0x0069BAF0 / 0x0048A4F0 ??_G rows.
// Names (agentNF250, 2026-11-22, CODEXP27 L29): was a free __fastcall stand-in (no Leaf line) over a row-local model; now the member the
//   compiler emits for the shared header class, so the leaf is the ??_G spelling (no landed code referencer: the address is reached only
//   through the vtable slot).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing;
namespace {
// Fidelity: scaffolding, not retail: constructing one makes VC7.1 emit this class's vftable and its scalar deleting destructor (the
// row's leaf). Internal to this TU (anonymous namespace, so no two rows define the same harness symbol); never called.
void EmitIntelligentPointerDeletingDestructor()
{
    delete new CIntelligentPointer<CThing>;
}
}
