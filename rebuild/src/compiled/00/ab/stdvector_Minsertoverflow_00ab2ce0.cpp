// std::vector::_M_insert_overflow(CSequenceInfo* __position, const CSequenceInfo& __x, const __false_type&, unsigned int __fill_len, bool __atend) -- (vector<N3DAnimationBuilder::CSequenceInfo>) STLport grow path: allocate size + max(size, fill_len), copy the prefix, construct fill_len copies of x, copy the suffix unless atend, free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCSequenceInfo@N3DAnimationBuilder@@V?$allocator@VCSequenceInfo@N3DAnimationBuilder@@@std@@@std@@IAEXPAVCSequenceInfo@N3DAnimationBuilder@@ABV34@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB2CE0, 310 bytes, thiscall, `ret 14h`.
// Identity (proven, two sources): its only caller (0x00AB2F5A) is the landed vector<N3DAnimationBuilder::CSequenceInfo>
//   ::_M_fill_insert 0x00AB2E40, which binds exactly this spelling (reloc_bindings.tsv); the 52-byte stride (0x4EC4EC4F
//   reciprocal, sar 4; `imul 34h`; 13-dword `rep movsd` copies) is Ego_r.pdb CSequenceInfo {long OriginalIndex;
//   CMatrix3x4 GeneratedTransform;}. `_M_insert_overflow` is protected in STLport 4.6 `_vector.h` (IAE).
// Label: the catalog's "Vector_Reallocate_0x34Bytes" is descriptive.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14 (everything else inline).
// Shape: the landed vector<CLandscapeTheme> twin 0x00B6C220 (STLport 4.6 `_M_insert_overflow(pos, x, const __false_type&,
//   fill_len, atend)` verbatim over a trivially copyable, trivially destructible record): the implicit copy is `rep movsd`,
//   the _Destroy loop of _M_clear vanishes, `__forceinline` __uninitialized_copy keeps both copies inline, max() through
//   references (`lea ecx,[esp+28h] ; jb ; lea ecx,[esp+10h]`).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
class CMatrix3x4 {                              // Ego_r.pdb, 48 bytes
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

namespace N3DAnimationBuilder {
class CSequenceInfo {                           // Ego_r.pdb (class, V-mangled), 52 bytes, trivially copyable
public:
    long OriginalIndex;                         // +0x0
    CMatrix3x4 GeneratedTransform;              // +0x4
};
}
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);
    public:
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
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
template void vector<N3DAnimationBuilder::CSequenceInfo>::_M_insert_overflow(N3DAnimationBuilder::CSequenceInfo*,
    const N3DAnimationBuilder::CSequenceInfo&, const __false_type&, unsigned int, bool);
}