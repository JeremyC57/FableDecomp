// CCountedPointer<NGameText::CDataBank>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCDataBank@NGameText@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403750, 53 bytes. Callees: the control block's releaser (indirect), engine free 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T = NGameText::CDataBank: catalog T NGameText::CDataBank + caller binding (00418b7a,0042f50b) + PDB fablewin ??1 symbol.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
namespace NGameText { class CDataBank; }
template CCountedPointer<NGameText::CDataBank>::~CCountedPointer();
