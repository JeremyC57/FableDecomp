// NPlayerGui::CDrawNPCName::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated non-virtual deleting destructor: the implicit destructor (Name, then the CDrawBase primitive handles) and operator delete when bit 0 is set; returns this
// Leaf: ??_GCDrawNPCName@NPlayerGui@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004430F0, 78 bytes, __thiscall `ret 4`; unreferenced in retail (like its siblings 0x00442D50 /
//   0x00442E60 / 0x00443010). Was labelled _global::Catch@0047e8cc (wrong: a plain function, no EH funclet).
// Identity (proven, two sources): Ego_d.pdb player_gui.obj lists NPlayerGui::CDrawNPCName::`scalar deleting destructor' (egod_tu.txt)
//   and FableWin.pdb names ??_GCDrawNPCName@NPlayerGui@@QAEPAXI@Z (0x01A8C7E0) in its ??_G run in the same order (last), which retail keeps (Damage 0x00442D50 < HighlightingTargetingCursor
//   0x00442E60 (landed) < MeleeActionButtonEffects 0x00443010 < NPCName 0x004430F0); the +0x28 member destructor it calls is
//   CWideString::~CWideString 0x0099B510 (reloc_bindings ??1CWideString@@QAE@XZ) = Ego_r.pdb CDrawNPCName::Name (engine/NPlayerGui_CDrawNPCName.h
//   +0x28); its body is the landed CCountedPointer<NPlayerGui::CDrawNPCName>::DeleteData 0x00443B60 (Ego_r.pdb name) with the flag test.
// Shape: the TU instantiates CCountedPointer<CDrawNPCName>::DeleteData (retail 0x00443B60, stored by Reset 0x00444D00); its
//   `delete static_cast<T*>(data)` makes VC7.1 emit this ??_G COMDAT (the deleter itself expands it inline). The implicit destructor
//   (CDrawBase IMPLICIT_DTOR opt-in) stores no vptr, as retail.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_IMPLICIT_DTOR
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/NPlayerGui_CDrawNPCName.h"
#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<NPlayerGui::CDrawNPCName>::DeleteData(void*);   // 0x00443B60
