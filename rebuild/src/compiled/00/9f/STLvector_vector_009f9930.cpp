// _STL::vector::vector(const vector&) -- STLport copy constructor of CProfileTreeEntry::Children (vector<CProfileManager::CProfileTreeEntry*>): allocate x.size() pointers, then copy them with the trivial memmove copy
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F9930, 96 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: called from list<CProfileTreeEntry>::insert @ 0x009F9BA0 on the node's `+0x4C` (Ego_r.pdb CProfileTreeEntry::Children
//   at +76) while copy-constructing a tree entry. The chunk label "std_vector_copy_constructor" is right in kind.
// Callees: 0x00BFEA0E malloc (STLport malloc allocator, `n != 0 ? malloc(n * 4) : 0`), 0x00BFEAE6 memmove.
// Shape: STLport 4.x `vector(const vector& x) : _Vector_base(x.size(), x.get_allocator()) { _M_finish =
//   __uninitialized_copy(x.begin(), x.end(), _M_start, _IsPODType()); }` with the base constructor and __copy_trivial
//   (`last == first ? result : (char*)memmove(result, first, last - first) + (last - first)`) both inlined; x's two pointers
//   are re-read after the malloc.
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

namespace _STL {

struct __true_type { __true_type() {} };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
              + ((const char*)__last - (const char*)__first);
}

template <class _Tp>
inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator<_Tp>&) {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    const _Tp* begin() const { return this->_M_start; }
    const _Tp* end() const { return this->_M_finish; }

    vector(const vector<_Tp, _Alloc>& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy(__x.begin(), __x.end(), this->_M_start, __true_type());
    }
};

}

class CProfileManager {
public:
    struct CProfileTreeEntry;
};

template _STL::vector<CProfileManager::CProfileTreeEntry*>::vector(const _STL::vector<CProfileManager::CProfileTreeEntry*>& __x);