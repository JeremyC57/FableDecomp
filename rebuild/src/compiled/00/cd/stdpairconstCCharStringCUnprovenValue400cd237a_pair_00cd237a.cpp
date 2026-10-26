// std::pair<const CCharString, CUnprovenValue4_00cd237a>::pair(const pair&) -- copy constructor: the first member through its out-of-line copy constructor, then the 4-byte second; returns this
// Leaf: ??0?$pair@$$CBVCCharString@@VCUnprovenValue4_00cd237a@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD237A, 27 bytes, thiscall `ret 4`.
// Identity (descriptive): std::pair<const CCharString, CUnprovenValue4_00cd237a>::pair(const pair&); evidence: descriptive value type: its callers are the node factory 0x00CD2C3A (0x00CD2C50, a 0x18-byte map node) and the `_Construct` helper 0x00CD2741; the node factory serves _M_insert 0x00CD2ECB <- insert_unique 0x00CD2F60 / 0x00CD3282 <- the landed map insert(hint) 0x00CD367C <- the landed operator[] 0x00CD39B1, used only by 0x00CBFAB8 on the global map at 0x0143E910 that CScriptManager::RegisterAllScripts fills with script name -> 4-byte value (0, or 0x33FFCC5B once). The landed rows call it map<CCharString, unsigned long>, but that operator[] is 0x0042D131 (CCodeSectionManager, PDB-typed) and retail has no identical-code folding, so this map's value type is a different 4-byte type; no PDB name pins it.
// Label: the manifest's "CComboMultiplierData" is propagated (family H044: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one record type).
// Callee: 0x0099EC30 (the first member's out-of-line copy).
// Shape: `first(o.first), second(o.second)` (or the matching assignments): edi holds the source, esi `this`, the 4-byte
//   second member is one eax move after the call.
#pragma optimize("s", on)                                 // size-optimised TU (unaligned start)
class CCharString {
public:
    void* PStringData;                                     // +0x0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
};
class CUnprovenValue4_00cd237a {                          // descriptive: the map's 4-byte value type is not proven
public:
    unsigned long Value;
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
template pair<const CCharString, CUnprovenValue4_00cd237a>::pair(const pair<const CCharString, CUnprovenValue4_00cd237a>&);
} // namespace std
