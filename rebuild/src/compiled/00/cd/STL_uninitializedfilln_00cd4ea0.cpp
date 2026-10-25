// std::__uninitialized_fill_n(CMarkerDataEntry* first, unsigned int n, const CMarkerDataEntry& x, const __false_type&) -- copy-construct n marker data entries (two strings and a number) into raw storage at first and return first + n
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD4EA0, 60 bytes, __fastcall (first ecx, n edx; x and the empty tag on the stack, `ret 8`).
// Callers: one, 0x00CDBE64 (the vector's _M_insert_overflow 0x00CDBDC0, reached from the marker-text parser 0x00CD5170
//   that push_backs {name, text, value} records: two CCharString copies then a long, default value 10, '#' = numeric).
// Element: 12 bytes -- CCharString +0x0, CCharString +0x4, long +0x8 (the type is not in the Ego PDB name sets that fit;
//   descriptive name); implicit memberwise copy constructor (each CCharString through its out-of-line copy constructor
//   0x0099EC30).
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

class CMarkerDataEntry {                        // 0xC bytes (descriptive)
public:
    CCharString Name;                           // +0x0
    CCharString Text;                           // +0x4
    long        Value;                          // +0x8
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

template CMarkerDataEntry* __fastcall __uninitialized_fill_n(CMarkerDataEntry*, unsigned int, const CMarkerDataEntry&,
                                                             const __false_type&);
}