// Behaviour test for 0x00709E60 vector<pair<CMeshInfo, CEngineTextureMorphSet>>::push_back (STLport shape).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_arrayCopies = 0;
struct CMeshInfo { long Index; float SizeX; unsigned char PrimaryMesh : 1; };
struct CMorphArray {
    int* _M_start; int* _M_finish; int* _M_end_of_storage;
    CMorphArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CMorphArray(const CMorphArray& o) : _M_start(o._M_start), _M_finish(o._M_finish), _M_end_of_storage(o._M_end_of_storage) { ++g_arrayCopies; }
};
class CEngineTextureMorphSet { public: CMorphArray Morphs; };
struct SPair {
    CMeshInfo first; CEngineTextureMorphSet second;
    SPair() {}
    SPair(const SPair& o) : first(o.first), second(o.second) {}
};

struct CVector {
    SPair* _M_start; SPair* _M_finish; SPair* _M_end_of_storage;
    int overflows;
    void _M_insert_overflow(SPair* pos, const SPair& x, unsigned int fill_len, bool atend)
    {
        ++overflows;
        unsigned int size = (unsigned int)(_M_finish - _M_start);
        unsigned int len = size + (size > fill_len ? size : fill_len);
        SPair* p = (SPair*)std::malloc(len * sizeof(SPair));
        SPair* out = p;
        for (SPair* it = _M_start; it != pos; ++it, ++out) new (out) SPair(*it);
        for (unsigned int i = 0; i < fill_len; ++i, ++out) new (out) SPair(x);
        if (!atend) for (SPair* it = pos; it != _M_finish; ++it, ++out) new (out) SPair(*it);
        std::free(_M_start);
        _M_start = p; _M_finish = out; _M_end_of_storage = p + len;
    }
    void push_back(const SPair& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) SPair(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, 1, true);
    }
};

int main()
{
    CVector v; v._M_start = (SPair*)std::malloc(sizeof(SPair) * 2); v._M_finish = v._M_start; v._M_end_of_storage = v._M_start + 2; v.overflows = 0;
    SPair x; x.first.Index = 1; x.first.SizeX = 0.5f; x.first.PrimaryMesh = 1;
    for (int i = 0; i < 5; ++i) { x.first.Index = i; v.push_back(x); }
    bool ok = v._M_finish - v._M_start == 5 && v.overflows == 2;
    for (int i = 0; i < 5; ++i) ok = ok && v._M_start[i].first.Index == i && v._M_start[i].first.PrimaryMesh == 1;
    if (!ok) { std::printf("00709e60_TEST FAIL\n"); return 1; }
    std::printf("00709e60_TEST PASS\n");
    return 0;
}