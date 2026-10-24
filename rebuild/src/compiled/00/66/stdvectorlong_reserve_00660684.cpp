// std::vector<long>::reserve(unsigned int n) -- grow the capacity to n: copy the elements into a new block (or just allocate when empty), free the old block, keep the size
// Leaf: ?reserve@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00660684, 108 bytes, thiscall `ret 4`. Callers: the CTCVillage serialiser at
// 0x0065CED5 (`push [esi+68h]`: reserve(PlayersCrimes.size()) on a local vector<long>), 0x00766959, 0x007CBA0D, 0x008BC21C
// (other 4-byte POD vectors folded onto the same body).
// Callees: vector::_M_allocate_and_copy(n, start, finish) @ 0x00660213, free @ 0x00BFEA14 (_M_clear: the POD destroy
// is empty, the deallocate's null test stays), malloc @ 0x00BFEA0E (the empty-vector allocate, inline).
// Label correction: the catalog "CVector_Reserve" is descriptive; this is STLport 4.x `vector<long>::reserve`.
// Shape: STLport verbatim: `if (capacity() < n) { old_size = size(); if (_M_start) { tmp = _M_allocate_and_copy(n,
// _M_start, _M_finish); _M_clear(); } else tmp = allocate(n); _M_set(tmp, tmp + old_size, tmp + n); }`; n lives in ebp
// (no frame pointer in this TU).
// Opt: size TU (`optimize("s", on)`).
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

namespace std {

template <class T> class allocator {
public:
    allocator() { }
    __forceinline T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    __forceinline void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    unsigned int capacity() const { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class ForwardIter> T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last);   // 0x00660213
    __forceinline void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    __forceinline void _M_set(T* s, T* f, T* e) { this->_M_start = s; this->_M_finish = f; this->_M_end_of_storage._M_data = e; }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int old_size = size();
            T* tmp;
            if (this->_M_start) {
                tmp = _M_allocate_and_copy(n, this->_M_start, this->_M_finish);
                _M_clear();
            } else {
                tmp = this->_M_end_of_storage.allocate(n);
            }
            _M_set(tmp, tmp + old_size, tmp + n);
        }
    }
};

template void vector<long>::reserve(unsigned int);
}