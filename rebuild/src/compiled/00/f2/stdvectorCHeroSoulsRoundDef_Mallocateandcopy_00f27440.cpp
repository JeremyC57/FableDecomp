// std::vector<CHeroSoulsRoundDef>::_M_allocate_and_copy(size_type n, const CHeroSoulsRoundDef* first, const CHeroSoulsRoundDef* last) -- STLport: allocate n elements and copy-construct [first, last) into them
// Leaf: ??$_M_allocate_and_copy@PBVCHeroSoulsRoundDef@@@?$vector@VCHeroSoulsRoundDef@@V?$allocator@VCHeroSoulsRoundDef@@@std@@@std@@IAEPAVCHeroSoulsRoundDef@@IPBV2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00f27440, 74 bytes, thiscall (`this` unused), `ret 0Ch`.
// Identity (probable): its only caller is the vector operator= 0x00F26C30 next to the landed CHeroSoulsWaveDef vector code; the element (0x38 bytes) is copied by 0x00F26DB0 (def vtable 0x012FB430, adjacent to CHeroSoulsWaveDef's 0x012FB498) -- Ego_r.pdb CHeroSoulsRoundDef (debug 60, retail 56; it holds the wave vector).
// Callees: malloc 0x00BFEA0E (n * 0x38; 0 when n is 0: the malloc_alloc allocator), _Construct 0x00F26DB0 (/Gr __fastcall:
//   destination ecx, source edx; out of line).
// Shape (family L142 = the landed N00f27bc0::vector<CHeroSoulsWaveDef> model of 0x00F27BC0): the copy loop is __uninitialized_copy
//   with the out-of-line _Construct (a same-TU noinline body); the output pointer is strength-reduced to source + delta.
extern "C" void* __cdecl malloc(unsigned int size);
inline void* operator new(unsigned int, void* p) { return p; }
class CHeroSoulsRoundDef {                                    // retail 0x38 bytes (def vtable 0x012FB430 over CDefClassBase)
public:
    unsigned char _body[0x38];
    CHeroSoulsRoundDef(const CHeroSoulsRoundDef& other);
};
namespace std {
struct __false_type { };
template <class _Tp> class allocator { };
template <class _T1, class _T2>
__declspec(noinline) void __fastcall _Construct(_T1* __p, const _T2& __val) { new ((void*)__p) _T1(__val); }   // 0x00F26DB0
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
protected:
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = __n ? (pointer)malloc(__n * sizeof(_Tp)) : 0;   // malloc_alloc: allocate(0) is 0
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
};
template CHeroSoulsRoundDef* vector<CHeroSoulsRoundDef>::_M_allocate_and_copy<const CHeroSoulsRoundDef*>(unsigned int, const CHeroSoulsRoundDef*, const CHeroSoulsRoundDef*);
}