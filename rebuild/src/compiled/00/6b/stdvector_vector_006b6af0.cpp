// std::vector::~vector() -- (vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry >) out-of-line destructor: destroy every 0x30-byte element (its implicit destructor frees the inner Themes array at +0x8 when it has a block), then free the vector's own block
// Leaf: ??1?$vector@VCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@V?$allocator@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6AF0, 58 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry >::~vector -- probable; evidence: only unreferenced code reaches it (the jmp thunks 0x006B6D10 / 0x006B6D80 / 0x006B6F00 and the deleting destructor 0x006B6E70, i.e. the CArray-level destructors of the same vector) in the environment-theme TU (CIDrawEnvironmentParams rows 0x006B6010..0x006B6B30); its 0x30-byte element with a block at +8 is exactly Ego_r.pdb NEnvironmentInternal::CEnvironmentThemePriorityStackEntry with the 12-byte retail CArray (the double at +0x20 rounds it to 48) and FableWin.pdb names ??1?$vector@VCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@...@QAE@XZ.
// Label: the manifest's "Vector_DestroyRange" is descriptive / propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (twice: the element's inner block, then the vector's block).
// Body: STLport 4.6 `~vector() { _Destroy(_M_start, _M_finish); }` with the inline `~_Vector_base() { if (_M_start) deallocate(_M_start, ...); }`; the element destructors are implicit (the inner vector's
//   trivially destructible elements need no loop: STLport's __type_traits<T>::has_trivial_destructor picks the empty
//   __destroy_aux).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };

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
    void _M_clear()                                             // protected in STLport 4.6; public as the landed bindings spell it
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
}
template <class T> class CArray : public std::vector<T> { };     // retail CArray: an STLport vector (12 bytes)
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {                   // 48 bytes (Ego_r.pdb 56 with its 16-byte CArray)
public:
    class CThemeWeight {                                      // 12 bytes (Ego_r.pdb)
    public:
        long ThemeID;
        float Weight;
        bool LockWeight;
    };
    float  BaseWeight;                                        // +0x00
    bool   BaseWeightLocked;                                  // +0x04
    CArray<CThemeWeight> Themes;                              // +0x08
    long   ThemeTransitionTargetID;                           // +0x14
    float  ThemeTransitionInitialWeight;                      // +0x18
    bool   ThemeTransitionActive;                             // +0x1c
    double ThemeTransitionStartTime;                          // +0x20
    float  ThemeTransitionDuration;                           // +0x28
};
}
namespace std {
template <> struct __type_traits<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight > { typedef __true_type has_trivial_destructor; };
}
template class std::vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry >;