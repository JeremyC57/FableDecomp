// std::_Destroy<pair<CCharString,CSymbolInfo> >(std::pair<CCharString,CSymbolMap::CSymbolInfo>* __pointer) -- STLport single-element destroy of a symbol-map entry: the inlined implicit ~pair (entry name, group name, then the key string)
// Leaf: ??$_Destroy@U?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@std@@@std@@YIXPAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FB8D0, 31 bytes, __fastcall (pointer in ecx), `jmp` tail; no call or address
//   reference (an unreferenced COMDAT, kept by retail's linker).
// Identity (proven): (1) TU: the NAnimationEvents TU (CManager::AddEventToAnim 0x006FA3F0 .. the AnimToExtraEventsMap helpers
//   0x006FB830 / 0x006FB8A0 / 0x006FB8F0 / 0x006FB9C0); the event manager owns a CSymbolMap (FableWin.pdb
//   CManager::GetEventSymbolMap returns `const CSymbolMap&`; CFillEvents / CFillEventNames take one), whose CVectorMap
//   SLONGMap holds pair<CCharString, CSymbolMap::CSymbolInfo>. (2) Placement: Ego_r's Dinkumware
//   `_Dest_val<allocator<pair<CCharString,CSymbolInfo>>, pair<..>>` (donor 0x0042A6B0, __fastcall YI) is STLport 4.6's
//   `_Destroy(_Tp*)` (stl/_construct.h); this body sits among the TU's STLport helpers (between __destroy_aux 0x006FB8A0
//   and swap 0x006FB8F0), while its implicit-`??1` twin 0x006FB460 sits with the class special members (after the
//   pair<unsigned long, CSmallVector<CEvent,32>> `??_G` 0x006FB430), the order chunk 656 proved for TeleportData
//   (0x00CD4550 ~T, 0x00CD4FA0 _Destroy<T>). __fastcall: the TU is /Gr (the neighbouring __destroy_aux is `ret 4` fastcall).
// Layout: Ego_r.pdb CSymbolMap::CSymbolInfo (0xc) = CCharString GroupName +0, EntryName +4, long Val +8; pair 0x10.
// Callees: CCharString::~CCharString 0x0099EAE0 (x3).
// Shape: STLport `__pointer->~_Tp()` with the implicit ~pair and ~CSymbolInfo inlined: `&second` in edi (EntryName
//   [edi+4], GroupName [edi]), the key string is the tail jmp. Explicit instantiation emits the inline template's COMDAT.
#include "fable_string.h"      // CCharString::~CCharString @ 0x0099EAE0

class CSymbolMap {
public:
    class CSymbolInfo {                                  // Ego_r.pdb, 0xc bytes
    public:
        CCharString GroupName;                           // +0x00
        CCharString EntryName;                           // +0x04
        long        Val;                                 // +0x08
    };
};

namespace std {
template <class _T1, class _T2>
struct pair {                                            // STLport 4.6 stl/_pair.h
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2>
    pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
};
template <class _Tp>
inline void _Destroy(_Tp* __pointer)                     // STLport 4.6 stl/_construct.h
{
    __pointer;
    __pointer->~_Tp();
}
}

template void std::_Destroy<std::pair<CCharString, CSymbolMap::CSymbolInfo> >(std::pair<CCharString, CSymbolMap::CSymbolInfo>*);