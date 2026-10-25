// _STL::__uninitialized_fill_n(COptimisedBone* first, unsigned int n, const COptimisedBone& x, const __false_type&) -- copy-construct n optimiser bone records into raw storage at first and return first + n
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD1700, 60 bytes, __fastcall (first ecx, n edx; x and the empty tag on the stack, `ret 8`).
// Callers: one caller, 0x00AD6016 (the vector's insert-grow 0x00AD5F80, landed).
// Element: Ego_r.pdb COptimisedBone (0x10 bytes): CCharString BoneName +0x0, long Parent +0x4, bool Remapped +0x8, long NewIndex +0xC;
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

class COptimisedBone {                              // 0x10 bytes
public:
    CCharString   BoneName;                     // +0x0
    long          Parent;                       // +0x4
    bool          Remapped;                     // +0x8
    long          NewIndex;                     // +0xC
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

template COptimisedBone* __fastcall __uninitialized_fill_n(COptimisedBone*, unsigned int, const COptimisedBone&, const __false_type&);
}