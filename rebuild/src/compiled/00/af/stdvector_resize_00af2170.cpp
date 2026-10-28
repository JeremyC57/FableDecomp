// std::vector::resize(size_type new_size, const value_type& x) -- (vector<NParticleEngine::CParticleEmitterInfo >) STLport resize: shrink by erasing [begin() + new_size, end()) (copy down, destroy the stale tail), or grow by inserting new_size - size() copies of x at the end
// Leaf: ?resize@?$vector@VCParticleEmitterInfo@NParticleEngine@@V?$allocator@VCParticleEmitterInfo@NParticleEngine@@@std@@@std@@QAEXIABVCParticleEmitterInfo@NParticleEngine@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF2170, 104 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): std::vector<NParticleEngine::CParticleEmitterInfo >::resize(unsigned int, const NParticleEngine::CParticleEmitterInfo&); evidence: NParticleEngine::CParticleDataBank's vtable slot 10 0x00AF16E0 (vtable RTTI .?AVCParticleDataBank@NParticleEngine@@) calls it (0x00AF1712) on this+0x1E0 with a zeroed 12-byte temporary -- Ego_r.pdb CParticleDataBank::ParticleEmitters is CArray<NParticleEngine::CParticleEmitterInfo> (Data, Bank, Index); the destroy callee 0x00AF1D90 is bound over CParticleEmitterInfo, and FableWin.pdb holds ?resize@?$vector@VCParticleEmitterInfo@NParticleEngine@@...@std@@QAEXI@Z; the other caller 0x00AF2200 is resize(n).
// Label: the manifest's "resize" is right in kind but propagated from a byte-identical body elsewhere (family J086: the
//   104-byte resize(n, x) of 12-byte class elements; retail has no identical-code folding, so each address is its own element type).
// Callees: __copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x00AF1CE0 (erase's __copy_ptrs,
//   __fastcall, `ret 0Ch`); __destroy_aux<T*>(first, last, const __false_type&) 0x00AF1D90 (the bindings' ?DestroyRange@@YIXPAVCParticleEmitterInfo@NParticleEngine@@... is an invented spelling of it) (erase's _Destroy, __fastcall,
//   `ret 4`); _M_fill_insert(pos, n, x) 0x00AF2010 (insert(end(), n, x), thiscall, `ret 0Ch`).
// Shape: STLport 4.6 _vector.h `resize(n, x)` with erase(first, last) inline; the tag temporaries live in the dead new_size slot.
namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
protected:
    public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // out of line
    protected:
};
} // namespace std
class CIVCountedPointeeBase;
template <class T> class CIVCountedPointer {               // 4 bytes
public:
    CIVCountedPointeeBase* Object;                         // +0x00
};
namespace NParticleEngine {
class CParticleDataBank;
class CParticleEmitterInfo {                               // 12 bytes (Ego_r.pdb)
public:
    class CEmitterData;
    CIVCountedPointer<CEmitterData> Data;                  // +0x00
    CParticleDataBank* Bank;                               // +0x04
    unsigned long Index;                                   // +0x08
};
}
typedef NParticleEngine::CParticleEmitterInfo TElement_00af2170;
template void std::vector<TElement_00af2170 >::resize(unsigned int, const TElement_00af2170&);