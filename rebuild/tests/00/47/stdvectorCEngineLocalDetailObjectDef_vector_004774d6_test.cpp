// Behaviour test for 0x004774d6 std::vector<CEngineLocalDetailObjectDef>::vector(const vector&) (model copy, 0x88-byte element).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
struct SElement {
    unsigned char _body[0x88];
    SElement() { _body[0] = 0; }
    SElement(const SElement& o) { for (int i = 0; i < 0x88; ++i) _body[i] = o._body[i]; ++g_copies; }
};

struct VectorBaseModel {
    SElement* _M_start;
    SElement* _M_finish;
    SElement* _M_end_of_storage;
    VectorBaseModel(unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n ? (SElement*)std::malloc(n * sizeof(SElement)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
    }
};

static SElement* uninitialized_copy_model(const SElement* first, const SElement* last, SElement* result)
{
    SElement* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SElement(*first);
    return cur;
}

struct VectorModel : VectorBaseModel {
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    VectorModel(const VectorModel& x) : VectorBaseModel(x.size())
    {
        _M_finish = uninitialized_copy_model(x._M_start, x._M_finish, _M_start);
    }
    VectorModel(const SElement* first, unsigned int n) : VectorBaseModel(n)
    {
        _M_finish = uninitialized_copy_model(first, first + n, _M_start);
    }
};

int main()
{
    SElement src[4];
    for (int i = 0; i < 4; ++i) { src[i]._body[0] = (unsigned char)(i + 1); src[i]._body[0x88 - 1] = (unsigned char)(0x40 + i); }
    VectorModel v(src, 4);
    g_copies = 0;
    VectorModel w(v);
    int ok = g_copies == 4 && w.size() == 4 && w._M_start[3]._body[0] == 4 && w._M_start[1]._body[0x88 - 1] == 0x41
          && w._M_end_of_storage == w._M_finish && w._M_start != v._M_start;
    std::free(v._M_start);
    std::free(w._M_start);
    if (!ok) { std::printf("004774d6_TEST FAIL\n"); return 1; }
    std::printf("004774d6_TEST PASS\n");
    return 0;
}