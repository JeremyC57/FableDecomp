#pragma optimize("s",on)
// CCountedPointer<NUISystem::CListArrow>::ReleaseData() -- release one counted pointer
// Leaf: ?ReleaseData@?$CCountedPointer@VCListArrow@NUISystem@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00535EBB, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the header's FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA body, as the landed
// ReleaseData rows 0x00403750 / 0x004190B6). Retail emits this body under both ~CCountedPointer<T> and ReleaseData<T>.
// Name: the binding table already binds this PDB name here (a landed caller); the self now spells it (one address, one name).
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NUISystem { class CListArrow; }
template void CCountedPointer<NUISystem::CListArrow>::ReleaseData();
