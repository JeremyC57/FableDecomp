// std::pair::operator=(const pair& o) -- (pair<CCharString, CTCBase::CRegisteredTCInfo>) STLport implicit assignment: the first member through CCharString's assignment, then the second through CRegisteredTCInfo's out-of-line assignment; returns this
// Leaf: ??4?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@QAEAAU01@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA56A, 33 bytes, thiscall `ret 4` (a size-optimised /Os module: unaligned entry).
// Identity (proven): std::pair<CCharString, CTCBase::CRegisteredTCInfo>::operator=(const pair&); evidence: the 16-byte pair's members are a CCharString (copy 0x0099EC30 / assignment 0x0099EFB0) and a 12-byte record whose copy 0x004D6C66 / assignment 0x004D78C5 move two dwords and then a CCharString at +8 -- Ego_r.pdb CTCBase::CRegisteredTCInfo {AllocFunc, DriverType, CCharString Name}; the landed std::swap<pair<CCharString, CTCBase::CRegisteredTCInfo> > 0x004DA6FC and the landed sort/heap helpers of the thing-component registry (CVectorMap<CCharString, CTCBase::CRegisteredTCInfo, CKeyPairCompareLess<...> > in Ego_r.pdb, a vector of these pairs) call it (assignments: swap 0x004DA714 / 0x004DA71F, the landed __copy 0x004DBDE1, __unguarded_linear_insert 0x004DD80B, __push_heap 0x004DD875, __adjust_heap 0x004DFDBB)
// Label: the manifest's "pair<CCharString,CCharString><CCharString,CCharString>" is propagated (family J041: five byte-similar pair bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one pair type).
// Callees: CCharString::operator=(const CCharString&) 0x0099EFB0 (PDB ??4CCharString@@QAEABV0@ABV0@@Z), CTCBase::CRegisteredTCInfo::operator=(const CRegisteredTCInfo&) 0x004D78C5 (PDB ??4CRegisteredTCInfo@CTCBase@@QAEAAV01@ABV01@@Z).
// Shape: STLport 4.6 _pair.h has no assignment; the implicit one assigns `first` then `second` (spelled out here): edi holds the source, esi `this`; the second member sits at +4.
#pragma optimize("s", on)                                 // size-optimised TU (unaligned start)
#include <stddef.h>
class CCharString {                                      // 4 bytes (Ego_r)
public:
    void* PStringData;                                     // +0x0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0 (PDB ??4CCharString@@QAEABV0@ABV0@@Z)
};
#define CTCBase CTCBasePdb
#include "engine/CTCBase.h"
#undef CTCBase
class CThing;
class CTCBase : public CTCBasePdb {                        // engine/CTCBase.h is data-only: the nested record is added here
public:
    class CRegisteredTCInfo {                              // Ego_r.pdb, 12 bytes
    public:
        CTCBase*      (__fastcall* AllocFunc)(CThing&);    // +0x0 (PDB ??0CRegisteredTCInfo@CTCBase@@QAE@P6IPAV1@AAVCThing@@@ZKABVCCharString@@@Z)
        unsigned long DriverType;                          // +0x4
        CCharString   Name;                                // +0x8
        CRegisteredTCInfo(const CRegisteredTCInfo& other);              // 0x004D6C66 (PDB ??0CRegisteredTCInfo@CTCBase@@QAE@ABV01@@Z)
        CRegisteredTCInfo& operator=(const CRegisteredTCInfo& other);   // 0x004D78C5 (PDB ??4CRegisteredTCInfo@CTCBase@@QAEAAV01@ABV01@@Z)
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x0
    _T2 second;                                            // +0x4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
    pair& operator=(const pair& __o) { first = __o.first; second = __o.second; return *this; }   // the implicit assignment, spelled out
};
template pair<CCharString, CTCBase::CRegisteredTCInfo>& pair<CCharString, CTCBase::CRegisteredTCInfo>::operator=(const pair<CCharString, CTCBase::CRegisteredTCInfo>&);
} // namespace std