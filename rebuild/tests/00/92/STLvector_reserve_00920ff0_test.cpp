// Behaviour test for 0x00920FF0 vector<C3DVector>::reserve: grows only when the capacity is short, keeps the size and
// the elements, frees the old block, and allocates a bare block for an empty vector.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_mallocs = 0;
static int g_frees = 0;
static void* TMalloc(std::size_t n) { ++g_mallocs; return std::malloc(n); }
static void TFree(void* p) { ++g_frees; std::free(p); }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class Vec3Vector {                       // STLport vector<C3DVector> model
public:
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    Vec3Vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    C3DVector* allocate(unsigned int n) { return n != 0 ? (C3DVector*)TMalloc(n * sizeof(C3DVector)) : 0; }
    C3DVector* _M_allocate_and_copy(unsigned int n, const C3DVector* first, const C3DVector* last)
    {
        C3DVector* result = allocate(n);
        C3DVector* cur = result;
        for ( ; first != last; ++first, ++cur)
            if (cur) new (cur) C3DVector(*first);
        return result;
    }
    void _M_clear() { if (_M_start) TFree(_M_start); }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            unsigned int old_size = size();
            C3DVector* tmp;
            if (_M_start) {
                tmp = _M_allocate_and_copy(n, _M_start, _M_finish);
                _M_clear();
            } else {
                tmp = allocate(n);
            }
            _M_start = tmp;
            _M_finish = tmp + old_size;
            _M_end_of_storage = tmp + n;
        }
    }
    void push_back_fit(const C3DVector& v) { new (_M_finish) C3DVector(v); ++_M_finish; }
};

static int Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    int ok = 1;
    Vec3Vector v;
    v.reserve(0);
    if (v._M_start != 0 || g_mallocs != 0) ok = 0;                      // capacity 0 is not below 0
    v.reserve(2);
    if (v.capacity() != 2 || v.size() != 0 || g_mallocs != 1 || g_frees != 0) ok = 0;
    v.push_back_fit(C3DVector(1.0f, 2.0f, 3.0f));
    v.push_back_fit(C3DVector(4.0f, 5.0f, 6.0f));
    v.reserve(2);
    if (g_mallocs != 1) ok = 0;                                          // enough capacity: no-op
    v.reserve(5);
    if (v.capacity() != 5 || v.size() != 2 || g_mallocs != 2 || g_frees != 1) ok = 0;
    if (!Near(v._M_start[0].X, 1.0f) || !Near(v._M_start[0].Z, 3.0f) || !Near(v._M_start[1].Y, 5.0f)) ok = 0;
    v._M_clear();
    if (!ok) { std::printf("00920ff0_TEST FAIL\n"); return 1; }
    std::printf("00920ff0_TEST PASS\n");
    return 0;
}