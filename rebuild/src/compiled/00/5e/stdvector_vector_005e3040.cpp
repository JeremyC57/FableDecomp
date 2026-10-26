// std::vector::~vector() -- (vector<std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > >) STLport destructor: destroy every element (out of line), then free the block
// Leaf: ??1?$vector@U?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@V?$allocator@U?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E3040, 37 bytes (+ int3 padding to 48), thiscall, plain `ret`.
// Identity (proven): vector<std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > >::~vector; evidence: its only caller is the scalar deleting destructor 0x005E35F0 (slot 0 of vtable 0x01255878, RTTI .?AVCAnimComponentMap@@) on this+8 -- Ego_r.pdb's CAnimComponentMap::Map +8, CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase>, CKeyPairCompareLess<...> > (a vector of pairs); its __destroy_aux 0x005E0F00 walks 8-byte elements and releases the intrusive counted pointer at +4 (dec [p+4], virtual slot 1 at zero, then zeroes it); Ego_r.pdb names ??1?$vector@U?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@...
// Label: the manifest's "OnReadFinished" is propagated (family H086: six destroy-and-free bodies with an out-of-line __destroy_aux --
//   STLport ~vector (with the inline ~_Vector_base) and the protected _M_clear() that _M_insert_overflow calls have the same
//   code; retail has no identical-code folding, so each address is one (function, element) pair).
// Callees: __destroy_aux<std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*> 0x005E0F00 (`__fastcall`, `ret 4`: first ecx, last edx, the __false_type tag by address),
//   free 0x00BFEA14 (allocator::deallocate behind its null test).
// Body: STLport 4.6 -- `_Destroy(_M_start, _M_finish)` then the block freed when non-null. __destroy_aux is a same-TU noinline
//   stand-in under auto_inline(off) (retail calls it out of line); the empty tag temporary lives in the `push ecx` slot.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
class CIVCountedPointeeBase;
template <class T> class CIVCountedPointer {           // 4 bytes (Ego_r.pdb)
public:
    CIVCountedPointeeBase* Object;                    // +0x00
    ~CIVCountedPointer();                             // inline in retail (in __destroy_aux)
};
namespace std {
template <class _T1, class _T2> struct pair {         // STLport pair
    _T1 first;
    _T2 second;
};
}

namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
#pragma auto_inline(off)
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)
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
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
}
enum EAnimComponent { };
class CAnimComponentBase;

template std::vector<std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >, std::allocator<std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > > >::~vector();