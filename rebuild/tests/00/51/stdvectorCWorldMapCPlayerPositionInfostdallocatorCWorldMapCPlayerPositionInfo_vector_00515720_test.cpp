// Behaviour test for 0x00515720 std::vector<CWorldMap::CPlayerPositionInfo>(n): n 13-byte records, each a copy of the
// default record (Set == false), finish == end of storage == start + n; n == 0 leaves the pointers null.
#include <cstdio>
#include <cstdlib>
#include <new>

class C3DVector { public: float X, Y, Z; C3DVector() { } };

#pragma pack(push, 1)
struct CPlayerPositionInfo {
    bool      Set;
    C3DVector Pos;
    CPlayerPositionInfo() : Set(false) { }
};
#pragma pack(pop)

struct false_type_model { false_type_model() { } };

template <class ForwardIter, class Size, class T>
ForwardIter uninitialized_fill_n_model(ForwardIter first, Size n, const T& x, const false_type_model&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (&*cur) T(x);
    return cur;
}

struct vector_model {
    CPlayerPositionInfo* _M_start;
    CPlayerPositionInfo* _M_finish;
    CPlayerPositionInfo* _M_end_of_storage;
    explicit vector_model(unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (CPlayerPositionInfo*)std::malloc(n * sizeof(CPlayerPositionInfo)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        _M_finish = uninitialized_fill_n_model(_M_start, n, CPlayerPositionInfo(), false_type_model());
    }
};

int main()
{
    if (sizeof(CPlayerPositionInfo) != 13) { std::printf("00515720_TEST FAIL\n"); return 1; }
    vector_model v(4);
    if (v._M_start == 0 || v._M_finish != v._M_start + 4 || v._M_end_of_storage != v._M_start + 4) { std::printf("00515720_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (v._M_start[i].Set != false) { std::printf("00515720_TEST FAIL\n"); return 1; }
    std::free(v._M_start);
    vector_model e(0);
    if (e._M_start != 0 || e._M_finish != 0 || e._M_end_of_storage != 0) { std::printf("00515720_TEST FAIL\n"); return 1; }
    std::printf("00515720_TEST PASS\n");
    return 0;
}