// CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::Sort() -- std::sort the CArray (vector) range with the
//   stored comparator (0x004CD5D0, __fastcall: start in ecx, finish in edx, Compare +0x0C by value), then clear Dirty (+0x0D)
// Leaf: ?Sort@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD650, 25 bytes.
// Identity (NF92, q11220 binder): FableWin.pdb 0x01CFA5F0 ?Sort@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@..@@@@QAEXXZ
//   (CThing::Components' map type, Ego_r.pdb); the body is CVectorMap.h's Sort (the NF91 0x00A73C40 recipe) and calls the re-authored
//   std::sort 0x004CD5D0. Was the stand-in CComponentMap::Resort (a CGuiVarTransferBool model, binding ?CGuiVarTransferBool_Helper@@YIXPAXH_N@Z).
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CVECTORMAP_SORT
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_containers.h"
#include "engine/CVectorMap.h"
class CTCBase;
template void CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::Sort();
