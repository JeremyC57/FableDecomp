// Behaviour test for 0x00A01120 vector<CGraphicInfo>::_M_insert_overflow (STLport grow path, 44-byte info with a clearing destructor).
#include <cstdio>
#include <cstdlib>
#include <new>

struct Vertex {                                  // stands in for CGraphicInfo (0x2C bytes)
    float X, Y, Z, RHW; unsigned int Diffuse, Specular; float U, V;
    void* Bank; int Index; void* Frames;
    ~Vertex() { Bank = 0; Frames = 0; }
};

struct Vec {
    Vertex* _M_start; Vertex* _M_finish; Vertex* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(Vertex* pos, const Vertex& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Vertex* new_start = len != 0 ? (Vertex*)std::malloc(len * sizeof(Vertex)) : 0;
        Vertex* new_finish = new_start;
        for (Vertex* s = _M_start; s != pos; ++s, ++new_finish) new (new_finish) Vertex(*s);
        if (fill_len == 1) {
            new (new_finish) Vertex(x);
            ++new_finish;
        } else
            for (unsigned int n = fill_len; n > 0; --n, ++new_finish) new (new_finish) Vertex(x);
        if (!atend)
            for (Vertex* s = pos; s != _M_finish; ++s, ++new_finish) new (new_finish) Vertex(*s);
        for (Vertex* d = _M_start; d != _M_finish; ++d) d->~Vertex();
        if (_M_start) std::free(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
    void push_back(const Vertex& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) Vertex(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, 1, true);
    }
};

static Vertex V(unsigned int k)
{
    Vertex v;
    v.X = (float)k; v.Y = (float)k + 0.5f; v.Z = 0.0f; v.RHW = 1.0f;
    v.Diffuse = k * 7; v.Specular = k * 11; v.U = 0.25f; v.V = 0.75f;
    v.Bank = &v; v.Index = (int)k; v.Frames = 0;
    return v;
}

int main()
{
    int ok = 1;
    Vec v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    for (unsigned int i = 0; i < 5; ++i) v.push_back(V(i));          // caps 1, 2, 4, 8
    if (v.size() != 5 || v._M_end_of_storage - v._M_start != 8) ok = 0;
    v._M_end_of_storage = v._M_finish;                                 // force the overflow path
    v._M_insert_overflow(v._M_start + 2, V(9), 3, false);             // 5 + max(5, 3) = 10
    unsigned int want[8] = { 0, 1, 9, 9, 9, 2, 3, 4 };
    if (v.size() != 8 || v._M_end_of_storage - v._M_start != 10) ok = 0;
    for (unsigned int i = 0; ok && i < 8; ++i) {
        const Vertex& e = v._M_start[i];
        if (e.Diffuse != want[i] * 7 || e.Specular != want[i] * 11 || e.X != (float)want[i] || e.V != 0.75f || e.Index != (int)want[i]) ok = 0;
    }
    std::free(v._M_start);
    if (!ok) { std::printf("00a01120_TEST FAIL\n"); return 1; }
    std::printf("00a01120_TEST PASS\n");
    return 0;
}