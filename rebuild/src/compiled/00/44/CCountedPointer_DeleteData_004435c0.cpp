// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<NPlayerGui::CDrawScreenFade>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded; the implicit destructor leaves only the PrimitiveHandles vector's teardown
// Leaf: ?DeleteData@?$CCountedPointer@VCDrawScreenFade@NPlayerGui@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004435C0, 63 bytes, a static __fastcall member (data in ecx), plain `ret`. Callees: each handle's virtual
//   destructor (vtable slot 0, flag 0) over [+0x1C, +0x20) in 8-byte steps, free 0x00BFEA14 (the STLport malloc allocator's block),
//   operator delete 0x00BFE9BC. Only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x004435C0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_004435c0, the behavioural PolymorphicRangeCleanup_004435c0 over invented CleanupOwner /
//   CleanupEntry stand-ins): FableWin.pdb ?DeleteData@?$CCountedPointer@VCDrawScreenFade@NPlayerGui@@@@SIXPAX@Z; its only reference stores it in
//   CCountedPointer<NPlayerGui::CDrawScreenFade>::Reset 0x00444210 (reloc_bindings ?Reset@?$CCountedPointer@VCDrawScreenFade@NPlayerGui@@@@QAEXPAVCDrawScreenFade@NPlayerGui@@@Z,
//   bound by the landed CPlayerGui::CPlayerGui 0x0043B570); Ego_r.pdb CPlayerGui::PDrawScreenFade is CCountedPointer<NPlayerGui::CDrawScreenFade>.
// Model: NPlayerGui::CDrawBase from engine/NPlayerGui_CDrawBase.h (PrimitiveHandles = vector<CEnginePrimitiveHandle>, 8-byte polymorphic
//   elements) with the IMPLICIT_DTOR opt-in (retail stores no vptr: no user destructor is expanded); the derived class adds only the
//   Ego_r.pdb members after the base (none need destruction).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_IMPLICIT_DTOR
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/CEnginePrimitiveHandle.h"
namespace NPlayerGui {
class CDrawScreenFade : public CDrawBase {
public:
    bool DrawnFullFadeOut;                                     // +0x28 (Ego_r.pdb +0x2C)
};
}

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<NPlayerGui::CDrawScreenFade>::DeleteData(void*);
