// Behaviour test for CRetailVector20::reserve (0x005C2013).
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct SRecord20 { unsigned char bytes[20]; };

struct CAllocProxy20 {
    SRecord20* _M_data;
    SRecord20* allocate(unsigned long count, const void* = 0) { return count ? (SRecord20*)std::malloc(count * 20) : 0; }
};

struct CRetailVector20 {
    SRecord20* _M_start; SRecord20* _M_finish; CAllocProxy20 _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    unsigned long capacity() const { return _M_end_of_storage._M_data - _M_start; }
    SRecord20* _M_allocate_and_copy(unsigned long count, SRecord20* first, SRecord20* last)
    {
        SRecord20* p = _M_end_of_storage.allocate(count);
        std::memcpy(p, first, (last - first) * sizeof(SRecord20));
        return p;
    }
    void _M_clear() { std::free(_M_start); }
    void _M_set(SRecord20* s, SRecord20* f, SRecord20* e) { _M_start = s; _M_finish = f; _M_end_of_storage._M_data = e; }
    void reserve(unsigned long count)
    {
        if (capacity() < count) {
            const unsigned long oldSize = size();
            SRecord20* buffer;
            if (_M_start != 0) {
                buffer = _M_allocate_and_copy(count, _M_start, _M_finish);
                _M_clear();
            } else {
                buffer = _M_end_of_storage.allocate(count);
            }
            _M_set(buffer, buffer + oldSize, buffer + count);
        }
    }
};

int main()
{
    CRetailVector20 v; v._M_start = v._M_finish = v._M_end_of_storage._M_data = 0;
    v.reserve(4);
    if (v.capacity() != 4 || v.size() != 0) return std::printf("005c2013_TEST FAIL\n"), 1;
    v._M_start[0].bytes[0] = 42; v._M_finish = v._M_start + 1;
    SRecord20* old = v._M_start;
    v.reserve(2);                                   // enough capacity: unchanged
    if (v._M_start != old || v.capacity() != 4) return std::printf("005c2013_TEST FAIL\n"), 1;
    v.reserve(10);
    if (v.capacity() != 10 || v.size() != 1 || v._M_start[0].bytes[0] != 42) return std::printf("005c2013_TEST FAIL\n"), 1;
    std::free(v._M_start);
    std::printf("005c2013_TEST PASS\n");
    return 0;
}
