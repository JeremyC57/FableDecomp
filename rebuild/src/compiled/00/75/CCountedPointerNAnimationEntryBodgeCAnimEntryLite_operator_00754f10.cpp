// CCountedPointer::operator=(const CCountedPointer&) -- share the other pointer: ShareData(Data, Info), return *this (T = NAnimationEntryBodge::CAnimEntryLite)
// Leaf: ??4?$CCountedPointer@VCAnimEntryLite@NAnimationEntryBodge@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00754F10, 72 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Set_00754f10 (stand-in name).
// T = NAnimationEntryBodge::CAnimEntryLite, proven by: 0x008621A0 binds CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>::operator= (0x00752640 binds the same T with a wrong U key); callers CTCLook::InitialiseHeadAnims / CModeStandBase::OnActivate; FableWin.pdb ??4?$CCountedPointer@VCAnimEntryLite@NAnimationEntryBodge@@ name.
// Member: the in-class operator= of engine/CCountedPointer.h (loads Data / Info, then the ShareData body), emitted as its own COMDAT.
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NAnimationEntryBodge { class CAnimEntryLite; }
template CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>& CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>::operator=(const CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>&);
