// std::vector<CConversation>::resize(size_type n, const CConversation& x) -- STLport 4.6's own body: erase the tail through
// erase(first, last) 0x00F337A0 when n is below the size, else _M_fill_insert(end(), n - size(), x) 0x00F33810.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F33730, oracle length 105 bytes.
// Name: ?resize@?$vector@VCConversation@@V?$allocator@VCConversation@@@std@@@std@@QAEXIABVCConversation@@@Z (STLport takes the
// value by const reference: the landed resize<char> 0x004122D0 / resize<long> 0x004428F0 spelling; Ego_r.pdb / FableWin.pdb name
// vector<CConversation> but come from a later library). The binary-load arm of TransferVectorOfSubComponents<CConversation>
// 0x00F334E0 calls it with (count, &default) after the inline resize(n) (RELOCATION_MATCH, chunk 1018); element 0x5C = Ego_r.pdb
// sizeof(CConversation). The size is a signed element count (`cdq / idiv` by 0x5C) compared unsigned.
// Leaf: ?resize@?$vector@VCConversation@@
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "engine/CConversation.h"

template void std::vector<CConversation>::resize(size_type n, const CConversation& x);
