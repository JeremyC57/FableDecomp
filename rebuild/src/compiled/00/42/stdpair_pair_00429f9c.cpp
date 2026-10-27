// std::pair::pair(const CCharString& __a, const CCountedPointer<NUISystem::CComponent>& __b) -- (<const CCharString, CCountedPointer<NUISystem::CComponent>>) copy both members
// Leaf: ??0?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@QAE@ABVCCharString@@ABV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429F9C, 30 bytes, thiscall, `ret 8`.
// Identity (proven): its only caller is std::map<CCharString, CCountedPointer<NUISystem::CComponent> >::operator[] 0x0042D45D (the landed src; `insert(it, value_type(key, T()))`); the second member's copy constructor 0x00428928 is bound ??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@ABV0@@Z; Ego_r.pdb has ??0?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@ (that map's value_type).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, CCountedPointer<NUISystem::CComponent> copy constructor 0x00428928.
// Shape (family M021, representative 0x00429F9C): STLport `pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}`
//   in a size-optimised module (unaligned entry, `push [esp+8]` straight from the argument slot): `optimize("s")`.
#pragma optimize("s", on)
class CCharString {                                        // 4 bytes (PStringData)
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    void* PStringData;
};
template <class T> class CCountedPointer {
public:
    CCountedPointer(const CCountedPointer& other);        // 0x00428928 (copy)
    T* Data;
    void* Info;
};
namespace NUISystem { class CComponent; }
namespace std {
template <class _T1, class _T2> struct pair {
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    _T1 first;
    _T2 second;
};
template struct pair<const CCharString, CCountedPointer<NUISystem::CComponent> >;
}