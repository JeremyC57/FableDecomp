// CCountedPointer<CBankFileAsyncEntry>::CCountedPointer(const CCountedPointer&) -- copy: take Data and Info, then add a reference
// Leaf: ??0?$CCountedPointer@VCBankFileAsyncEntry@@@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D69A0, 25 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_CopyCtor_009d69a0 (stand-in name).
// T = CBankFileAsyncEntry, proven by: CEngineMap::OpenDevelopmentStaticMap 0x00B42530 copies a CCountedPointer<CBankFileAsyncEntry> through it before assigning AsyncFileEntry (Ego_r CEngineMap +0x3C CCountedPointer<CBankFileAsyncEntry>) with operator= 0x00A5F6D0 and releasing the copy with ~CCountedPointer<CBankFileAsyncEntry> 0x009C9FC0; CEngineLandscapePatch::UpdateForeground uses the same trio; the landed caller models CCountedPointer<CBankFileAsyncEntry>; FableWin.pdb ??0 copy name.
// Member: the in-class copy constructor of engine/CCountedPointer.h (const&), emitted out of line as its own COMDAT.
#include "engine/CCountedPointer.h"
class CBankFileAsyncEntry;
template CCountedPointer<CBankFileAsyncEntry>::CCountedPointer(const CCountedPointer<CBankFileAsyncEntry>&);
