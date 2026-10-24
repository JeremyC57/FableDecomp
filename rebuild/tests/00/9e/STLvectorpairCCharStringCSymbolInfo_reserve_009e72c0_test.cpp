// Behaviour test for 0x009E72C0 STLport vector::reserve (16-byte entries): grows only when needed, keeps the elements.
#include <cstdio>
#include <cstdlib>
#include <new>

struct SEntry { int key; int a; int b; int c; };
static int gCopies = 0, gDestroys = 0;

static SEntry* __uninitialized_copy(const SEntry* first, const SEntry* last, SEntry* result)
{
    SEntry* cur = result;
    for (; first != last; ++first, ++cur) { new (cur) SEntry(*first); ++gCopies; }
    return cur;
}
static void __destroy_aux(SEntry* first, SEntry* last) { for (; first != last; ++first) ++gDestroys; }

struct vector {
    SEntry* _M_start; SEntry* _M_finish; SEntry* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    SEntry* _M_allocate(unsigned int n) { return n != 0 ? (SEntry*)std::malloc(n * sizeof(SEntry)) : 0; }
    void _M_deallocate(SEntry* p) { if (p != 0) std::free(p); }
    void _M_clear() { __destroy_aux(_M_start, _M_finish); _M_deallocate(_M_start); }
    void _M_set(SEntry* s, SEntry* f, SEntry* e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }
    SEntry* _M_allocate_and_copy(unsigned int n, const SEntry* first, const SEntry* last)
    {
        SEntry* result = _M_allocate(n);
        __uninitialized_copy(first, last, result);
        return result;
    }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int old_size = size();
            SEntry* tmp;
            if (_M_start) {
                tmp = _M_allocate_and_copy(n, _M_start, _M_finish);
                _M_clear();
            } else {
                tmp = _M_allocate(n);
            }
            _M_set(tmp, tmp + old_size, tmp + n);
        }
    }
};

int main()
{
    vector v = { 0, 0, 0 };
    v.reserve(0);
    bool ok = v._M_start == 0 && v.capacity() == 0;
    v.reserve(2);
    ok = ok && v._M_start != 0 && v.size() == 0 && v.capacity() == 2 && gCopies == 0;
    v._M_start[0].key = 5; v._M_start[1].key = 6; v._M_finish = v._M_start + 2;
    v.reserve(1);
    ok = ok && v.capacity() == 2 && gCopies == 0;
    v.reserve(8);
    ok = ok && v.capacity() == 8 && v.size() == 2 && v._M_start[0].key == 5 && v._M_start[1].key == 6 && gCopies == 2 && gDestroys == 2;
    std::free(v._M_start);
    if (!ok) { std::printf("009e72c0_TEST FAIL\n"); return 1; }
    std::printf("009e72c0_TEST PASS\n");
    return 0;
}