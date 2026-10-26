// std::vector::erase(iterator first, iterator last) -- (vector<N3DAnimationBuilder::CSequenceInfo >) STLport range erase of trivially-assigned records: copy [last, end()) down onto first, set the new end; returns first
// Leaf: ?erase@?$vector@VCSequenceInfo@N3DAnimationBuilder@@V?$allocator@VCSequenceInfo@N3DAnimationBuilder@@@std@@@std@@QAEPAVCSequenceInfo@N3DAnimationBuilder@@PAV34@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB2C70, 99 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): std::vector<N3DAnimationBuilder::CSequenceInfo >::erase(N3DAnimationBuilder::CSequenceInfo*, N3DAnimationBuilder::CSequenceInfo*); evidence: C3DAnimationBuilder::BuildMeshAnimation 0x00AB2520 (Ego_r.pdb ?BuildMeshAnimation@C3DAnimationBuilder@@QAEKAAVCOutputAnimation@N3DAnimationBuilder@@...) stores GeneratedBoneCount at output+0x0C and resizes output+0 = Ego_r.pdb N3DAnimationBuilder::COutputAnimation::Sequences, CArray<N3DAnimationBuilder::CSequenceInfo> (52-byte records = the 0x34 stride) through resize 0x00AB3290, which shrinks through this erase.
// Label: the manifest's "Vector_MoveElements_0x34Bytes" is behavioural / propagated (family J102: five range erases of 0x24- to 0x34-byte records;
//   retail has no identical-code folding, so each address is its own element type).
// Callees: none (the forward __copy is inline: a division by 0x34 (multiply by 0x4EC4EC4F), then one `rep movsd` of
//   13 dwords per element; _Destroy's loop over the trivially-destroyed tail folds away).
// Shape: STLport 4.6 _vector.h `erase(first, last)` over J092's helpers with __copy_ptrs / __copy `__forceinline` (plain inline: VC7.1 keeps __copy out of line).
class CMatrix3x4 {                                         // 48 bytes (Ego_r.pdb: a union of rows / elements; opaque here)
public:
    unsigned char _body[0x30];
};
namespace N3DAnimationBuilder {
class CSequenceInfo {                                      // 52 bytes (Ego_r.pdb; class key V from FableWin.pdb)
public:
    long OriginalIndex;                                    // +0x00
    CMatrix3x4 GeneratedTransform;                         // +0x04
};
}
namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                 const random_access_iterator_tag&, _Distance*)   // inline in retail (agent951 G040's device)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
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
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
} // namespace std
typedef N3DAnimationBuilder::CSequenceInfo TElement_00ab2c70;
template TElement_00ab2c70* std::vector<TElement_00ab2c70 >::erase(TElement_00ab2c70*, TElement_00ab2c70*);