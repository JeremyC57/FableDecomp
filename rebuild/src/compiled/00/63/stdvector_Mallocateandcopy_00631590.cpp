// std::vector::_M_allocate_and_copy(unsigned int n, const T* first, const T* last) -- (vector<NUISystem::CUIStateDef >) STLport growth helper: allocate n elements and copy-construct [first, last) into the block; returns it
// Leaf: ??$_M_allocate_and_copy@PBVCUIStateDef@NUISystem@@@?$vector@VCUIStateDef@NUISystem@@V?$allocator@VCUIStateDef@NUISystem@@@std@@@std@@QAEPAVCUIStateDef@NUISystem@@IPBV23@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00631590, 77 bytes (+ int3 padding to 80), thiscall `ret 0Ch` (`this` is never read: the allocator is empty); speed-optimised TU.
// Identity (proven): vector<NUISystem::CUIStateDef >::_M_allocate_and_copy<const NUISystem::CUIStateDef*>; evidence: its copy constructor callee 0x006250B0 stores the vptr 0x0125871C of RTTI .?AVCUIStateDef@NUISystem@@ (after the inlined CParentDefClassBase copy, vptr 0x01230CBC), and the stride 0x7C is that class's retail size (Ego_r.pdb rebased: the parent-def base 4 bytes shorter, STLport containers 12 bytes); its only caller 0x006315E0 is vector<NUISystem::CUIStateDef>::operator= -- Ego_r.pdb CUIDef::States +0x44 (vector<NUISystem::CUIStateDef>); the operator= is called by CUIDef's copy constructor 0x006303C0 and CUIDef::operator= 0x00631720.
// Label: the manifest's "std::vector_uninitialized_copy_uistate" is propagated (family H088: six def-vector allocate-and-copy helpers, one per element).
// Callees: malloc 0x00BFEA0E (allocator::allocate), NUISystem::CUIStateDef::CUIStateDef(const NUISystem::CUIStateDef&) 0x006250B0 (the def's copy constructor).
// Layout: NUISystem::CUIStateDef is 0x7C bytes in retail (Ego_r.pdb 0x84 bytes; the parent-def base 4 bytes shorter, 1 STLport container 12 bytes, 16 in the PDB), the `imul` stride; opaque here.
// Shape: H059's model without the size pragma -- the speed-mode form of the same STLport `_M_allocate_and_copy` (`add esp, 4` after
//   malloc, `last` held in ebx, the loop aligned with `mov edi, edi`); allocate / _Construct / __uninitialized_copy stay `__forceinline`.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

namespace NUISystem {
class CUIStateDef {                                           // retail 0x7C bytes
public:
    unsigned char _body[0x7C];
    CUIStateDef(const CUIStateDef& other);                      // 0x006250B0
};
}

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

template NUISystem::CUIStateDef* std::vector<NUISystem::CUIStateDef >::_M_allocate_and_copy<const NUISystem::CUIStateDef*>(unsigned int n, const NUISystem::CUIStateDef* first, const NUISystem::CUIStateDef* last);