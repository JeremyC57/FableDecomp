// CVectorMap::LowerBound(const ETCInterfaceType& key) -- first entry of the sorted pair<ETCInterfaceType, CTCBase*> vector whose key is not less than key (binary search)
// Leaf: ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040F020, 60 bytes, thiscall, `ret 4`, returns the entry pointer (end() when none).
// Identity (proven): CThing's component map (CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*>); Ego_r.pdb and FableWin.pdb both have the non-const QAE LowerBound of this instantiation (FableWin also the const QBE one, which is 0x004365B0); the hundreds of non-const CThing::GetTC/GetInterface callers (e.g. CUserProfileManager::ApplyVideoValues 0x0040ACA8, NUISystem::CManager slot 14 0x0041E9E3) and the GOTCHAS one-address-one-name rule (0x0040F020 non-const, 0x004365B0 const) fix it; the Dinkumware PDB return type (_Vector_iterator) is STLport's plain pointer in retail, as the catalog binding spells it.
// Shape (family M008, representative 0x0040F020): CVectorMap derives from the STLport vector<pair<K, V> > (the PDB layout's
//   leading padding is that base; Compare +0xC, Dirty +0xD in retail). The search is Lionhead's own: the count is
//   `_M_finish - _M_start` (pointer difference, `sar 3`), computed before the cursor, halved with the signed `/ 2`
//   (cdq/sub/sar), the key read once (edi) and `first < key` steps past the middle (`count -= half + 1`).
enum ETCInterfaceType { TC_INTERFACE_NONE = 0 };
class CTCBase;
#include "engine/CVectorMap.h"
template class CVectorMap<ETCInterfaceType, CTCBase*>;