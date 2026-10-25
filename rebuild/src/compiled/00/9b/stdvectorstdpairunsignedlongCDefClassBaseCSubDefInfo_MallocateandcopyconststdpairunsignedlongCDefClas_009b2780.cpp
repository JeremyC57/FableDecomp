// std::vector<std::pair<unsigned long,CDefClassBase::CSubDefInfo> >::_M_allocate_and_copy<const std::pair<unsigned long,CDefClassBase::CSubDefInfo>*>(unsigned int n, first, last) -- allocate an n-element block of (sub-def CRC, CSubDefInfo) entries and copy-construct [first, last) into it
// Leaf: ??$_M_allocate_and_copy@PBU?$pair@KVCSubDefInfo@CDefClassBase@@@std@@@?$vector@U?$pair@KVCSubDefInfo@CDefClassBase@@@std@@V?$allocator@U?$pair@KVCSubDefInfo@CDefClassBase@@@std@@@2@@std@@QAEPAU?$pair@KVCSubDefInfo@CDefClassBase@@@1@IPBU21@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B2780, 86 bytes, thiscall `ret 0Ch` (this unused). Catalog name
// "Deque_AllocateBuffer" is wrong: the body is STLport vector::_M_allocate_and_copy (caller: vector::reserve @ 0x009B4D80).
// Identity: the reserve 0x009B4D80 is called on `lea ebp,[ebx+29h]` by CParentDefClassBase::InternalLoadBinary
//   0x009AED70 -- Ego_r.pdb CParentDefClassBase::SubDefInfoMap (+41 = 0x29) is CVectorMap<unsigned long,
//   CDefClassBase::CSubDefInfo, ...>, whose entry pair<unsigned long, CSubDefInfo {DefIndex, OriginalParentDefIndex}> is
//   the 12-byte element; FableWin.pdb has ?reserve@?$vector@U?$pair@KVCSubDefInfo@CDefClassBase@@@std... and
//   ?DeallocateUnusedMemory@?$CArray@U?$pair@KVCSubDefInfo@CDefClassBase@@@std... (the other reserve caller 0x009B79B0).
//   Renamed from the stand-in _STL::vector<SRecord12> (pair<unsigned long, SValue8>); FINDINGS chunk 410 asked to unify
//   the element with 0x009B4D80.
// Callee: malloc @ 0x00BFEA0E (allocator::allocate, `n != 0 ? malloc(n * 12) : 0`).
// Shape: STLport 4.x verbatim: allocate, then __uninitialized_copy(first, last, result, __false_type()) = the
// placement-new loop with VC7.1's null check (`__forceinline`: /Ob2 leaves the plain inline out of line for
// this instantiation); cur is strength-reduced to `first + (result - first)`. No EH frame (the
// element copy cannot throw). The pair's memberwise user copy ctor (STLport declares one) gives the three single-word
// copies (a plain 12-byte record copies through a block move instead).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __false_type { __false_type() { } };

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;             // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    template <class ForwardIter>
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
};

}

class CDefClassBase {
public:
    class CSubDefInfo {                                     // Ego_r.pdb CDefClassBase::CSubDefInfo (8 bytes)
    public:
        long DefIndex;                                      // +0x00
        long OriginalParentDefIndex;                        // +0x04
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}
typedef std::pair<unsigned long, CDefClassBase::CSubDefInfo> SSubDefEntry;   // CVectorMap<unsigned long, CSubDefInfo> entry (12 bytes)

template SSubDefEntry* std::vector<SSubDefEntry>::_M_allocate_and_copy<const SSubDefEntry*>(
    unsigned int n, const SSubDefEntry* first, const SSubDefEntry* last);