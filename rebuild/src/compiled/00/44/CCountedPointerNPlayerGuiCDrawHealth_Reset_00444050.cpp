// CCountedPointer::Reset(T*) -- release the old pointee, take the new one and give it a fresh count block (T = NPlayerGui::CDrawHealth)
// Leaf: ?Reset@?$CCountedPointer@VCDrawHealth@NPlayerGui@@@@QAEXPAVCDrawHealth@NPlayerGui@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00444050, 103 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Set_00444050 (stand-in name).
// T = NPlayerGui::CDrawHealth, proven by: the stored deleter 0x00443500 calls NPlayerGui::CDrawHealth::~CDrawHealth 0x00441800 then operator delete (DeleteData<CDrawHealth>); CPlayerGui::CPlayerGui 0x0043B570 binds Reset<NPlayerGui::CDrawHealth> (PDrawHealth: Ego_r CPlayerGui CCountedPointer<NPlayerGui::CDrawHealth>); FableWin.pdb Reset name.
// Member: the in-class Reset of engine/CCountedPointer.h (ReleaseData expanded; its Data store dies under Data = data), own COMDAT.
// The count block stores CCountedPointer<T>::DeleteData (its own COMDAT; T is only forward-declared here).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NPlayerGui { class CDrawHealth; }
template void CCountedPointer<NPlayerGui::CDrawHealth>::Reset(NPlayerGui::CDrawHealth*);
