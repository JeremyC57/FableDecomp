#pragma optimize("s",on)
// CCountedPointer::operator=(const CCountedPointer&) -- share the other pointer: ShareData(Data, Info), return *this (T = std::vector<CCharString, std::allocator<CCharString> >)
// Leaf: ??4?$CCountedPointer@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C0800, 23 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Assign_005c0800 (stand-in name).
// T = std::vector<CCharString, std::allocator<CCharString> >, proven by: NInventory::CTCInventoryBase::Copy 0x005BFB96 assigns SerialisedSelectedItems through it; it calls ShareData 0x005C058F, whose release is ReleaseData<vector<CCharString>> 0x005C0563 (Ego_r CTCInventoryBase CCountedPointer<std::vector<CCharString>> SerialisedSelectedItems); FableWin.pdb ??4 name.
// Member: the in-class operator= of engine/CCountedPointer.h (loads Data / Info, then the ShareData body), emitted as its own COMDAT.
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CCharString;
namespace std { template <class _Tp> class allocator; template <class _Tp, class _Alloc> class vector; }
template CCountedPointer<std::vector<CCharString, std::allocator<CCharString> > >& CCountedPointer<std::vector<CCharString, std::allocator<CCharString> > >::operator=(const CCountedPointer<std::vector<CCharString, std::allocator<CCharString> > >&);
