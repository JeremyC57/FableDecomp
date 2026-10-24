// std::vector<CQuestMenuEntry_0056d490*>::vector(const vector& x) -- STLport copy constructor of a vector of 4-byte pointers: allocate size() words and memmove the source range
// Leaf: ??0?$vector
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056D490, 95 bytes, thiscall `ret 4`. The mapped value of a local
//   map<unsigned long, vector<...*> > in CQuestManager::UpdateUI_QuestMenu (0x0056AD40): called from that map's
//   node value copy (pair copy ctor 0x0056DAB0) and its insert/create-node bodies (0x0056D5C0, 0x0056DEB0).
//   The element is a 4-byte trivially copyable type (pointer-sized, memmove'd); its class is not recoverable
//   from these bytes, hence the placeholder name. The catalog label "Vector_CopyConstructor" is descriptive.
// Callees: malloc @ 0x00BFEA0E (allocator::allocate, null for n == 0), memmove @ 0x00BFEAE6.
// Shape: STLport 4.x verbatim: `_Vector_base(x.size(), x.get_allocator())` (size computed first, the three
//   words zeroed, allocate, start/finish/end set), then `_M_finish = __uninitialized_copy(x._M_start,
//   x._M_finish, _M_start, __true_type())` -> `__copy_trivial`: `last == first ? result : (char*)memmove(result,
//   first, last - first) + (last - first)`.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

class CQuestMenuEntry_0056d490;

namespace std {

struct __true_type { };
struct __false_type { };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template <class _Tp>
inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return (const _Alloc&)this->_M_end_of_storage; }
    vector(const vector& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish,
                                               this->_M_start, __true_type());
    }
};

template vector<CQuestMenuEntry_0056d490*>::vector(const vector<CQuestMenuEntry_0056d490*>&);

}