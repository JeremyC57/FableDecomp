#include <cstdio>
#include <cstdlib>
// Behaviour test for vector<CTextureManagerPoolInfo>::vector(n) @ 0x00B3E920: n slots allocated (none for zero), each a
// copy of a default-constructed info (pixel format -1), finish and end of storage at start + n.
inline void* operator new(size_t, void* p, int) { return p; }
class CPixelFormat {
public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) { }
};
class CTextureManagerPoolInfo {
public:
    unsigned long Width;
    unsigned long Height;
    CPixelFormat  PixelFormat;
    bool          UseMipmaps;
    unsigned long Count;
};
template <class T> T* UninitializedFillN(T* first, unsigned int n, const T& x)   // the out-of-line 0x00B3E390
{
    T* cur = first;
    for ( ; n > 0; --n, ++cur)
        new (cur, 0) T(x);
    return cur;
}
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    explicit vector(unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (T*)malloc(n * sizeof(T)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        _M_finish = UninitializedFillN(_M_start, n, T());
    }
};
int main()
{
    vector<CTextureManagerPoolInfo> v(7);
    if (v._M_start == 0 || v._M_finish != v._M_start + 7 || v._M_end_of_storage != v._M_start + 7) { std::printf("00b3e920_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i)
        if (v._M_start[i].PixelFormat.FormatIndex != -1) { std::printf("00b3e920_TEST FAIL\n"); return 1; }
    free(v._M_start);
    vector<CTextureManagerPoolInfo> e(0);
    if (e._M_start != 0 || e._M_finish != 0 || e._M_end_of_storage != 0) { std::printf("00b3e920_TEST FAIL\n"); return 1; }
    std::printf("00b3e920_TEST PASS\n");
    return 0;
}