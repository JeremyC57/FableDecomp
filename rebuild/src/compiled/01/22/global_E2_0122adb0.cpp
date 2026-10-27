// _$E2() -- `dynamic atexit destructor for 'CLocalDetailCacheMap::CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren::sphere_list'': the function-local static's inlined STLport ~vector (trivially destroyed elements: just free the storage)
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x0122ADB0, 17 bytes, in `.text$yd`: `mov eax,[0x0143B978] ; test ; je ; push eax ; call free ; pop ecx ; ret`.
// Identity (proven): the static vector at 0x0143B978 is initialised and registers this thunk (0x00BDF120) inside CLocalDetailCacheMap::CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren 0x00BDF100 (resize 0x00BE0140 and _M_insert_overflow 0x00BDFE70 are the landed vector<C3DBoundingSphere> members); Ego_r.pdb/FableWin.pdb: `...::CalcBoundingSphereAndFadeDistanceFromChildren'::`2'::`dynamic atexit destructor for 'sphere_list'' (top-level scope, ?1).
// Static: `static std::vector<C3DBoundingSphere> sphere_list;` in CLocalDetailCacheMap::CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren (0x00BDF100).
// Callees: free 0x00BFEA14 (STLport allocator<T>::deallocate behind the _Vector_base null test).
// Shape (family M004, representative 0x01229370): VC7.1 emits a function-local static's atexit destructor as the TU's `_$E2`
//   in .text$yd; ~vector's _Destroy loop over elements with trivial destructors vanishes, leaving ~_Vector_base's
//   `if (_M_start) free(_M_start)`.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                       // +0x00
    _Tp* _M_finish;                                      // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    unsigned int size() const { return this->_M_finish - this->_M_start; }
};
}
class C3DVector { public: float X, Y, Z; };
class C3DBoundingSphere {                                // Ego_r.pdb, 16 bytes
public:
    C3DBoundingSphere() { }
    C3DBoundingSphere(const C3DBoundingSphere& o) : Center(o.Center), Radius(o.Radius) { }
    C3DVector Center;
    float Radius;
};
class CLocalDetailCacheMap {
public:
    class CQuadTreeElement {
    public:
        void CalcBoundingSphereAndFadeDistanceFromChildren();   // 0x00BDF100
        long ChildCount;
    };
};
// Only the local static's atexit destructor is graded; this body just owns the static (the retail one does more).
void CLocalDetailCacheMap::CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren()
{
    static std::vector<C3DBoundingSphere> sphere_list;   // 0x0143B978
    ChildCount = sphere_list.size();
}