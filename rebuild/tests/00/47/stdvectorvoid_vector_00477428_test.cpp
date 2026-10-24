// Behaviour test for 0x00477428 std::vector<void*>::vector(const vector&) (model copy).
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct VectorBaseModel {
    void** _M_start;
    void** _M_finish;
    void** _M_end_of_storage;
    VectorBaseModel(unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n ? (void**)std::malloc(n * sizeof(void*)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
    }
};

static void** uninitialized_copy_model(void* const* first, void* const* last, void** result)
{
    std::memmove(result, first, (char*)last - (char*)first);
    return result + (last - first);
}

struct VectorModel : VectorBaseModel {
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    VectorModel(const VectorModel& x) : VectorBaseModel(x.size())
    {
        _M_finish = uninitialized_copy_model(x._M_start, x._M_finish, _M_start);
    }
    VectorModel(void** first, unsigned int n) : VectorBaseModel(n)
    {
        _M_finish = uninitialized_copy_model(first, first + n, _M_start);
    }
};

int main()
{
    int a, b, c;
    void* src[3] = { &a, &b, &c };
    VectorModel v(src, 3);
    VectorModel w(v);
    int ok = w.size() == 3 && w._M_start != v._M_start && w._M_start[0] == &a && w._M_start[2] == &c
          && w._M_end_of_storage == w._M_finish;
    VectorModel e(src, 0);
    VectorModel f(e);
    if (f.size() != 0 || f._M_start != 0) ok = 0;
    std::free(v._M_start);
    std::free(w._M_start);
    if (!ok) { std::printf("00477428_TEST FAIL\n"); return 1; }
    std::printf("00477428_TEST PASS\n");
    return 0;
}