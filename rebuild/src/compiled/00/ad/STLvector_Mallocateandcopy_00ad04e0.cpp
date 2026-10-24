// _STL::vector::_M_allocate_and_copy(unsigned int n, const CGroup* first, const CGroup* last) -- (vector<COptimisedVertex::CGroup>) STLport growth helper: allocate n 8-byte bone-weight groups and copy-construct [first, last) into the block, return it
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD04E0, length 78 bytes, thiscall `ret 0Ch` (`this` is never
//   read: the allocator is empty).
// Callees: malloc @ 0x00BFEA0E (allocator::allocate).
// Identity: the only caller (0x00AD403B) is vector<CGroup>::reserve 0x00AD4010, reached from the COptimisedVertex method
//   at 0x00ACA5F0 that reserves a local vector to the vertex's Groups map size (+0xA0) and copies each map entry's
//   {key, value} into it; the 8-byte element is COptimisedVertex::CGroup {BoneIndex, BoneWeight} (Ego_r.pdb, the model
//   of the landed _Push_heap 0x00AD0920 over the same vector). The manifest's "Vector_Reserve_8bytes_Variant2" is
//   descriptive.
// Shape: the landed STLport `_M_allocate_and_copy` (0x0075B800 / 0x00AAFCA0): `result = allocate(n)` then the inlined
//   `__uninitialized_copy(first, last, result, __false_type())` -- placement-new null test per element, the implicit
//   copy moving both words through an integer register (the float too), destination = first + (result - first).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

namespace QuadricOptimiserInternals {
class COptimisedVertex {
public:
    class CGroup {                                          // 8 bytes (Ego_r.pdb)
    public:
        unsigned long BoneIndex;                            // +0x0
        float         BoneWeight;                           // +0x4
    };
};
}
typedef QuadricOptimiserInternals::COptimisedVertex::CGroup CGroup;

namespace _STL {

struct __false_type { __false_type() { } };

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class T, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
};

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class InputIter, class ForwardIter>
inline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
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

}   // namespace _STL

template CGroup* _STL::vector<CGroup>::_M_allocate_and_copy<const CGroup*>(
    unsigned int n, const CGroup* first, const CGroup* last);