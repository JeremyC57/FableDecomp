// std::vector<C4DVector>::push_back(const C4DVector& value) -- STLport push_back of a 16-byte vector: copy-construct at finish and advance when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@VC4DVector@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098D820, length 80 bytes (70 + int3 padding), `ret 4`.
// Callees: std::vector<C4DVector>::_M_insert_overflow(pointer, const C4DVector&, const __false_type&, size_type, bool)
// @ 0x0098D240 (thiscall, five arguments: finish, value, the POD tag, 1, true).
// Caller: CShaderRenderManager::SetPixelShaderConstant @ 0x0098B390 (the PSConstTable CArray<C4DVector>).
// Label: the manifest's "std_vector_PushBack_Fixed" is descriptive.
// Shape: STLport 4.6 push_back -- `_Copy_Construct(_M_finish, x)` is placement new (null test) with C4DVector's
// memberwise copy; the overflow arm passes `_IsPODType()` (a __false_type temporary homed on the value argument slot).
#include <new>
#include <stddef.h>

class C4DVector {
public:
    float X, Y, Z, W;
};

namespace std {

struct __false_type { __false_type() { } };

template <class T> class allocator { public: allocator() { } };

template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
};

template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};

template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_t fillLen, bool atEnd = false);   // 0x0098D240
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) T(x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
    }
};

}   // namespace std

template void std::vector<C4DVector>::push_back(const C4DVector&);