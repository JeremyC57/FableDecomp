// CVectorMap::LowerBound(const ETCInterfaceType& key) const -- first entry of the sorted pair<ETCInterfaceType, CTCBase*> vector whose key is not less than key (binary search)
// Leaf: ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x004365B0, 60 bytes, thiscall, `ret 4`, returns the entry pointer (end() when none).
// Identity (proven): the const overload of CThing::Components' LowerBound: FableWin.pdb ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QBE (_Vector_const_iterator, STLport's const pointer in retail); called by the const component lookups (CGameScriptInterface::GiveHeroGold 0x00435038, CPlayerGui::GetButtonAlpha 0x00439B34 ...); retail keeps it as a separate byte twin of 0x0040F020 (GOTCHAS: the lander rejects routing one through the other).
// Shape (family M008, representative 0x0040F020): CVectorMap derives from the STLport vector<pair<K, V> > (the PDB layout's
//   leading padding is that base; Compare +0xC, Dirty +0xD in retail). The search is Lionhead's own: the count is
//   `_M_finish - _M_start` (pointer difference, `sar 3`), computed before the cursor, halved with the signed `/ 2`
//   (cdq/sub/sar), the key read once (edi) and `first < key` steps past the middle (`count -= half + 1`).
enum ETCInterfaceType { TC_INTERFACE_NONE = 0 };
class CTCBase;
#include "engine/CVectorMap.h"
template class CVectorMap<ETCInterfaceType, CTCBase*>;