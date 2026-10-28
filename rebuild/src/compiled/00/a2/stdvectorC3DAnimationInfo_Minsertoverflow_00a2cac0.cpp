// std::vector<C3DAnimationInfo>::_M_insert_overflow(C3DAnimationInfo* position, const C3DAnimationInfo& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport growth insert: allocate size + max(size, fill_len) entries, copy the head, construct x once (or fill fill_len copies), copy the tail unless atend, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VC3DAnimationInfo@@V?$allocator@VC3DAnimationInfo@@@std@@@std@@IAEXPAVC3DAnimationInfo@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2CAC0, 257 bytes, thiscall, `ret 14h` (five arguments).
// Identity: STLport 4.6 `vector<_Tp>::_M_insert_overflow(pointer, const _Tp&, const __false_type&, size_type, bool)`
//   (stl/_vector.h, protected: IAE), element C3DAnimationInfo: 0x28 stride (`imul 66666667h ; sar 4`), x copied through
//   `??0C3DAnimationInfo@@QAE@ABV0@@Z` 0x00A2A4F0, virtual destructor in the _Destroy loop; its only caller is the landed
//   push_back 0x00A2D200 (`push 1 ; push 1` = atend, fill_len). The manifest label "vector_insert_fill_generic_0x28" is
//   descriptive; the landed 0x00A2D200 binds this address as the invented
//   `?InsertGrow@CRetailVector_C3DAnimationInfo@@...` (NAME_FIXES.tsv).
// Callees: malloc 0x00BFEA0E, `__uninitialized_copy<C3DAnimationInfo*, C3DAnimationInfo*>(first, last, result, const
//   __false_type&)` 0x00A2AF20 (/Gr fastcall, head and tail), `__uninitialized_fill_n<C3DAnimationInfo*, unsigned int,
//   C3DAnimationInfo>` 0x00A2AFA0, the copy constructor 0x00A2A4F0 (fill_len == 1), the element's virtual destructor
//   (slot 0, `push 0 ; call [vtbl]`), free 0x00BFEA14.
// Shape: the vector<CPhysicsMeshInfo> twin 0x00A2C960 with the 40-byte element and its out-of-line copy constructor.
// Layout: local models -- engine/C3DAnimationInfo.h / CMeshDataBankEntry.h are flat (no vptr, no base), and the
//   placement copy needs the real polymorphic pair (header debt, NOTES.md).
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

class C3DAnimationInfo : public CMeshDataBankEntry {       // 0x28 bytes (Ego_r.pdb)
public:
    C3DAnimationInfo(const C3DAnimationInfo& other);       // 0x00A2A4F0 (out of line)
    CIVCountedPointeeBase* Data;                           // +0x0C CIVCountedPointer<C3DAnimationInfo::CAnimationData>
    float Duration;                                        // +0x10
    float NonLoopingDuration;                              // +0x14
    float MovementVector[3];                               // +0x18 C3DVector
    float Rotation;                                        // +0x24
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
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                          // 0x00A2AF20 (same-TU body: lets the tag share the live atend slot)
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                          // 0x00A2AFA0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)
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
typedef C3DAnimationInfo _Elem;
template void vector<_Elem>::_M_insert_overflow(_Elem*, const _Elem&, const __false_type&, unsigned int, bool);
} // namespace std