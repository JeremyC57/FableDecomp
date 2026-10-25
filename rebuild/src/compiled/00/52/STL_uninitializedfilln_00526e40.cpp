// std::__uninitialized_fill_n(CSharerParserOutputToDebugContextMapper::CEntry* first, unsigned int n, const CSharerParserOutputToDebugContextMapper::CEntry& x, const __false_type&) -- copy-construct n line-mapping entries into raw storage at first and return first + n
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00526E40, 56 bytes, __fastcall (first ecx, n edx; x and the empty tag on the stack, `ret 8`).
// Callers: 0x00527C5A, 0x0052B478, 0x0052BA9B (the vector's fill-insert / grow paths; the landed fill 0x00527C10 is the same TU).
// Element: Ego_r.pdb CSharerParserOutputToDebugContextMapper::CEntry (0xC bytes): long SrcLine +0x0, long DestLine +0x4, CCharString DebugContext +0x8;
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

class CSharerParserOutputToDebugContextMapper {
public:
    class CEntry {                              // 0xC bytes
    public:
        long          SrcLine;                  // +0x0
        long          DestLine;                 // +0x4
        CCharString   DebugContext;             // +0x8
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

template CSharerParserOutputToDebugContextMapper::CEntry* __fastcall __uninitialized_fill_n(CSharerParserOutputToDebugContextMapper::CEntry*, unsigned int, const CSharerParserOutputToDebugContextMapper::CEntry&, const __false_type&);
}