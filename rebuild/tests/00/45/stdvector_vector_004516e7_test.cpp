// Behaviour test for 0x004516E7 std::vector<float>::vector(size_type n, const float& value, const allocator&) (model copy).
#include <cstdio>
#include <cstdlib>

struct VectorBaseModel {
    float* _M_start;
    float* _M_finish;
    float* _M_end_of_storage;
    VectorBaseModel(unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n ? (float*)std::malloc(n * sizeof(float)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
    }
};

static float* fill_n_model(float* first, unsigned int n, const float& val)
{
    for (; n > 0; --n, ++first)
        *first = val;
    return first;
}

struct VectorModel : VectorBaseModel {
    VectorModel(unsigned int n, const float& val) : VectorBaseModel(n) { _M_finish = fill_n_model(_M_start, n, val); }
};

int main()
{
    VectorModel v(5, 0.5f);
    int ok = v._M_finish == v._M_start + 5 && v._M_end_of_storage == v._M_finish;
    for (int i = 0; i < 5; ++i)
        if (v._M_start[i] != 0.5f) ok = 0;
    VectorModel e(0, 1.0f);
    if (e._M_start != 0 || e._M_finish != 0) ok = 0;
    std::free(v._M_start);
    if (!ok) { std::printf("CTCEnemy_004516e7_TEST FAIL\n"); return 1; }
    std::printf("CTCEnemy_004516e7_TEST PASS\n");
    return 0;
}
