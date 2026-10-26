// Behaviour test for 0x004AD220 std::vector<CThingBuilding*>::push_back: in-place store while there is room, the
// STLport growth path (size + max(size, 1)) when full, values and order kept.
#include <cstdio>
// (source) Leaf: ?push_back@?$vector@PAVCThingBuilding@@V?$allocator@PAVCThingBuilding@@@std@@@std@@QAEXABQAVCThingBuilding@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AD220, 50 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: landed as _global::PushBack_dword_004ad220 over a CRetailVector_dword stand-in. Its growth callee 0x004AC530 is
//   vector<CThingBuilding*>::_M_insert_overflow (shared with GetAllThings / GetAllLeafThings<CThingBuilding, vector<CThingBuilding*>,
//   CThingFilter_None> 0x0066E5A0 / 0x004AD700), and Ego_r/FableWin both name
//   ?push_back@?$vector@PAVCThingBuilding@@V?$allocator@PAVCThingBuilding@@@std@@@std@@QAEXABQAVCThingBuilding@@@Z. Its twin
//   0x004AD260 (growth callee 0x004AC610 = vector<const CThingBuilding*>) is the const-element push_back.
// Callers: 0x0050345A and 0x005034B3.
// Shape: the __true_type tag is homed in the dead x argument slot (`lea edx,[esp+0Ch]` after the two pushes): the verbatim
//   STLport overflow body sits in the TU under auto_inline(off) + noinline, so VC7.1 sees the tag is never read.
inline void* operator new(unsigned int, void* where) { return where; }
namespace std { struct __true_type { __true_type() {} }; }
using std::__true_type;
class CThingBuilding;
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6
namespace std {
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
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
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
protected:
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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);   // 0x004AC530 for vector<CThingBuilding*>
};
#pragma auto_inline(off)
template <class _Tp, class _Alloc> __declspec(noinline)
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                                             bool __atend)
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
#pragma auto_inline(on)
}

template void std::vector<CThingBuilding*>::push_back(CThingBuilding* const&);

int main()
{
    std::vector<CThingBuilding*> v;
    v._M_start = 0; v._M_finish = 0; v._M_end_of_storage._M_data = 0;
    CThingBuilding* items[5];
    for (int i = 0; i < 5; ++i) items[i] = (CThingBuilding*)(0x1000 + 0x10 * i);
    bool ok = true;
    v.push_back(items[0]);                                  // grows to 1
    ok = ok && v.size() == 1 && v._M_end_of_storage._M_data - v._M_start == 1;
    v.push_back(items[1]);                                  // grows to 2
    ok = ok && v.size() == 2 && v._M_end_of_storage._M_data - v._M_start == 2;
    v.push_back(items[2]);                                  // grows to 4
    ok = ok && v.size() == 3 && v._M_end_of_storage._M_data - v._M_start == 4;
    v.push_back(items[3]);                                  // room: stored in place
    ok = ok && v.size() == 4 && v._M_end_of_storage._M_data - v._M_start == 4;
    v.push_back(items[4]);                                  // grows to 8
    ok = ok && v.size() == 5 && v._M_end_of_storage._M_data - v._M_start == 8;
    for (int k = 0; k < 5; ++k) ok = ok && v._M_start[k] == items[k];
    if (!ok) { std::printf("004ad220_TEST FAIL size=%u\n", v.size()); return 1; }
    std::printf("004ad220_TEST PASS\n");
    return 0;
}
