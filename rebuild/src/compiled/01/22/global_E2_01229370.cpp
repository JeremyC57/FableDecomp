// _$E2() -- `dynamic atexit destructor for 'CEnvironment::SetWeightedThemes::ThemeWeights'': the function-local static's inlined STLport ~vector (trivially destroyed elements: just free the storage)
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x01229370, 17 bytes, in `.text$yd`: `mov eax,[0x013BA8EC] ; test ; je ; push eax ; call free ; pop ecx ; ret`.
// Identity (proven): the static vector at 0x013BA8EC is initialised (guard bit 1 of 0x013BA8F8, zero stores, atexit of this thunk at 0x006B3EC2) inside 0x006B3E60, a `ret 14h` CEnvironment member that grows it through vector<pair<long,float> >::insert 0x006B5ED0 / lower_bound 0x006B5A50; Ego_r.pdb/FableWin.pdb: `CEnvironment::SetWeightedThemes'::`17'::`dynamic atexit destructor for 'ThemeWeights'' (?SetWeightedThemes@CEnvironment@@QAEXJJPAJPAMJ@Z, five arguments as retail's ret 14h); the debug scope numbers differ (Ego_r 17, FableWin 29), so the static is declared at the function's top level here (?1).
// Static: `static std::vector<std::pair<long, float> > ThemeWeights;` in CEnvironment::SetWeightedThemes (0x006B3E60).
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
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
};
}
class CEnvironment {
public:
    void SetWeightedThemes(long a, long b, long* themes, float* weights, long count);   // 0x006B3E60, `ret 14h`
    long LastCount;
};
// Only the local static's atexit destructor is graded; this body just owns the static (the retail one does more).
void CEnvironment::SetWeightedThemes(long, long, long*, float*, long)
{
    static std::vector<std::pair<long, float> > ThemeWeights;   // 0x013BA8EC
    LastCount = ThemeWeights.size();
}