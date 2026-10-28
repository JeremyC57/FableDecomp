// std::vector<C3DMeshInfo>::_M_insert_overflow(C3DMeshInfo* position, const C3DMeshInfo& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport growth insert: allocate size + max(size, fill_len) entries, copy the head, construct x once (or fill fill_len copies), copy the tail unless atend, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VC3DMeshInfo@@V?$allocator@VC3DMeshInfo@@@std@@@std@@IAEXPAVC3DMeshInfo@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2C7F0, 286 bytes, thiscall, `ret 14h` (five arguments).
// Identity: STLport 4.6 `vector<_Tp>::_M_insert_overflow(pointer, const _Tp&, const __false_type&, size_type, bool)`
//   (stl/_vector.h, protected: IAE), element C3DMeshInfo: 0x60 stride (`imul 2AAAAAABh ; sar 4` = /0x60), every copy
//   through `??0C3DMeshInfo@@QAE@ABV0@@Z` 0x00A2A400, virtual destructor in the _Destroy loop. Callers: the landed
//   CMeshDataBank::NotifyEntryAdded 0x00A26D40 (inlined push_back of the mesh-info array) and push_back 0x00A2D140; both
//   bind this address with invented spellings (`?_M_insert_overflow@CMeshInfoArray@@QAEX...`,
//   `?InsertGrow@CRetailVector_C3DMeshInfo@@QAEX...`, NAME_FIXES.tsv). Manifest label "std_vector_expand_96bytes" is
//   descriptive.
// Callees: malloc 0x00BFEA0E, C3DMeshInfo's copy constructor 0x00A2A400 (inlined head copy and fill loops), the
//   out-of-line `__uninitialized_copy<C3DMeshInfo*, C3DMeshInfo*>(first, last, result, const __false_type&)` 0x00A2AE20
//   (/Gr fastcall, tail only), the element's virtual destructor (slot 0), free 0x00BFEA14.
// Shape: STLport's plain-inline helpers as written: VC7.1 inlines the head __uninitialized_copy and the
//   __uninitialized_fill_n loop (their bodies are one call each, the copy constructor being out of line) and leaves
//   the tail copy as a call, exactly as retail; noinline or __forceinline helpers give an all-call or all-inline body.
// Layout: local models -- engine/C3DMeshInfo.h / CMeshDataBankEntry.h are flat (no vptr, no base), and the placement
//   copy needs the real polymorphic pair (header debt, NOTES.md).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CMeshDataBank;
class CIVCountedPointeeBase;

class CMeshDataBankEntry {                                 // vtable 0x0129CD28, 12 bytes
public:
    virtual ~CMeshDataBankEntry();
    CMeshDataBank* Bank;                                   // +0x04
    unsigned long  Index;                                  // +0x08
};

class C3DMeshInfo : public CMeshDataBankEntry {            // 0x60 bytes (Ego_r.pdb)
public:
    C3DMeshInfo(const C3DMeshInfo& other);                 // 0x00A2A400 (out of line)
    CIVCountedPointeeBase* MeshData;                       // +0x0C CIVCountedPointer<C3DMeshInfo::CMeshData>
    float BoundingBox[6];                                  // +0x10 C3DBoundingBox
    float BoundingSphere[4];                               // +0x28 C3DBoundingSphere
    float SafeBoundingRadius;                              // +0x38
    long PhysicsMeshIndex;                                 // +0x3C
    float LODErrors[4];                                    // +0x40
    unsigned long LODDataSizes[4];                         // +0x50
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
__forceinline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
__forceinline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                          // 0x00A2AE20 (tail call; the head copy inlines)
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                          // inlined (no out-of-line copy for C3DMeshInfo)
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
typedef C3DMeshInfo _Elem;
template void vector<_Elem>::_M_insert_overflow(_Elem*, const _Elem&, const __false_type&, unsigned int, bool);
} // namespace std