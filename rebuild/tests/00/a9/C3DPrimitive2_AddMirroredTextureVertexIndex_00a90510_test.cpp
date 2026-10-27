// Behaviour test for 0x00A90510 C3DPrimitive2::AddMirroredTextureVertexIndex(const unsigned long*, unsigned long)
// Model: push_back each index; when full, the overflow path grows the storage (doubling) and appends.
#include <cstdio>
#include <cstdlib>
#include <cstring>
class TArray {
public:
    unsigned long* _M_start; unsigned long* _M_finish; unsigned long* _M_end_of_storage;
    int overflows;
    void insert_overflow(const unsigned long& x)
    {
        ++overflows;
        unsigned int n = (unsigned int)(_M_finish - _M_start);
        unsigned int cap = n ? n * 2 : 1;
        unsigned long* p = (unsigned long*)std::malloc(cap * sizeof(unsigned long));
        if (n) std::memcpy(p, _M_start, n * sizeof(unsigned long));
        p[n] = x;
        std::free(_M_start);
        _M_start = p; _M_finish = p + n + 1; _M_end_of_storage = p + cap;
    }
    void push_back(const unsigned long& x)
    {
        if (_M_finish != _M_end_of_storage) { *_M_finish = x; ++_M_finish; }
        else insert_overflow(x);
    }
};
class C3DPrimitive2 { public:
    TArray MirroredTextureVertexIndexList;
    void AddMirroredTextureVertexIndex(const unsigned long* indices, unsigned long count)
    {
        for (unsigned long i = 0; i < count; ++i)
            MirroredTextureVertexIndexList.push_back(indices[i]);
    }
};
int main()
{
    C3DPrimitive2 p;
    p.MirroredTextureVertexIndexList._M_start = p.MirroredTextureVertexIndexList._M_finish = p.MirroredTextureVertexIndexList._M_end_of_storage = 0;
    p.MirroredTextureVertexIndexList.overflows = 0;
    unsigned long idx[5] = { 3, 1, 4, 1, 5 };
    p.AddMirroredTextureVertexIndex(idx, 0);
    if (p.MirroredTextureVertexIndexList._M_start != 0) { std::printf("00a90510_TEST FAIL\n"); return 1; }
    p.AddMirroredTextureVertexIndex(idx, 5);
    TArray& v = p.MirroredTextureVertexIndexList;
    if (v._M_finish - v._M_start != 5 || v.overflows != 4) { std::printf("00a90510_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) if (v._M_start[i] != idx[i]) { std::printf("00a90510_TEST FAIL\n"); return 1; }
    std::free(v._M_start);
    std::printf("00a90510_TEST PASS\n");
    return 0;
}