// std::vector::erase(iterator first, iterator last) -- (vector<CTCInventoryMap::CUIMiniMapMarker >) STLport range erase: copy [last, end()) down onto first, destroy the stale tail, set the new end; returns first
// Leaf: ?erase@?$vector@VCUIMiniMapMarker@CTCInventoryMap@@V?$allocator@VCUIMiniMapMarker@CTCInventoryMap@@@std@@@std@@QAEPAVCUIMiniMapMarker@CTCInventoryMap@@PAV34@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00616D50, 77 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): std::vector<CTCInventoryMap::CUIMiniMapMarker >::erase(CTCInventoryMap::CUIMiniMapMarker*, CTCInventoryMap::CUIMiniMapMarker*); evidence: its callers are the CTCInventoryMap TU's resize(n, x) 0x00619050, the persist-context vector transfer 0x00619710 (Ego_r.pdb CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker>) and 0x00617FB0; the 28-byte stride and the string destroyed at +4 are Ego_r.pdb's CUIMiniMapMarker (Region, UIDefName, Position, DefNameLength, GraphicIndex, Angle) of CTCInventoryMap::MiniMapMarkers; FableWin.pdb holds vector<CTCInventoryMap::CUIMiniMapMarker>::erase.
// Label: the manifest's "CTCInventoryMap::CUIMiniMapMarker_MoveConstruct" is propagated / behavioural (family J092: the 77-byte range erases of class elements with one
//   string member; retail has no identical-code folding, so each address is its own element type).
// Callees: std::__copy<CTCInventoryMap::CUIMiniMapMarker*, CTCInventoryMap::CUIMiniMapMarker*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x00612F70 (__fastcall,
//   `ret 0Ch`; retail homes the empty tag in erase's LIVE `first` slot, which VC7.1 does only when it sees the same-TU __copy
//   body never touches the tag, so the body is given here), ~CCharString 0x0099EAE0 (??1CCharString@@QAE@XZ) on each stale element's +4.
// Shape: STLport 4.6 _vector.h `erase(first, last)`; _Destroy's __destroy_aux loop inline.
class CCharString {                                        // 4 bytes (TLC)
public:
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    void* PStringData;                                     // +0x00
};
class C2DVector {                                          // 8 bytes (Ego_r.pdb)
public:
    float X;                                               // +0x00
    float Y;                                               // +0x04
};
class CTCInventoryMap {                                    // partial view: only the nested type is used here
public:
    class CUIMiniMapMarker {                               // 28 bytes (Ego_r.pdb)
    public:
        long Region;                                       // +0x00
        CCharString UIDefName;                             // +0x04
        C2DVector Position;                                // +0x08
        long DefNameLength;                                // +0x10
        long GraphicIndex;                                 // +0x14
        float Angle;                                       // +0x18
    };
};
namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>              // 0x00612F70 (same-TU body, out of line)
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
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
typedef CTCInventoryMap::CUIMiniMapMarker TElement_00616d50;
template TElement_00616d50* std::vector<TElement_00616d50 >::erase(TElement_00616d50*, TElement_00616d50*);