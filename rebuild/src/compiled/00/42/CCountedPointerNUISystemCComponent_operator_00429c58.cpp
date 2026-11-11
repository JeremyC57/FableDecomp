#pragma optimize("s",on)
// CCountedPointer::operator=(const CCountedPointer&) -- share the other pointer: ShareData(Data, Info), return *this (T = NUISystem::CComponent)
// Leaf: ??4?$CCountedPointer@VCComponent@NUISystem@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429C58, 23 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Assign_00429c58 (stand-in name).
// T = NUISystem::CComponent, proven by: 0x00535B10 binds CCountedPointer<NUISystem::CComponent>::operator=; it calls ShareData<CComponent> 0x0042920A, whose release is ReleaseData<NUISystem::CComponent> 0x004291DE (landed, PDB); callers vector<CCountedPointer<CComponent>>::insert and CTCInventoryWeapons::FinalConstructWeaponItemEntriesPC; FableWin.pdb ??4 name.
// Member: the in-class operator= of engine/CCountedPointer.h (loads Data / Info, then the ShareData body), emitted as its own COMDAT.
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NUISystem { class CComponent; }
template CCountedPointer<NUISystem::CComponent>& CCountedPointer<NUISystem::CComponent>::operator=(const CCountedPointer<NUISystem::CComponent>&);
