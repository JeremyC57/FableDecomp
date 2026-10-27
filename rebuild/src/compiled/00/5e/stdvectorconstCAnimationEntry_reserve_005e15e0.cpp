// std::vector<const CAnimationEntry*>::reserve(unsigned int __n) -- grow the capacity to n, moving the elements into a fresh block
// Leaf: ?reserve@?$vector@PBVCAnimationEntry@@V?$allocator@PBVCAnimationEntry@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E15E0, 179 bytes, thiscall, `ret 4`.
// Identity (probable): its callers 0x005DCC00 / 0x005DCD10 are CAnimationSet::GetRandomAnim / GetRandomLoadedAnim (FindFirstAnimWithName, then a random pick among the matching entries collected into local vectors reserved to 5; GetRandomLoadedAnim returns a const CAnimationEntry*); FableWin.pdb has ?reserve@?$vector@PBVCAnimationEntry@@... and CAnimationSet::GetAnimsWithNameStartingWith fills a vector<const CAnimationEntry*>.
// Callees: malloc 0x00BFEA0E (allocator::allocate, both arms), memmove 0x00BFEAE6 (__copy_trivial), free 0x00BFEA14 (_M_clear).
// Shape (family M159, representative 0x005E15E0): STLport 4.6 `reserve` verbatim with `_M_allocate_and_copy` inlined -- the POD
//   copy is `__copy_trivial` (memmove when the range is non-empty); the new block spilled to the `push ecx` word. Speed TU.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                    // 0x00BFEA14
extern "C" void* __cdecl memmove(void* d, const void* s, unsigned int n);   // 0x00BFEAE6
class CAnimationEntry;
namespace std {
struct __true_type { };
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _Tp> inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __true_type());
        return __result;
    }
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void reserve(size_type __n);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(size_type __n)
{
    if (capacity() < __n) {
        const size_type __old_size = size();
        pointer __tmp;
        if (this->_M_start) {
            __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
            _M_clear();
        } else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}
}
template void std::vector<const CAnimationEntry*>::reserve(unsigned int);