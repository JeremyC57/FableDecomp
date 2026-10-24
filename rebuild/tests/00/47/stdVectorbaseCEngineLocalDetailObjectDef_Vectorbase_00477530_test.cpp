// Behaviour test for 0x00477530 std::_Vector_base<T>::_Vector_base(size_t n, const allocator&) (model copy, 0x88-byte element).
#include <cstdio>
#include <cstdlib>

struct SElement { unsigned char _body[0x88]; };
static unsigned int g_lastSize = 0;
static void* malloc_spy(unsigned int n) { g_lastSize = n; return std::malloc(n); }

struct AllocatorModel {
    SElement* allocate(unsigned int n) { return n != 0 ? (SElement*)malloc_spy(n * sizeof(SElement)) : 0; }
};
struct ProxyModel : AllocatorModel {
    SElement* _M_data;
    ProxyModel(const AllocatorModel& a, SElement* p) : AllocatorModel(a), _M_data(p) { }
};
struct VectorBaseModel {
    SElement* _M_start;
    SElement* _M_finish;
    ProxyModel _M_end_of_storage;
    VectorBaseModel(unsigned int n, const AllocatorModel& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};

int main()
{
    AllocatorModel a;
    VectorBaseModel v(3, a);
    int ok = g_lastSize == 3 * 0x88 && v._M_start != 0 && v._M_finish == v._M_start && v._M_end_of_storage._M_data == v._M_start + 3;
    g_lastSize = 0;
    VectorBaseModel e(0, a);
    if (e._M_start != 0 || e._M_finish != 0 || e._M_end_of_storage._M_data != 0 || g_lastSize != 0) ok = 0;
    std::free(v._M_start);
    if (!ok) { std::printf("00477530_TEST FAIL\n"); return 1; }
    std::printf("00477530_TEST PASS\n");
    return 0;
}