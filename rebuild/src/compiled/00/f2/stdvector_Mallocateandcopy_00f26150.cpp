// std::vector::_M_allocate_and_copy(unsigned int n, const T* first, const T* last) -- (vector<CArenaCreatureDef >) STLport growth helper: allocate n elements and copy-construct [first, last) into the block; returns it
// Leaf: ??$_M_allocate_and_copy@PBVCArenaCreatureDef@@@?$vector@VCArenaCreatureDef@@V?$allocator@VCArenaCreatureDef@@@std@@@std@@QAEPAVCArenaCreatureDef@@IPBV2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F26150, 77 bytes (+ int3 padding to 80), thiscall `ret 0Ch` (`this` is never read: the allocator is empty); speed-optimised TU.
// Identity (proven): vector<CArenaCreatureDef >::_M_allocate_and_copy<const CArenaCreatureDef*>; evidence: its copy constructor callee 0x00F25DD0 (CDefClassBase copy constructor 0x0046644C, then the vptr 0x012FB358; this TU has no RTTI, the vtable's locator slot is 0) copies exactly the members +0x28 CCharString, +0x2C dword, +0x30 CCharString, +0x34 dword -- of all Ego_r.pdb classes only CArenaCreatureDef has that retail layout (rebased, STLport containers 12 bytes) at the stride size 0x38; its only caller 0x00F25F90 is vector<CArenaCreatureDef>::operator= -- Ego_r.pdb CArenaWaveDef::Creatures +0x2C (vector<CArenaCreatureDef>); the operator= is called by 0x00F25F60, the CArenaWaveDef assignment.
// Label: the manifest's "std_vector_allocate_COpinionDeedReactionDef" is propagated (family H088: six def-vector allocate-and-copy helpers, one per element).
// Callees: malloc 0x00BFEA0E (allocator::allocate), CArenaCreatureDef::CArenaCreatureDef(const CArenaCreatureDef&) 0x00F25DD0 (the def's copy constructor).
// Layout: CArenaCreatureDef is 0x38 bytes in retail (Ego_r.pdb 0x38 bytes), the `imul` stride; opaque here.
// Shape: H059's model without the size pragma -- the speed-mode form of the same STLport `_M_allocate_and_copy` (`add esp, 4` after
//   malloc, `last` held in ebx, the loop aligned with `mov edi, edi`); allocate / _Construct / __uninitialized_copy stay `__forceinline`.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CArenaCreatureDef {                                           // retail 0x38 bytes
public:
    unsigned char _body[0x38];
    CArenaCreatureDef(const CArenaCreatureDef& other);                      // 0x00F25DD0
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
    template <class ForwardIter>
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
};

}   // namespace std

template CArenaCreatureDef* std::vector<CArenaCreatureDef >::_M_allocate_and_copy<const CArenaCreatureDef*>(unsigned int n, const CArenaCreatureDef* first, const CArenaCreatureDef* last);