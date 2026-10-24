// Behaviour test for 0x00617370 std::vector<unsigned __int64>::_M_insert_overflow(pos, x, __true_type, fill_len, atend):
// POD growth -- allocate old + max(old, fill_len), memmove the head, fill_len copies of x, memmove the tail unless atend,
// free the old block.
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

namespace stl {
struct __true_type { };
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)std::memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}
template <class OutputIter, class Size, class T> inline OutputIter fill_n(OutputIter first, Size n, const T& value)
{
    for (; n > 0; --n, ++first)
        *first = value;
    return first;
}
static int g_frees = 0;
template <class T> struct allocator {
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(std::malloc(n * sizeof(T))) : 0; }
    void deallocate(T* p, unsigned int) { ++g_frees; std::free(p); }
};
template <class T, class A = allocator<T> > class vector {
public:
    typedef T* pointer;
    pointer _M_start;
    pointer _M_finish;
    struct EndOfStorage : A { pointer _M_data; } _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear() { if (_M_start) _M_end_of_storage.deallocate(_M_start, (unsigned int)(_M_end_of_storage._M_data - _M_start)); }
    void _M_set(pointer s, pointer f, pointer e) { _M_start = s; _M_finish = f; _M_end_of_storage._M_data = e; }
    void _M_insert_overflow(pointer pos, const T& x, const __true_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (max)(old_size, fill_len);
        pointer new_start = _M_end_of_storage.allocate(len);
        pointer new_finish = (pointer)__copy_trivial(_M_start, pos, new_start);
        new_finish = fill_n(new_finish, fill_len, x);
        if (!atend)
            new_finish = (pointer)__copy_trivial(pos, _M_finish, new_finish);
        _M_clear();
        _M_set(new_start, new_finish, new_start + len);
    }
};
}

typedef stl::vector<u64> V;

int main()
{
    int ok = 1;
    V v;
    v._M_start = v._M_finish = v._M_end_of_storage._M_data = 0;
    u64 big = ((u64)0xdeadbeef << 32) | 0x01234567UL;
    // empty: nothing freed, one slot
    v._M_insert_overflow(v._M_finish, big, stl::__true_type(), 1, true);
    if (v.size() != 1 || v._M_start[0] != big || v._M_end_of_storage._M_data != v._M_start + 1 || stl::g_frees != 0) ok = 0;
    // append to a full vector of 1: capacity 2
    u64 two = 2;
    v._M_insert_overflow(v._M_finish, two, stl::__true_type(), 1, true);
    if (v.size() != 2 || v._M_start[1] != 2 || v._M_end_of_storage._M_data - v._M_start != 2 || stl::g_frees != 1) ok = 0;
    // fill 3 copies before element 1: len = 2 + max(2, 3) = 5, tail moved behind the fill
    u64 nine = 9;
    v._M_insert_overflow(v._M_start + 1, nine, stl::__true_type(), 3, false);
    if (v.size() != 5 || v._M_end_of_storage._M_data - v._M_start != 5 || v._M_start[0] != big || v._M_start[1] != 9 ||
        v._M_start[3] != 9 || v._M_start[4] != 2 || stl::g_frees != 2) ok = 0;
    std::free(v._M_start);
    if (!ok) { std::printf("00617370_TEST FAIL\n"); return 1; }
    std::printf("00617370_TEST PASS\n");
    return 0;
}