// std::vector::_M_allocate_and_copy(unsigned int n, const T* first, const T* last) -- (vector<CConversation >) STLport growth helper: allocate n elements and copy-construct [first, last) into the block; returns it
// Leaf: ??$_M_allocate_and_copy@PBVCConversation@@@?$vector@VCConversation@@V?$allocator@VCConversation@@@std@@@std@@QAEPAVCConversation@@IPBV2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E54E40, 77 bytes (+ int3 padding to 80), thiscall `ret 0Ch` (`this` is never read: the allocator is empty); speed-optimised TU.
// Identity (proven): vector<CConversation >::_M_allocate_and_copy<const CConversation*>; evidence: its copy constructor callee 0x00E54CA0 (CDefClassBase copy constructor 0x0046644C, then the vptr 0x012E5FF0; this TU has no RTTI, the vtable's locator slot is 0) copies exactly the members +0x28 dword, +0x2C container, +0x38 container, +0x44 container, +0x50 container -- of all Ego_r.pdb classes only CConversation has that retail layout (rebased, STLport containers 12 bytes) at the stride size 0x5C; its only caller 0x00E54AA0 is vector<CConversation>::operator= -- Ego_r.pdb CScriptDef::BookReactions (and CV_BookCollectingScript::BookReactions, vector<CConversation>); the operator= is called by CScriptDef::operator= 0x00F2A250 and 0x00E54990.
// Label: the manifest's "std_vector_CConversation_CopyAssign" is propagated (family H088: six def-vector allocate-and-copy helpers, one per element).
// Callees: malloc 0x00BFEA0E (allocator::allocate), CConversation::CConversation(const CConversation&) 0x00E54CA0 (the def's copy constructor).
// Layout: CConversation is 0x5C bytes in retail (Ego_r.pdb 0x6C bytes; 4 STLport containers 12 bytes, 16 in the PDB), the `imul` stride; opaque here.
// Shape: H059's model without the size pragma -- the speed-mode form of the same STLport `_M_allocate_and_copy` (`add esp, 4` after
//   malloc, `last` held in ebx, the loop aligned with `mov edi, edi`); allocate / _Construct / __uninitialized_copy stay `__forceinline`.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CConversation {                                           // retail 0x5C bytes
public:
    unsigned char _body[0x5C];
    CConversation(const CConversation& other);                      // 0x00E54CA0
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

template CConversation* std::vector<CConversation >::_M_allocate_and_copy<const CConversation*>(unsigned int n, const CConversation* first, const CConversation* last);