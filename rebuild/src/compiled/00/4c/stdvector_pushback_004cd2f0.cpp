// std::vector<pair<ETCInterfaceType, CTCBase*> >::push_back(const value_type& x) -- STLport 4.6 _vector.h: copy-construct at the finish when
//   there is room (placement new under VC's null test, `add [ecx+4],8`), else _M_insert_overflow(finish, x, __false_type(), 1, true)
// Leaf: ?push_back@?$vector@U?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@V?$allocator@U?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@@2@@std@@QAEXABU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD2F0, 58 bytes; unreferenced in the image (an emitted COMDAT).
// Identity (NF92, q11219 binder): its only callee is 0x004CCCE0, the __false_type _M_insert_overflow of CThing::Components'
//   vector<pair<ETCInterfaceType, CTCBase*> > (re-authored by NF92 on the spelling AddTC 0x004C9D60 / CVectorMap::Insert 0x004CD400 bind), so this
//   is that vector's push_back (8-byte elements: `sar eax,3`). Explicit instantiation of stlport/fable_containers.h. Was the stand-in
//   CPairVector::push_back (an SPair8 model, binding a made-up __true_type overload). Descriptive-by-library (STLport).
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_containers.h"
class CTCBase;
template void std::vector<std::pair<ETCInterfaceType, CTCBase*> >::push_back(const std::pair<ETCInterfaceType, CTCBase*>&);
