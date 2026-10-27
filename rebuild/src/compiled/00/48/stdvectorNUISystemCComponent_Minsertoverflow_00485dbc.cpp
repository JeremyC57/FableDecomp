// std::vector<NUISystem::CComponent*>::_M_insert_overflow(NUISystem::CComponent** position, NUISystem::CComponent* const& x, const __true_type&, unsigned int fillLength, bool atEnd) -- STLport POD growth insert: allocate size + max(size, n), copy the head, n copies of x, the tail unless atEnd, free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@PAVCComponent@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00485DBC, 137 bytes, thiscall `ret 14h`; size TU with a frame pointer.
// Identity (proven): reloc_bindings names it ?_M_insert_overflow@?$vector@PAVCComponent@NUISystem@@...ABU__true_type@2@I_N@Z (vector<NUISystem::CComponent*>, among
//   propagated stand-ins); caller push_back 0x00486024 / CDynamicList::GetDataFromDef 0x00541390.
// Callees: malloc 0x00BFEA0E (allocate), std::__copy_trivial 0x00405BA0 (__fastcall, head and tail), fill_n 0x0048514C (__fastcall),
//   free 0x00BFEA14.
// Shape (family K105, representative 0x00485DBC): the landed __true_type overload 0x007A91A0 (family F005) in a frame/size TU:
//   allocate and _M_clear/_M_set inline, __copy_trivial and fill_n called; new_start in ebx, len in edi.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
namespace NUISystem { class CComponent; }
namespace std {
struct __true_type { };
template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
void* __fastcall __copy_trivial(const void* __first, const void* __last, void* __result);   // 0x00405BA0
template <class _OutputIter, class _Size, class _Tp>
_OutputIter __fastcall fill_n(_OutputIter __first, _Size __n, const _Tp& __value);         // 0x0048514C
template <class _Tp> class allocator {
public:
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    __forceinline void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    __forceinline size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    __forceinline void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    __forceinline void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);

        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
        // handle insertion
        __new_finish = fill_n(__new_finish, __fill_len, __x);
        if (!__atend)
            // copy remainder
            __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};
template void vector<NUISystem::CComponent* >::_M_insert_overflow(NUISystem::CComponent**, NUISystem::CComponent* const&, const __true_type&, unsigned int, bool);
}