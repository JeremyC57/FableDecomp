// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CPlayerGui>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCPlayerGui@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00419091, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00419091`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00419091, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCPlayerGui@@@@SIXPAX@Z; stored by Reset 0x004195AF (reloc_bindings stand-in ?Reset@CPlayerGuiHolder@@QAEXPAVCPlayerGui@@@Z, caller 0x00487FB0), called at 0x00487FFE with a fresh CPlayerGui (ctor 0x0043B570, RTTI vtable .?AVCPlayerGui@@).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CRESOURCELIST
#include "engine/CPlayerGui.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CPlayerGui>::DeleteData(void*);
