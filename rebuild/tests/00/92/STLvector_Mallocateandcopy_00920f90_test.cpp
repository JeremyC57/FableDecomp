// Behaviour test for 0x00920F90 vector<C3DVector>::_M_allocate_and_copy: allocates n elements and copies [first, last)
// into the front; returns the block start (null for n == 0).
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
    C3DVector src[3] = { C3DVector(1.0f, 2.0f, 3.0f), C3DVector(4.0f, 5.0f, 6.0f), C3DVector(7.0f, 8.0f, 9.0f) };
    C3DVector* p = v._M_allocate_and_copy(4, src, src + 3);
    if (p == 0 || g_mallocs != 1) ok = 0;
    if (p && (!Near(p[0].X, 1.0f) || !Near(p[1].Y, 5.0f) || !Near(p[2].Z, 9.0f))) ok = 0;
    if (p) TFree(p);
    C3DVector* q = v._M_allocate_and_copy(0, src, src);
    if (q != 0 || g_mallocs != 1) ok = 0;
    if (!ok) { std::printf("00920f90_TEST FAIL\n"); return 1; }
    std::printf("00920f90_TEST PASS\n");
    return 0;
}