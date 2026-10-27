// _$E2() -- `dynamic atexit destructor for 'CAnimateThing::CalcInterpolatedMeshAnim::sorted_combination_anims'': the function-local static's inlined STLport ~vector (trivially destroyed elements: just free the storage)
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x01229630, 17 bytes, in `.text$yd`: `mov eax,[0x013BAC20] ; test ; je ; push eax ; call free ; pop ecx ; ret`.
// Identity (proven): the static vector at 0x013BAC20 is initialised and registers this thunk (0x0070DEB4) inside CAnimateThing::CalcInterpolatedMeshAnim 0x0070D940, which fills it through vector<list<CAnimSet>::const_iterator>::_M_insert_overflow 0x00711080 (landed, agent953 J004); Ego_r.pdb: `CAnimateThing::CalcInterpolatedMeshAnim'::`23'::`dynamic atexit destructor for 'sorted_combination_anims''; the static sits in a nested block in retail (debug scope 23), declared at the top level here (?1).
// Static: `static std::vector<std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> > > sorted_combination_anims;` in CAnimateThing::CalcInterpolatedMeshAnim (0x0070D940).
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
template <class _Tp> struct _Const_traits { };
template <class _Tp, class _Traits> struct _List_iterator {
    _List_iterator() { }
    void* _M_node;
};
}
class CAnimSet;
class C3DAnimationBlendState {
public:
    C3DAnimationBlendState();
    C3DAnimationBlendState(const C3DAnimationBlendState& other);
    ~C3DAnimationBlendState();
    long Count;
};
class CAnimateThing {
public:
    class CCompleteAnimation;
    class CDelayedAnimation;
private:
    C3DAnimationBlendState CalcInterpolatedMeshAnim(const CCompleteAnimation& anim, const CDelayedAnimation& delayed, float alpha) const;   // 0x0070D940
};
// Only the local static's atexit destructor is graded; this body just owns the static (the retail one does more).
C3DAnimationBlendState CAnimateThing::CalcInterpolatedMeshAnim(const CCompleteAnimation&, const CDelayedAnimation&, float) const
{
    static std::vector<std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> > > sorted_combination_anims;   // 0x013BAC20
    C3DAnimationBlendState result;
    result.Count = sorted_combination_anims.size();
    return result;
}