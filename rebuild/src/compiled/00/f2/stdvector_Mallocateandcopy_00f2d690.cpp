// std::vector::_M_allocate_and_copy(unsigned int n, const T* first, const T* last) -- (vector<CFeatDef >) STLport growth helper: allocate n elements and copy-construct [first, last) into the block; returns it
// Leaf: ??$_M_allocate_and_copy@PBVCFeatDef@@@?$vector@VCFeatDef@@V?$allocator@VCFeatDef@@@std@@@std@@QAEPAVCFeatDef@@IPBV2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F2D690, 77 bytes (+ int3 padding to 80), thiscall `ret 0Ch` (`this` is never read: the allocator is empty); speed-optimised TU.
// Identity (proven): vector<CFeatDef >::_M_allocate_and_copy<const CFeatDef*>; evidence: its copy constructor callee 0x00F2D270 (CDefClassBase copy constructor 0x0046644C, then the vptr 0x012FB778; this TU has no RTTI, the vtable's locator slot is 0) copies exactly the members +0x28 dword, +0x2C dword, +0x30 dword, +0x34 dword, +0x38 dword, +0x3C dword, +0x40 dword, +0x44 byte, +0x48 dword, +0x4C byte, +0x50 dword, +0x54 dword, +0x58 dword, +0x5C byte, +0x60 dword -- of all Ego_r.pdb classes only CFeatDef has that retail layout (rebased, STLport containers 12 bytes) at the stride size 0x64; its only caller 0x00F2D010 is vector<CFeatDef>::operator= -- Ego_r.pdb CScriptDef::Feats (vector<CFeatDef>); the operator= is called from 0x00F2AF6B inside CScriptDef::operator= 0x00F2A250.
// Label: the manifest's "std_vector_allocate_CFeatDef" is propagated (family H088: six def-vector allocate-and-copy helpers, one per element).
// Callees: malloc 0x00BFEA0E (allocator::allocate), CFeatDef::CFeatDef(const CFeatDef&) 0x00F2D270 (the def's copy constructor).
// Layout: CFeatDef is 0x64 bytes in retail (Ego_r.pdb 0x64 bytes), the `imul` stride; opaque here.
// Shape: H059's model without the size pragma -- the speed-mode form of the same STLport `_M_allocate_and_copy` (`add esp, 4` after
//   malloc, `last` held in ebx, the loop aligned with `mov edi, edi`); allocate / _Construct / __uninitialized_copy stay `__forceinline`.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CFeatDef {                                           // retail 0x64 bytes
public:
    unsigned char _body[0x64];
    CFeatDef(const CFeatDef& other);                      // 0x00F2D270
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

template CFeatDef* std::vector<CFeatDef >::_M_allocate_and_copy<const CFeatDef*>(unsigned int n, const CFeatDef* first, const CFeatDef* last);