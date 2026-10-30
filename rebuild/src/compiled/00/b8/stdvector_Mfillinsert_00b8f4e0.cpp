// std::vector::_M_fill_insert(CEngineOcclusionManager::COccluderFlareEntry* position, unsigned int n, const CEngineOcclusionManager::COccluderFlareEntry& x) -- (vector<CEngineOcclusionManager::COccluderFlareEntry>) STLport insert of n copies of a 28-byte flare-test entry: slide the tail inside the spare capacity and fill the hole from a copy of x, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@UCOccluderFlareEntry@CEngineOcclusionManager@@V?$allocator@UCOccluderFlareEntry@CEngineOcclusionManager@@@std@@@std@@QAEXPAUCOccluderFlareEntry@CEngineOcclusionManager@@IABU34@@Z
// Grades: 3 (the third only re-graded the corrected Leaf line; DIFFER 316v339 with the implicit C3DVector copy: rep movsd for x_copy; RELOCATION_MATCH with the memberwise user copy ctor)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8F4E0, 339 bytes, thiscall `ret 0Ch`, no EH frame.
// Identity (proven): the landed vector<CEngineOcclusionManager::COccluderFlareEntry>::resize(n, x) 0x00B8F780 calls it
//   (0x00B8F7D1), and resize's callers are CEngineOcclusionManager::FrameStart 0x00B8DBE0 (this+0x24024 = FlareTests,
//   Ego_r.pdb CArray<COccluderFlareEntry>) and 0x00B8F810; the 0x92492493 / sar 4 reciprocal is the 28-byte element; the
//   landed __copy_backward<COccluderFlareEntry*, ..., int> 0x00B8E980 is its five-argument copy-backward callee. The jmp
//   thunk 0x00B8F730 (vector::insert(pos, n, x) -> _M_fill_insert) is the other reference. The catalog label
//   "Vector_InsertWithCount_CameraTag" and the pair<float, CSubtitledMessage::CCameraTag> spellings bound on its callees
//   0x00B8EED0 / 0x00B8E930 / 0x00B8F370 are propagated (NAME_FIXES.tsv).
// Callees (free helpers __fastcall): __uninitialized_copy 0x00B8EED0 (twice), __copy_backward<..., int> 0x00B8E980 (five
//   arguments: behind the inline __copy_backward_ptrs), __uninitialized_fill_n 0x00B8E930 (behind the inline
//   uninitialized_fill_n), fill 0x00B8E880 (twice), _M_insert_overflow 0x00B8F370.
// Shape: the landed 0x00AF7570 model (STLport 4.6 _vector.c verbatim): x_copy (28 bytes) is the whole frame; retail homes the
//   tags on the dead x slot, so the tag-taking helpers get same-TU noinline bodies that never touch the tag.
// Layout: COccluderFlareEntry (Ego_r.pdb struct, 28 bytes): Position C3DVector, Radius, CircleSegments, MaxPixelCount,
//   OccluderHandle (no engine header for it or C3DVector).
inline void* operator new(unsigned int, void* p) { return p; }

class C3DVector {
public:
    float X, Y, Z;                                        // +0x00
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // retail's memberwise copy (word by word, no rep movsd)
};
class CEngineOcclusionManager {
public:
    struct COccluderFlareEntry {                          // Ego_r.pdb (struct), 28 bytes
        C3DVector Position;                               // +0x00
        float Radius;                                     // +0x0C
        unsigned long CircleSegments;                     // +0x10
        unsigned long MaxPixelCount;                      // +0x14
        unsigned long OccluderHandle;                     // +0x18
    };
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00B8EED0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x00B8E930
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                              // 0x00B8E980
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);                     // 0x00B8E880

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                       // 0x00B8F370
public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}
}


typedef CEngineOcclusionManager::COccluderFlareEntry TFlareEntry;
template void std::vector<TFlareEntry>::_M_fill_insert(TFlareEntry*, unsigned int, const TFlareEntry&);