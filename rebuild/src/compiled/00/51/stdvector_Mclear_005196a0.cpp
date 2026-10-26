// std::vector::_M_clear() -- (vector<CLevelLoader::CObject::CLevelFiles >) STLport 4.6 `_M_clear()`: destroy every element, then free the block (the reallocating branch of vector::operator= / reserve)
// Leaf: ?_M_clear@?$vector@VCLevelFiles@CObject@CLevelLoader@@V?$allocator@VCLevelFiles@CObject@CLevelLoader@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005196A0, 50 bytes (+ int3 padding to 64), thiscall, plain `ret`.
// Identity (proven): vector<CLevelLoader::CObject::CLevelFiles >::_M_clear; evidence: its only caller is the vector operator= 0x006C2D40, which 0x006C27A0 applies to this+0x10 from its first argument before assigning the set at +0 from its second and storing its third at +0x1C -- Ego_r.pdb's CLevelLoader::CObject::SetAsLoading(const vector<CLevelFiles>&, const set<CCharString>&, long) over LoadableSections (+0), RegionLevelFiles (+0x10 with the retail 12-byte set) and RegionIndex (+0x1C); the element destructor releases three CCountedPointers at +4 / +0x0C / +0x14 (CLevelFiles: LevelID, PTopologyFile, PNavigationFile, PThingFile)
// Label: the manifest's "ResetRings" is propagated (family H072: six destroy-and-free bodies whose element destructor runs on the element itself -- STLport ~vector (with the inline
//   ~_Vector_base) and the protected _M_clear() that vector::operator= calls when it reallocates have the same code; retail has
//   no identical-code folding, so each address is one (function, element) pair).
// Callees: CLevelLoader::CObject::CLevelFiles::~CLevelFiles 0x0050F980 (out of line; Ego_r.pdb emits ??1CLevelFiles@CObject@CLevelLoader@@), free 0x00BFEA14 (allocator::deallocate behind its null test).
// Body: STLport 4.6 -- `_Destroy(_M_start, _M_finish)` (the element's implicit destructor in an inline __destroy_aux loop,
//   stride 28) then the block freed when non-null.
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
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
}
class CLevelLoader {
public:
    class CObject {
public:
        class CLevelFiles {                                    // 0x1C bytes: LevelID + three CCountedPointer<CAFile>
        public:
            unsigned char Data[0x1C];
            ~CLevelFiles();                                    // 0x0050F980
        };
    };
};

template void std::vector<CLevelLoader::CObject::CLevelFiles, std::allocator<CLevelLoader::CObject::CLevelFiles > >::_M_clear();