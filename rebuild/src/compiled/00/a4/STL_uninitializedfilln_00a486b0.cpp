// _STL::__uninitialized_fill_n(CLipSyncPhoneme* first, unsigned int n, const CLipSyncPhoneme& x, const __false_type&) -- copy-construct n lip-sync phonemes into raw storage at first and return first + n
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A486B0, 54 bytes, __fastcall (first ecx, n edx; x and the empty tag on the stack, `ret 8`).
// Callers: one, 0x00A4A7B6 (the phoneme vector's fill insert 0x00A4A710, manifest "vector_insert").
// Callees: CCharString copy constructor 0x0099EC30.
// Element: Ego_r.pdb CLipSyncPhoneme (12 bytes): CCharString Symbol +0x0, unsigned long ID +0x4, float Weight +0x8 -- implicit
//   memberwise copy (the string through its out-of-line copy constructor, then two dword moves).
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

class CLipSyncPhoneme {
public:
    CCharString   Symbol;                       // +0x0
    unsigned long ID;                           // +0x4
    float         Weight;                       // +0x8
};

namespace _STL {
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

template CLipSyncPhoneme* __fastcall __uninitialized_fill_n(CLipSyncPhoneme*, unsigned int, const CLipSyncPhoneme&,
                                                            const __false_type&);
}