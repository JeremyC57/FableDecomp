// std::vector<pair<ETCInterfaceType, CTCBase*> >::_M_insert_overflow(pos, x, __false_type, n, at_end) -- STLport 4.6 _vector.c: grow to
//   size + max(size, n) (malloc), copy the head, construct n copies of x (one in place when n == 1), copy the tail unless at_end, destroy and
//   free the old block, take the new pointers
// Flags: /Gr
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@V?$allocator@U?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@@2@@std@@IAEXPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@2@ABU32@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CCCE0, 259 bytes (/Gr template TU, as the old row).
// Identity (NF92, q11219): the protected STLport member (IAE, __false_type overload) of CThing::Components' vector<pair<ETCInterfaceType,
//   CTCBase*> > (Ego_r.pdb CThing::Components = CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> >, its CArray base a vector);
//   the ported CThing::AddTC 0x004C9D60 and the landed CVectorMap::Insert 0x004CD400 bind this address under this spelling. Explicit
//   instantiation of stlport/fable_containers.h (the 0x006C80E0 recipe). Was the stand-in FableVector_004ccce0::vector<pair<unsigned int,
//   unsigned int> > (a local std model). Descriptive-by-library (STLport; both PDBs are Dinkumware builds).
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_containers.h"
class CTCBase;
template void std::vector<std::pair<ETCInterfaceType, CTCBase*> >::_M_insert_overflow(std::pair<ETCInterfaceType, CTCBase*>*,
    const std::pair<ETCInterfaceType, CTCBase*>&, const std::__false_type&, unsigned int, bool);
