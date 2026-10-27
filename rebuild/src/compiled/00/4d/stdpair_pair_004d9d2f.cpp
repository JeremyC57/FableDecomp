// std::pair::pair(const CCharString& __a, const CTCBase::CRegisteredTCInfo& __b) -- (<CCharString, CTCBase::CRegisteredTCInfo>) copy both members
// Leaf: ??0?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@QAE@ABVCCharString@@ABVCRegisteredTCInfo@CTCBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D9D2F, 30 bytes, thiscall, `ret 8`.
// Identity (proven): callers Init_RegisterGameComponents 0x004EE23F (seven sites) and 0x004E6D8A build it on the stack and pass it to a vector push_back 0x004E40C3 (so `first` is not const); the second member's copy constructor 0x004D6C66 is bound ??0CRegisteredTCInfo@CTCBase@@QAE@ABV01@@Z; Ego_r.pdb ??0?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@QAE@ABVCCharString@@ABVCRegisteredTCInfo@CTCBase@@@Z.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, CTCBase::CRegisteredTCInfo copy constructor 0x004D6C66.
// Shape (family M021, representative 0x00429F9C): STLport `pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}`
//   in a size-optimised module (unaligned entry, `push [esp+8]` straight from the argument slot): `optimize("s")`.
#pragma optimize("s", on)
class CCharString {                                        // 4 bytes (PStringData)
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    void* PStringData;
};
class CTCBase {
public:
    class CRegisteredTCInfo {
    public:
        CRegisteredTCInfo(const CRegisteredTCInfo& other);   // 0x004D6C66 (copy)
        unsigned long Members[3];
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    _T1 first;
    _T2 second;
};
template struct pair<CCharString, CTCBase::CRegisteredTCInfo >;
}