// std::pair::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~pair<const CCharString, CArray<unsigned long> > inlined (second freed, then first destroyed), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_G?$pair@$$CBVCCharString@@V?$CArray@K@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D3670, 48 bytes, thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: std::pair<const CCharString, CArray<unsigned long> > -- probable; evidence: the bytes need a CCharString at +0 and a vector of trivially destructible elements at +4 (rules out C3DSkeletalMorphTarget, whose CArray<C3DSkeletalBoneMorph> elements hold strings); the same TU's 0x009D3CA0 copy-constructs exactly this pair (CCharString copy 0x0099EC30 at +0, the 4-byte-element vector copy 0x00535470 at +4) for the heap/sort helpers 0x009D3750; FableWin.pdb names ??_G?$pair@$$CBVCCharString@@V?$CArray@K@@@std@@QAEPAXI@Z (the element could equally be long: same bytes)
// Label: the manifest's "CParticleEffect::`scalar_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (the inlined _Vector_base destructor: allocator::deallocate behind the null test), CCharString::~CCharString 0x0099EAE0 (first), operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 stores the vptr only in user-written destructors and pair<const CCharString, CArray<unsigned long> >'s is implicit; members are
//   destroyed in reverse declaration order (the vector's trivially destructible elements need no loop: STLport's
//   __type_traits<T>::has_trivial_destructor selects the empty __destroy_aux).
//   The non-polymorphic deleting destructor is emitted where a delete expression needs it; retail keeps it although
//   nothing references it (no call or pointer in Fable.exe), so DeleteBankPair is that delete expression, nothing more.
class CCharString {
public:
    CCharString();                                            // 0x0099E4B0
    ~CCharString();                                           // 0x0099EAE0
    void* PStringData;
};
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <> struct __type_traits<unsigned long> { typedef __true_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
template <class T> class CArray : public std::vector<T> { };     // retail CArray: an STLport vector (12 bytes)
typedef std::pair<const CCharString, CArray<unsigned long> > TBankPair;
void DeleteBankPair(TBankPair* p) { delete p; }