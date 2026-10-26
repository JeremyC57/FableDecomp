// Behaviour test for 00996b60 (CMemoryDataOutputStream::Clear): a buffer with capacity is emptied and its storage freed (all three words zero); an unallocated buffer is left alone; the write position is rewound either way.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_frees = 0;
static void* g_lastFreed = 0;
static void test_free(void* p) { ++g_frees; g_lastFreed = p; std::free(p); }
inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)std::memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}
template <class T> inline void swap_values(T& a, T& b) { T tmp = a; a = b; b = tmp; }
template <class T> struct allocator {
    void deallocate(T* p, unsigned int) { test_free(p); }
};
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class T> struct vector : _Vector_base<T> {
    unsigned int capacity() const { return this->_M_end_of_storage._M_data - this->_M_start; }
    T* erase(T* first, T* last)
    {
        T* i = (T*)__copy_trivial(last, this->_M_finish, first);
        this->_M_finish = i;
        return first;
    }
    void clear() { erase(this->_M_start, this->_M_finish); }
    void swap(vector& x)
    {
        swap_values(this->_M_start, x._M_start);
        swap_values(this->_M_finish, x._M_finish);
        swap_values(this->_M_end_of_storage._M_data, x._M_end_of_storage._M_data);
    }
};
template <class T> class CArray : public vector<T> { public:
    void Clear()
    {
        if (this->capacity() != 0) {
            this->clear();
            CArray<T> empty;
            this->swap(empty);
        }
    }
};
class CMemoryDataOutputStream { public:
    virtual ~CMemoryDataOutputStream() { }
    void*         m_lineTerminator;
    CArray<char>  Data;
    unsigned long CurrentPosition;
    CMemoryDataOutputStream() : m_lineTerminator(0), CurrentPosition(0) { }
    void Clear();
};
void CMemoryDataOutputStream::Clear()
{
    Data.Clear();
    CurrentPosition = 0;
}
int main()
{
    CMemoryDataOutputStream stream;
    stream.CurrentPosition = 9;
    stream.Clear();
    if (g_frees != 0 || stream.CurrentPosition != 0 || stream.Data._M_start != 0) { std::printf("00996b60_TEST FAIL\n"); return 1; }
    char* block = static_cast<char*>(std::malloc(16));
    std::memcpy(block, "abcdefghij", 10);
    stream.Data._M_start = block;
    stream.Data._M_finish = block + 10;
    stream.Data._M_end_of_storage._M_data = block + 16;
    stream.CurrentPosition = 10;
    stream.Clear();
    if (g_frees != 1 || g_lastFreed != block || stream.CurrentPosition != 0) { std::printf("00996b60_TEST FAIL\n"); return 1; }
    if (stream.Data._M_start != 0 || stream.Data._M_finish != 0 || stream.Data._M_end_of_storage._M_data != 0) { std::printf("00996b60_TEST FAIL\n"); return 1; }
    std::printf("00996b60_TEST PASS\n");
    return 0;
}