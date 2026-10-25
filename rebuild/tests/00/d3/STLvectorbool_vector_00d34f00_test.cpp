// Behaviour test for vector<bool>::vector(const vector<bool>&) (0x00D34F00): allocate x.size() bytes (none for an
// empty source) and copy the elements; finish = start + size, capacity = size.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct CBoolVector {
    bool* _M_start;
    bool* _M_finish;
    bool* _M_end_of_storage;
    CBoolVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void Base(unsigned int n)                   // _Vector_base(n, allocator)
    {
        _M_start = 0; _M_finish = 0; _M_end_of_storage = 0;
        _M_start = n ? (bool*)std::malloc(n) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
    }
    static bool* UninitializedCopy(const bool* first, const bool* last, bool* result)
    {
        return (last == first) ? result : (bool*)std::memmove(result, first, last - first) + (last - first);
    }
    CBoolVector(const CBoolVector& x)
    {
        Base(x.size());
        _M_finish = UninitializedCopy(x._M_start, x._M_finish, _M_start);
    }
};

int main()
{
    bool pads[5] = { true, false, true, true, false };
    CBoolVector src;
    src._M_start = pads; src._M_finish = pads + 5; src._M_end_of_storage = pads + 5;
    CBoolVector copy(src);
    bool ok = copy.size() == 5 && copy._M_end_of_storage == copy._M_start + 5 && copy._M_start != pads
        && std::memcmp(copy._M_start, pads, 5) == 0;
    CBoolVector empty;
    CBoolVector copy2(empty);
    ok = ok && copy2._M_start == 0 && copy2._M_finish == 0 && copy2._M_end_of_storage == 0;
    std::free(copy._M_start);
    if (!ok) {
        std::printf("00d34f00_TEST FAIL\n");
        return 1;
    }
    std::printf("00d34f00_TEST PASS\n");
    return 0;
}