// std::vector::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend) -- (vector<NInventory::CItem >) STLport growth insert: allocate size + max(size, fill_len), copy the head, construct fill_len copies of x, copy the tail unless atend, release the old block, adopt the new one
// Flags: /Gr
// Leaf: ?_M_insert_overflow@?$vector@VCItem@NInventory@@V?$allocator@VCItem@NInventory@@@std@@@std@@IAEXPAVCItem@NInventory@@ABV34@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C276C, 176 bytes (size-optimised TU with a frame pointer), thiscall, `ret 14h`.
// Identity (proven): std::vector<NInventory::CItem >::_M_insert_overflow (the non-POD overload); evidence: its fill_len == 1 constructor 0x005C08E6 is the CItem copy constructor (the bindings' ??0CItem@NInventory@@QAE@ABV01@@Z); its callers are vector<NInventory::CItem>::_M_fill_insert 0x005C2831 (agent379, landed) and the push_back 0x005C2996, the vector whose erase agent951 landed as vector<NInventory::CItem>::erase 0x005C1ECC
// Label: the manifest's "CActiveFile_ReserveAndInitialize" is a propagated / behavioural label (family J071: five 176-byte growth inserts, each its own element).
// Callees: allocator::allocate(n, const void* = 0) 0x005C0436 (out of line on _M_end_of_storage), __uninitialized_copy(first, last,
//   result, const __false_type&) 0x005C0DF0 (__fastcall, head and tail), the element copy constructor 0x005C08E6 (behind the placement
//   null test, fill_len == 1), __uninitialized_fill_n(first, n, x, const __false_type&) 0x005C0E1B (__fastcall), _M_clear() 0x005C1F1F.
// Shape: STLport 4.6 _vector.c `_M_insert_overflow` (non-POD, no EH) verbatim: `(max)` through its references, old size and then
//   new_finish in the frame. The constructor-less __false_type tags get no store and share the padding of the `atend` slot
//   ([ebp+1Bh]) because this TU compiles both callee bodies (STLport's loops, instantiated above) ahead of the growth insert.
// Element: NInventory::CItem (Ego_r.pdb 0x14 bytes)
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* __p) { return __p; }
namespace std {
struct __false_type { };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n, const void* = 0);    // 0x005C0436
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>                    // 0x005C0DF0 for this element (compiled ahead of the growth insert)
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>               // 0x005C0E1B for this element (compiled ahead of the growth insert)
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {          // STLport, 12 bytes
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    pointer _M_start;
    pointer _M_finish;
    _STLP_alloc_proxy<pointer, _Alloc> _M_end_of_storage;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear();                                     // 0x005C1F1F
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend)
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
}   // namespace std
template <class T> class CCountedPointer {               // 8 bytes: the object and its shared CCPPointerInfo count block
public:
    T*    Data;                                          // +0x0
    void* Info;                                          // +0x4 CCPPointerInfo*
};
namespace NInventory {
class CPerItemData;
class CItem {                                            // Ego_r.pdb 0x14 bytes
public:
    long                          ItemType;              // +0x00
    long                          NumberOfItems;         // +0x04
    CCountedPointer<CPerItemData> PPerItemData;          // +0x08
    long                          AverageBoughtPrice;    // +0x10
    CItem(const CItem& other);                           // 0x005C08E6 (implicit; emitted out of line in retail)
};
}
namespace std {
template NInventory::CItem* __uninitialized_copy<NInventory::CItem*, NInventory::CItem*>(NInventory::CItem*, NInventory::CItem*, NInventory::CItem*, const __false_type&);
template NInventory::CItem* __uninitialized_fill_n<NInventory::CItem*, unsigned int, NInventory::CItem >(NInventory::CItem*, unsigned int, const NInventory::CItem&, const __false_type&);
template void vector<NInventory::CItem >::_M_insert_overflow(NInventory::CItem*, const NInventory::CItem&, const __false_type&, unsigned int, bool);
}