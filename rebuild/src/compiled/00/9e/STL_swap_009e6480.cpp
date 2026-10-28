// std::swap(pair<CCharString,CSymbolMap::CSymbolInfo>& a, pair<CCharString,CSymbolMap::CSymbolInfo>& b) -- exchange two symbol-map entries (key string + group/entry names + value) through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E6480, 168 bytes (+ int3 padding), /Gr `__fastcall` (a in ecx -> edi, b in edx
//   -> ebx), plain `ret`.
// Identity: the sort TU of CSymbolMap's sorted vector map (Ego_r.pdb CVectorMap<CCharString, CSymbolMap::CSymbolInfo,
//   CCompareString<CSymbolInfo> >, 16-byte std::pair elements); the neighbours are the landed pair copy constructor
//   0x009E6050, pair::operator= 0x009E6380 and the pair copy/copy_backward helpers 0x009E6410 / 0x009E6600. Callers: the
//   out-of-line iter_swap 0x009E66E0 (a bare `jmp`) and the partition step 0x009E6CD9. The catalog's
//   "swap<CSymbolMap::CSymbolInfo>" names the second member only.
// Callees: CCharString copy ctor 0x0099EC30, CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0.
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` over the pair with STLport's user copy constructor and
//   the implicit assignment (inlined here, not the out-of-line 0x009E6380): three string copies and the long, three
//   string assignments and the long per direction; the temporary dies in reverse member order.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CSymbolMap {
public:
    class CSymbolInfo {                                 // Ego_r.pdb, 12 bytes
    public:
        CCharString GroupName;                          // +0x00
        CCharString EntryName;                          // +0x04
        long        Val;                                // +0x08
    };
};

namespace std {
template <class _T1, class _T2>
struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap<pair<CCharString, CSymbolMap::CSymbolInfo> >(pair<CCharString, CSymbolMap::CSymbolInfo>&, pair<CCharString, CSymbolMap::CSymbolInfo>&);
}