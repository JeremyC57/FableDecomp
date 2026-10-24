#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
// behaviour copy of CArray<CTVertexSeaBackground>::push_back @ 0x00be5950 (STLport push_back over a test grow path)
struct __false_type { };
class CTVertexSeaBackground {
public:
    unsigned int X, Y, Z;
    CTVertexSeaBackground() { }
    CTVertexSeaBackground(const CTVertexSeaBackground& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
static void Fill(CTVertexSeaBackground& v, int i) { v.X = i; v.Y = i * 2; v.Z = i * 3; }
static int Key(const CTVertexSeaBackground& v) { return (v.Y == v.X * 2 && v.Z == v.X * 3) ? (int)v.X : -1; }
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }
template <class T> class CArray {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    int overflowCalls;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0), overflowCalls(0) { }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        ++overflowCalls;
        unsigned int size = (unsigned int)(_M_finish - _M_start);
        unsigned int len = size + (size > fill_len ? size : fill_len);
        T* fresh = (T*)std::malloc(len * sizeof(T));
        if (size) std::memcpy(fresh, _M_start, size * sizeof(T));
        for (unsigned int i = 0; i < fill_len; ++i) _Construct(fresh + size + i, x);
        std::free(_M_start);
        _M_start = fresh; _M_finish = fresh + size + fill_len; _M_end_of_storage = fresh + len;
        (void)position; (void)atend;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
int main()
{
    CArray<CTVertexSeaBackground> a;
    int ok = 1;
    for (int i = 0; i < 9; ++i) { CTVertexSeaBackground v; Fill(v, i); a.push_back(v); }
    if (a._M_finish - a._M_start != 9) ok = 0;
    if (a.overflowCalls != 5) ok = 0;             // capacities 1, 2, 4, 8, 16
    for (int i = 0; i < 9; ++i) if (Key(a._M_start[i]) != i) ok = 0;
    std::free(a._M_start);
    if (!ok) { std::printf("00be5950_TEST FAIL\n"); return 1; }
    std::printf("00be5950_TEST PASS\n");
    return 0;
}