// _STL::__uninitialized_fill_n(pair<pair<CCharString, CCharString>, long>* first, unsigned int n, const pair<pair<CCharString, CCharString>, long>& x, const __false_type&) -- copy-construct n (string-pair key, long) map entries into raw storage at first and return first + n
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3FC00, 60 bytes, __fastcall (first ecx, n edx; x and the empty tag on the stack, `ret 8`).
// Callers: one, 0x00A41434 (the vector's grow path at 0x00A41390, called from the pair vector's push_back 0x00A41840 that
//   the landed CVectorMap<{CCharString, CCharString}, long>::Add 0x00A41A70 calls).
// Element: the 12-byte CVectorMap entry STLport pair<pair<CCharString, CCharString>, long> (key strings +0x0 / +0x4, value
//   +0x8); STLport's memberwise pair copy constructor (each CCharString through its out-of-line copy constructor 0x0099EC30).
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

namespace _STL {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

typedef pair<pair<CCharString, CCharString>, long> CStringPairKeyEntry;     // 0xC bytes

template CStringPairKeyEntry* __fastcall __uninitialized_fill_n(CStringPairKeyEntry*, unsigned int, const CStringPairKeyEntry&,
                                                                const __false_type&);
}