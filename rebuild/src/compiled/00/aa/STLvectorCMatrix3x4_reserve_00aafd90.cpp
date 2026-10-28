// std::vector<CMatrix3x4>::reserve(unsigned int n) -- STLport reserve of the 48-byte matrix array: when the capacity is below n, move the elements into a fresh block of n (copy + free) or just allocate one
// Leaf: ?reserve@?$vector@VCMatrix3x4@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAFD90, length 153 bytes, `ret 4`.
// Callees: vector<CMatrix3x4>::_M_allocate_and_copy<const CMatrix3x4*> @ 0x00AAFCA0 (out of line), free @ 0x00BFEA14
//   (deallocate in _M_clear), malloc @ 0x00BFEA0E (allocate).
// Identity: called from C3DAnimFileSequenceChunk's constructor area / ReadFromFile (0x00AAF789, 0x00AAF9A3); the landed
//   ReadFromFile 0x00AAF850 names 0x00AAFD90 its CMatrix3x4 array's Reserve. The manifest's "C3DAnimSequence_Reserve"
//   is descriptive.
// Shape: STLport 4.6 reserve verbatim: capacity() and size() as signed /48 divides (0x2AAAAAAB, `sar edx,3`), compared
//   unsigned; `_M_start` non-null -> _M_allocate_and_copy + _M_clear (trivial _Destroy, null-tested free), else the
//   inline allocate; `_M_set(tmp, tmp + old_size, tmp + n)`.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

class CMatrix3x4 {                                          // 48 bytes: three rows of four floats
public:
    float m_row[3][4];
};

namespace std {

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

template <class V, class T, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
};

template <class T> inline void _Destroy(T*, T*) { }

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    protected:
    template <class ForwardIter>
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last);   // 0x00AAFCA0
    public:
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(T* s, T* f, T* e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    public:
    void reserve(size_type n);
};

template <class T, class A>
void vector<T, A>::reserve(size_type n)
{
    if (capacity() < n) {
        const size_type old_size = size();
        T* tmp;
        if (this->_M_start) {
            tmp = _M_allocate_and_copy(n, (const T*)this->_M_start, (const T*)this->_M_finish);
            _M_clear();
        }
        else {
            tmp = this->_M_end_of_storage.allocate(n);
        }
        _M_set(tmp, tmp + old_size, tmp + n);
    }
}

template void vector<CMatrix3x4>::reserve(unsigned int);

}   // namespace std