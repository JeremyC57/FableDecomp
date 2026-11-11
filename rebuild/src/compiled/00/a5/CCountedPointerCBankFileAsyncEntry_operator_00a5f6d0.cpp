// CCountedPointer<CBankFileAsyncEntry>::operator=(const CCountedPointer&) -- share the other pointer: ShareData(Data, Info), return *this
// Leaf: ??4?$CCountedPointer@VCBankFileAsyncEntry@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5F6D0, 72 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Set_00a5f6d0 (stand-in name).
// T = CBankFileAsyncEntry, proven by: CEngineMap::OpenDevelopmentStaticMap 0x00B42530 assigns AsyncFileEntry through it (Ego_r CEngineMap +0x3C CCountedPointer<CBankFileAsyncEntry>), then releases its local with ~CCountedPointer<CBankFileAsyncEntry> 0x009C9FC0; FableWin.pdb ??4 name.
// Member: the in-class operator= of engine/CCountedPointer.h (loads Data / Info, then the ShareData body), emitted as its own COMDAT.
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CBankFileAsyncEntry;
template CCountedPointer<CBankFileAsyncEntry>& CCountedPointer<CBankFileAsyncEntry>::operator=(const CCountedPointer<CBankFileAsyncEntry>&);
