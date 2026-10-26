// std::pair<const CCharString, CBankFileManager::CBankInfo>::pair(const pair& __o) -- STLport 4.6 pair copy constructor for CBankFileManager::Banks' value type: copy the key, then the CBankInfo (four CWideString paths and the RegisteredBank pointer)
// Leaf: ??0?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A9AD0, length 79 bytes, thiscall `ret 4`, returns `this`.
// Identity (sweep3, family pass 1 "invented SBankRecord"): callers are the CBankInfo map's _M_insert 0x009AB120
//   (_M_create_node inlined: 0x009AB174 / 0x009AB19D), its out-of-line _M_create_node 0x009AAA30 and STLport
//   _Construct 0x009AA560 -- the placement copy of a map node's value, so this is the value type's copy constructor.
//   The value is CCharString + {4 x CWideString, a dword}: Ego_r.pdb CBankFileManager::CBankInfo (0x14 bytes), the
//   mapped type of CBankFileManager::Banks. STLport 4.6 `pair` declares `pair(const pair<_T1,_T2>& __o) : first(__o.first),
//   second(__o.second) {}` (the "convert from pair<const T1, T2>" constructor), which VC7.1 kept out of line here; the
//   CBankInfo copy is the implicit memberwise one, inlined. The old name SNamedTextEntry_009a9ad0 (and the earlier
//   chunk label CSpecialAbilitiesDrainLifeDef::operator=) were stand-ins.
// Callees: 0x0099EC30 (CCharString::CCharString(const CCharString&), on +0), 0x0099B720
//   (CWideString::CWideString(const CWideString&), four times on +4..+0x10).
// Shape: the key copied on `this`, then the value's address formed once on both sides (`lea edi,[ebx+4]`, `add esi,4`)
//   and its four strings and trailing pointer copied through those re-based pointers, the pointer moved through eax
//   last; `this` returned from ebx.
class CCharString {
public:
    void* PStringData;                                      // +0x00 (CCharStringData*)
    CCharString(const CCharString& other);                  // 0x0099EC30
};

class CWideString {
public:
    void* PStringData;                                      // +0x00 (CWideStringData*)
    CWideString(const CWideString& other);                  // 0x0099B720
};

class CBankFile;
class CBankFileManager {
public:
    class CBankInfo {                                       // 0x14 bytes (Ego_r.pdb CBankFileManager::CBankInfo)
    public:
        CWideString DevBankFilePath;                        // +0x00
        CWideString RetailBankFilePath;                     // +0x04
        CWideString DevHeaderFilePath;                      // +0x08
        CWideString RetailHeaderFilePath;                   // +0x0C
        CBankFile*  RegisteredBank;                         // +0x10
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    // convert from pair<const T1, T2>
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

template std::pair<const CCharString, CBankFileManager::CBankInfo>::pair(const std::pair<const CCharString, CBankFileManager::CBankInfo>&);
