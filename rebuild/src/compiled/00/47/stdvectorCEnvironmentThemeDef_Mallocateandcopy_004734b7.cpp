// std::vector<CEnvironmentThemeDef>::_M_allocate_and_copy(unsigned int n, const CEnvironmentThemeDef* first, const CEnvironmentThemeDef* last) -- STLport growth helper: allocate n CEnvironmentThemeDefs and copy-construct [first, last) into the block; returns it
// Leaf: ??$_M_allocate_and_copy@PBVCEnvironmentThemeDef@@@?$vector@VCEnvironmentThemeDef@@V?$allocator@VCEnvironmentThemeDef@@@std@@@std@@QAEPAVCEnvironmentThemeDef@@IPBV2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004734B7, 77 bytes, thiscall `ret 0Ch` (`this` is never read: the allocator is empty); size-optimised def TU.
// Identity (proven): vector<CEnvironmentThemeDef>::_M_allocate_and_copy<const CEnvironmentThemeDef*>; evidence: its copy constructor callee 0x0047318D stores the vptr of RTTI .?AVCEnvironmentThemeDef@@ (after the CParentDefClassBase copy constructor 0x0046EE98), the element stride 0xC4 is that class's retail size, and its only caller 0x00472EDA is vector<CEnvironmentThemeDef>::operator= (assigning CEnvironmentThemeDaySetDef::TimeTheme).
// Label: the manifest's "EnvironmentTheme_AllocCopy" is descriptive (family H059: six def-vector allocate-and-copy helpers, one per element).
// Callees: malloc 0x00BFEA0E (allocator::allocate), CEnvironmentThemeDef::CEnvironmentThemeDef(const CEnvironmentThemeDef&) 0x0047318D (the def's copy constructor).
// Layout: CEnvironmentThemeDef is 0xC4 bytes in retail (Ego_r.pdb 0xC8 bytes: the CParentDefClassBase base: 4 bytes shorter), the `imul` stride; opaque here.
// Shape: the landed STLport `_M_allocate_and_copy` (0x00AD04E0): `result = allocate(n)` then the inlined `__uninitialized_copy(first, last,
//   result, __false_type())` -- the placement-new null test per element, the destination walking as `edi + esi`. Under the def TU's
//   size optimisation VC7.1 keeps the plain-`inline` allocate / __uninitialized_copy out of line, so they (and _Construct) are `__forceinline`.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CEnvironmentThemeDef {                                               // retail 0xC4 bytes
public:
    unsigned char _body[0xC4];
    CEnvironmentThemeDef(const CEnvironmentThemeDef& other);                              // 0x0047318D
};

namespace std {

struct __false_type { __false_type() { } };

template <class T> class allocator {
public:
    allocator() { }
    __forceinline T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class T, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
};

template <class T1, class T2>
__forceinline void _Construct(T1* p, const T2& value)
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
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    protected:
    template <class ForwardIter>
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
    public:
};

}   // namespace std

template CEnvironmentThemeDef* std::vector<CEnvironmentThemeDef >::_M_allocate_and_copy<const CEnvironmentThemeDef*>(unsigned int n, const CEnvironmentThemeDef* first, const CEnvironmentThemeDef* last);