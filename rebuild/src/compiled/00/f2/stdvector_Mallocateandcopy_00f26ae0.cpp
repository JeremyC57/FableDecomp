// std::vector::_M_allocate_and_copy(unsigned int n, const T* first, const T* last) -- (vector<CArenaWaveDef >) STLport growth helper: allocate n elements and copy-construct [first, last) into the block; returns it
// Leaf: ??$_M_allocate_and_copy@PBVCArenaWaveDef@@@?$vector@VCArenaWaveDef@@V?$allocator@VCArenaWaveDef@@@std@@@std@@QAEPAVCArenaWaveDef@@IPBV2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F26AE0, 77 bytes (+ int3 padding to 80), thiscall `ret 0Ch` (`this` is never read: the allocator is empty); speed-optimised TU.
// Identity (proven): vector<CArenaWaveDef >::_M_allocate_and_copy<const CArenaWaveDef*>; evidence: its copy constructor callee 0x00F25C60 (CDefClassBase copy constructor 0x0046644C, then the vptr 0x012FB2F0; this TU has no RTTI, the vtable's locator slot is 0) copies exactly the members +0x28 dword, +0x2C container, +0x38 byte -- of all Ego_r.pdb classes only CArenaWaveDef has that retail layout (rebased, STLport containers 12 bytes) at the stride size 0x3C; its only caller 0x00F26920 is vector<CArenaWaveDef>::operator= -- Ego_r.pdb CArenaRoundDef::Waves +0x2C (vector<CArenaWaveDef>); the operator= is called by 0x00F268F0, the CArenaRoundDef assignment.
// Label: the manifest's "std_vector_allocate_CParentDefClassBase" is propagated (family H088: six def-vector allocate-and-copy helpers, one per element).
// Callees: malloc 0x00BFEA0E (allocator::allocate), CArenaWaveDef::CArenaWaveDef(const CArenaWaveDef&) 0x00F25C60 (the def's copy constructor).
// Layout: CArenaWaveDef is 0x3C bytes in retail (Ego_r.pdb 0x40 bytes; 1 STLport container 12 bytes, 16 in the PDB), the `imul` stride; opaque here.
// Shape: H059's model without the size pragma -- the speed-mode form of the same STLport `_M_allocate_and_copy` (`add esp, 4` after
//   malloc, `last` held in ebx, the loop aligned with `mov edi, edi`); allocate / _Construct / __uninitialized_copy stay `__forceinline`.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CArenaWaveDef {                                           // retail 0x3C bytes
public:
    unsigned char _body[0x3C];
    CArenaWaveDef(const CArenaWaveDef& other);                      // 0x00F25C60
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

template CArenaWaveDef* std::vector<CArenaWaveDef >::_M_allocate_and_copy<const CArenaWaveDef*>(unsigned int n, const CArenaWaveDef* first, const CArenaWaveDef* last);