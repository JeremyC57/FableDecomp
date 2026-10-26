// std::vector::erase(iterator position) -- (vector<CCountedPointer<CChunkedFileChunk> >) STLport single erase: shift the tail down one slot over position, drop the last slot (release its count block, clear both words), return position
// Leaf: ?erase@?$vector@V?$CCountedPointer@VCChunkedFileChunk@@@@V?$allocator@V?$CCountedPointer@VCChunkedFileChunk@@@@@std@@@std@@QAEPAV?$CCountedPointer@VCChunkedFileChunk@@@@PAV3@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA91F0, 97 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity (proven): std::vector<CCountedPointer<CChunkedFileChunk> >::erase(iterator); evidence: CChunkedFileSuperChunk::RemoveSubChunk 0x00AA8280 (Ego_r.pdb ?RemoveSubChunk@CChunkedFileSuperChunk@@QAEXABV?$CCountedPointer@VCChunkedFileChunk@@@@@Z) finds the chunk in this+0x10, clears its parent and tail-jumps here on this+0x10 = Ego_r.pdb SubChunks, CArray<CCountedPointer<CChunkedFileChunk> >; RemoveSubChunksOfType 0x00AA82C0 also erases through it.
// Label: the manifest's "RemovePredicate" is propagated / wrong in kind (family J095: five byte-identical bodies, one per
//   pointee; retail has no identical-code folding).
// Callees: std::__copy<CCountedPointer<CChunkedFileChunk>*, CCountedPointer<CChunkedFileChunk>*, int>(first, last, result, const random_access_iterator_tag&,
//   int*) 0x00AA8F40 (__fastcall, `ret 0Ch`; the empty tag sits in the dead `position` argument slot), the count block's
//   DeleteFunc (indirect) and operator delete 0x00BFE9BC from the inline ~CCountedPointer.
// Shape: STLport 4.6 _vector.h `if (pos + 1 != end()) __copy_ptrs(pos + 1, _M_finish, pos, _TrivialAss()); --_M_finish;
//   _Destroy(_M_finish); return pos;` (the landed 0x0084F570 shape) with J094's CCountedPointer.
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
class CCPPointerInfo {
public:
    unsigned long RefCount;                                 // +0
    void (__fastcall* DeleteFunc)(void* data);              // +4, Data in ecx
    void* Data;                                             // +8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0
    CCPPointerInfo* Info;                                   // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
};
class CChunkedFileChunk;
namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };   // STLport's hierarchy: not an aggregate, so the
template <class _RandomAccessIter, class _OutputIter, class _Distance>      // `random_access_iterator_tag()` temporary is not zero-filled
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
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
    typedef __false_type _TrivialAss;
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_ptrs(__position + 1, this->_M_finish, __position, _TrivialAss());
        --this->_M_finish;
        _Destroy(this->_M_finish);
        return __position;
    }
};
} // namespace std
typedef CCountedPointer<CChunkedFileChunk > TElement_00aa91f0;
template TElement_00aa91f0* std::vector<TElement_00aa91f0 >::erase(TElement_00aa91f0*);