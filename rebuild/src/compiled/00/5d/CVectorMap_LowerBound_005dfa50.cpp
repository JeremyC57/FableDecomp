// CVectorMap::LowerBound(const EAnimComponent& key) -- first entry of the sorted pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>> vector whose key is not less than key (binary search)
// Leaf: ?LowerBound@?$CVectorMap@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@V?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@@@QAEPAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@ABW4EAnimComponent@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DFA50, 60 bytes, thiscall, `ret 4`, returns the entry pointer (end() when none).
// Identity (proven): the animation-component map lookup (CAnimationEntry::AddComponent 0x005DEBB8, the GetComponent<T> rows 0x005E0280.., CAnimationSet::TransferBinaryOut 0x005DC1A2, CAnimationEntry::TransferBinaryIn 0x005DD224 over the static maps RegisteredComponentsByType / RegisteredComponentDefaults and the entries' own maps); FableWin.pdb lists only the NON-const QAE LowerBound of CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase>, CKeyPairCompareLess<...> > (retail has one copy). The existing catalog bindings spell it const (QBE) and without the comparator argument: review at landing.
// Shape (family M008, representative 0x0040F020): CVectorMap derives from the STLport vector<pair<K, V> > (the PDB layout's
//   leading padding is that base; Compare +0xC, Dirty +0xD in retail). The search is Lionhead's own: the count is
//   `_M_finish - _M_start` (pointer difference, `sar 3`), computed before the cursor, halved with the signed `/ 2`
//   (cdq/sub/sar), the key read once (edi) and `first < key` steps past the middle (`count -= half + 1`).
enum EAnimComponent { ANIM_COMPONENT_FLAGS = 0 };
class CAnimComponentBase;
template <class T> class CIVCountedPointer {              // 4 bytes: the pointee carries the count
public:
    T* Object;
};
#include "engine/CVectorMap.h"
template class CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >;