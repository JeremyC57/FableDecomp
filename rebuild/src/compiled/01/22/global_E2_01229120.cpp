// _$E2() -- `dynamic atexit destructor for 'CAnimationEntry::RegisteredComponentsByType'': the inlined STLport ~vector of the static (out-of-line __destroy_aux over the elements, then free the storage)
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x01229120, 43 bytes, in `.text$yd`; registered by the initializer 0x0121AE80 (`push 0x01229120 ; call _atexit`).
// Identity (proven): the 0x10-byte CVectorMap at 0x013B8C28 (start, finish +4, end +8 = 0x013B8C30, Dirty +0xD) filled by the StatRegisterClass registrars 0x005DD76E..; its initializer 0x0121AE80 registers this thunk. Second of CAnimationEntry's three statics in the Ego_r.pdb/FableWin.pdb initializer order (ByToken = the tree at 0x013B8C18, ByType = this, Defaults = 0x013B8C38, the GetComponent fallback map). Element type from its __destroy_aux 0x005E0F00, the same instance RegisteredComponentDefaults' thunk calls.
// Callees: std::__destroy_aux<pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*> 0x005E0F00 (`__fastcall`: first in ecx, last in edx, the __false_type tag by address,
//   `ret 4`), free 0x00BFEA14 (STLport allocator<T>::deallocate behind the _Vector_base null test).
// Shape (family M003, representative 0x01229090): VC7.1 emits the static's initializer `_$E1` and this atexit thunk `_$E2`
//   with ~vector inlined: STLport `_Destroy(_M_start, _M_finish)` -> `__destroy_aux(first, last, __false_type())` called out
//   of line with the empty tag temporary (user ctor, no store) in the `push ecx` word ([esp+3]); then ~_Vector_base re-reads
//   _M_start for the null-tested free. The initializer here default-constructs the static (only the thunk is graded).
extern "C" void __cdecl free(void* block);               // 0x00BFEA14
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x005E0F00
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
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
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
enum EAnimComponent { ANIM_COMPONENT_FLAGS = 0 };
class CAnimComponentBase;
template <class T> class CIVCountedPointer {             // 4 bytes: the pointee carries the count
public:
    ~CIVCountedPointer();
    T* Object;
};
template <class K, class V> class CKeyPairCompareLess { public: CKeyPairCompareLess() { } };   // empty comparator (PDB class)
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {   // retail 0x10: STLport vector base + Compare +0xC + Dirty +0xD
public:
    C Compare;                                            // +0x0C
    bool Dirty;                                           // +0x0D
};
class CAnimationEntry {
public:
    static CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > RegisteredComponentsByType;   // 0x013B8C28
};
CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > CAnimationEntry::RegisteredComponentsByType;