#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test: the vector<const CAnimationEntry*> copy constructor allocates size() slots and copies the pointers.
struct CAnimationEntry { int Id; };
template <class T> struct TestVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    TestVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    TestVector(unsigned int n, int)   // _Vector_base(n, allocator) 0x0062F5D0
    {
        _M_start = n ? (T*)std::malloc(n * sizeof(T)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    TestVector(const TestVector& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        TestVector tmp(x.size(), 0);
        _M_start = tmp._M_start; _M_end_of_storage = tmp._M_end_of_storage;
        // __uninitialized_copy -> __copy_trivial (memmove of the pointer range) 0x00405BA0
        std::memmove(_M_start, x._M_start, (x._M_finish - x._M_start) * sizeof(T));
        _M_finish = _M_start + (x._M_finish - x._M_start);
        tmp._M_start = 0;
    }
};
int main()
{
    CAnimationEntry e[3] = { {1}, {2}, {3} };
    TestVector<const CAnimationEntry*> v(3, 0);
    v._M_start[0] = &e[0]; v._M_start[1] = &e[2]; v._M_start[2] = &e[1]; v._M_finish = v._M_start + 3;
    TestVector<const CAnimationEntry*> c(v);
    if (c.size() != 3 || c._M_start == v._M_start || c._M_end_of_storage != c._M_start + 3) { std::printf("0062f590_TEST FAIL\n"); return 1; }
    if (c._M_start[0] != &e[0] || c._M_start[1] != &e[2] || c._M_start[2] != &e[1]) { std::printf("0062f590_TEST FAIL\n"); return 1; }
    TestVector<const CAnimationEntry*> empty;
    TestVector<const CAnimationEntry*> c2(empty);
    if (c2.size() != 0) { std::printf("0062f590_TEST FAIL\n"); return 1; }
    std::free(v._M_start); std::free(c._M_start);
    std::printf("0062f590_TEST PASS\n");
    return 0;
}