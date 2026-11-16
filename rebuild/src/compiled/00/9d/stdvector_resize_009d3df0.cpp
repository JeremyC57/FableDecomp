// std::vector<CBankFileEntryUpdateData*>::resize(size_type, CBankFileEntryUpdateData* const&) -- STLport resize: erase the surplus
//   (an inline memmove move-down) when shrinking, else fill-insert copies of the value at the end (_M_fill_insert 0x009D3300)
// Leaf: ?resize@?$vector@PAVCBankFileEntryUpdateData@@V?$allocator@PAVCBankFileEntryUpdateData@@@std@@@std@@QAEXIABQAVCBankFileEntryUpdateData@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D3DF0.
// Name: descriptive-by-library (STLport 4.6 const& form; FableWin.pdb 0x02F9E960's Dinkumware form takes the value): the landed binder
//   0x009CE5E0 CBankFile::AddEntryMultipleFiles (UpdateData.resize(n, 0)) binds this symbol. Emitted from the shared
//   stlport/fable_containers.h (NF33 2026-11-16; was the stand-in CRetailVector_009d3df0::Resize_CRetailVector_009d3df0).
#include "stlport/fable_containers.h"
class CBankFileEntryUpdateData;
template void std::vector<CBankFileEntryUpdateData*, std::allocator<CBankFileEntryUpdateData*> >::resize(std::vector<CBankFileEntryUpdateData*, std::allocator<CBankFileEntryUpdateData*> >::size_type, CBankFileEntryUpdateData* const&);
