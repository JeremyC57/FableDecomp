// CLocalDetailCacheMap::CObjectTypeCollectionPalette::GetIndexFromType(const CLocalDetailObjectCollectionType*) -- index of an object type in the palette, appending it when it is not there yet
// Leaf: ?GetIndexFromType@CObjectTypeCollectionPalette
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDEF40, 121 bytes (+ int3 padding; `ret 4`).
// Callees: STLport vector<const CLocalDetailObjectCollectionType*>::_M_insert_overflow(pointer, const T&, const __true_type&,
//   size_type, bool) @ 0x00BDFC00.
// Layout (retail): the palette is the STLport vector itself (start +0, finish +4, end of storage +8).
// Shape: the search is `for (i < size()) if ((*this)[i] == type) return i;` (the element pointer strength-reduced, size re-read each
//   iteration); STLport push_back is inlined: the null-tested placement copy at finish, else _M_insert_overflow with the empty
//   __true_type temporary and the value both addressed in the `type` argument slot.
#pragma warning(disable: 4291)
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
class CLocalDetailObjectCollectionType;
namespace std {
struct __true_type { __true_type() { } };
template <class T> struct allocator { };
template <class T, class A = allocator<T> > struct vector {  // STLport vector: start, finish, end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() { return end() - begin(); }
    T& operator[](unsigned int n) { return *(begin() + n); }
    void _M_insert_overflow(T* position, const T& x, const __true_type& tag, unsigned int fillLength, bool atEnd);   // 0x00BDFC00
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else {
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
        }
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);

#pragma auto_inline(off)
// Same-TU body of the vector growth member (_M_insert_overflow, instantiated in retail's TU): the new storage receives the value
// read through its const reference; the empty tag is never touched.
template <class T, class A>
__declspec(noinline) void vector<T, A>::_M_insert_overflow(T* position, const T& x, const __true_type& tag, unsigned int fillLength, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_s7(this, fillLength);
    if (p)
        new (p) T(x);
}
#pragma auto_inline(on)
}
class CLocalDetailCacheMap { public:
    class CObjectTypeCollectionPalette { public:
        std::vector<const CLocalDetailObjectCollectionType*> Types;   // +0x00
        long GetIndexFromType(const CLocalDetailObjectCollectionType* type);
    };
};

long CLocalDetailCacheMap::CObjectTypeCollectionPalette::GetIndexFromType(const CLocalDetailObjectCollectionType* type)
{
    for (unsigned long i = 0; i < Types.size(); ++i) {
        if (Types[i] == type)
            return i;
    }
    Types.push_back(type);
    return Types.size() - 1;
}