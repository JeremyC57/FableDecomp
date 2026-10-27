// std::vector::_M_insert_overflow(std::pair<long, long>* __position, const std::pair<long, long>& __x, const __false_type&, unsigned int __fill_len, bool __atend) -- (vector<std::pair<long, long>>) growth insert
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@IAEXPAU?$pair@JJ@2@ABU32@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059400C, 181 bytes, thiscall, `ret 14h`, ebp frame.
// Identity (proven): its callers are the push_back 0x005947B6 (bound ?push_back@?$vector@U?$pair@JJ@std@@@std@@... from CTCInventoryTrade::InitialiseBuyableItems 0x005A30A1, whose FableWin.pdb signature takes vector<pair<long,long> >&; also used by CTCInventoryWeapons::RefreshAugmentationsLeftDescriptions on GetAllItems' vector<pair<long,long> >) and CTCQuickAccessMenu::GetItemsListUI 0x006276F0.
// Callees: malloc 0x00BFEA0E, std::__uninitialized_copy 0x00592D43 and std::__uninitialized_fill_n 0x00513A80 (`__fastcall`), free 0x00BFEA14.
// Shape (family M147, representative 0x0059400C): the M124 model (STLport 4.6, range helpers out of line with visible
//   bodies, tags homed in the dead __atend slot) in a frame-pointer size module: `optimize("y", off)` + `optimize("s", on)`;
//   the old size lives in the frame because (max) takes it by reference.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __true_type { };
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit copy
};

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

// the two range helpers stay out of line (`__declspec(noinline)`), but their bodies are visible: VC7.1 then knows the
// empty tag's address is not kept and homes the tag temporaries in the dead __atend slot, as retail does
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

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

template void vector<std::pair<long, long> >::_M_insert_overflow(std::pair<long, long>*, const std::pair<long, long>&, const __false_type&, unsigned int, bool);

} // namespace std