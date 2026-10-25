// std::__uninitialized_fill_n(CSymbolMap::CSymbolInfo* first, unsigned int n, const CSymbolMap::CSymbolInfo& x, const __false_type&) -- copy-construct n symbol-map entries into raw storage at first and return first + n
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E5FE0, 60 bytes, __fastcall (first ecx, n edx; x and the empty tag on the stack, `ret 8`).
// Callers: one caller, 0x009E7794 (the vector's grow path at 0x009E76F0; CSymbolMap code surrounds it).
// Element: Ego_r.pdb CSymbolMap::CSymbolInfo (0xC bytes): CCharString GroupName +0x0, CCharString EntryName +0x4, long Val +0x8;
//   implicit memberwise copy constructor (the CCharString member through its out-of-line copy constructor 0x0099EC30).
// Label: catalog "vector<unsigned __int64>::_Ufill" is propagated across the byte-identical-shaped STLport helpers.
// Shape: STLport 4.6 __uninitialized_fill_n verbatim (`for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);`, unsigned
//   n so `jbe`), the placement new keeping its null test (`test esi,esi ; je`).
inline void* operator new(unsigned int, void* p) { return p; }

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;               // +0x0
    CCharString(const CCharString& other);      // 0x0099EC30
};

class CSymbolMap {
public:
    class CSymbolInfo {                              // 0xC bytes
    public:
        CCharString   GroupName;                // +0x0
        CCharString   EntryName;                // +0x4
        long          Val;                      // +0x8
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CSymbolMap::CSymbolInfo* __fastcall __uninitialized_fill_n(CSymbolMap::CSymbolInfo*, unsigned int, const CSymbolMap::CSymbolInfo&, const __false_type&);
}