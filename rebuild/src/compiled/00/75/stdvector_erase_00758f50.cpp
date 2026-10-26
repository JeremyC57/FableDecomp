// std::vector::erase(iterator first, iterator last) -- (vector<std::pair<EBodyLocation, CWoundMorphs::CEntry> >) STLport range erase of trivially-assigned records: copy [last, end()) down onto first, set the new end; returns first
// Leaf: ?erase@?$vector@U?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@std@@V?$allocator@U?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@std@@@2@@std@@QAEPAU?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@2@PAU32@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00758F50, 99 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): std::vector<std::pair<EBodyLocation, CWoundMorphs::CEntry> >::erase(std::pair<EBodyLocation, CWoundMorphs::CEntry>*, std::pair<EBodyLocation, CWoundMorphs::CEntry>*); evidence: CWoundMorphs::TransferBinaryIn 0x00757990 (Ego_r.pdb ?TransferBinaryIn@CWoundMorphs@@QAEXAAVCMemoryDataInputStream@@@Z) resizes its Morphs vector with a zeroed 0x2C-byte value through resize 0x007595F0, which shrinks through this erase -- Ego_r.pdb CWoundMorphs::Morphs, CVectorMap<EBodyLocation, CWoundMorphs::CEntry, ...> = vector<pair<EBodyLocation, CWoundMorphs::CEntry> > (44 bytes); FableWin.pdb holds that vector's erase and resize.
// Label: the manifest's "Vector_Erase_Range" is behavioural / propagated (family J102: five range erases of 0x24- to 0x34-byte records;
//   retail has no identical-code folding, so each address is its own element type).
// Callees: none (the forward __copy is inline: a division by 0x2C (multiply by 0x2E8BA2E9), then one `rep movsd` of
//   11 dwords per element; _Destroy's loop over the trivially-destroyed tail folds away).
// Shape: STLport 4.6 _vector.h `erase(first, last)` over J092's helpers with __copy_ptrs / __copy `__forceinline` (plain inline: VC7.1 keeps __copy out of line).
enum EBodyLocation { };                                   // Ego_r.pdb (enumerators not needed)
class CWoundMorphs {                                       // partial view: only the nested type is used here
public:
    class CEntry {                                         // 40 bytes (Ego_r.pdb; class key V from FableWin.pdb)
    public:
        EBodyLocation Location;                            // +0x00
        long ReplacingTextureIndex;                        // +0x04
        long ReplacingBumpMapIndex;                        // +0x08
        long WoundTextureIndex;                            // +0x0C
        long WoundBumpMapIndex;                            // +0x10
        long ScarTextureIndex;                             // +0x14
        long ScarBumpMapIndex;                             // +0x18
        long FadeToScarDuration;                           // +0x1C
        long TotalDuration;                                // +0x20
        float PermanentScarStrength;                       // +0x24
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x00
    _T2 second;                                            // +0x04
};
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
typedef std::pair<EBodyLocation, CWoundMorphs::CEntry> TElement_00758f50;
template TElement_00758f50* std::vector<TElement_00758f50 >::erase(TElement_00758f50*, TElement_00758f50*);