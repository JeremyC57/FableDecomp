#include <cstdio>
#include <new>

// Behaviour model of STLport vector<CTexture>::erase(first, last) (0x0049B3B0): the tail is assigned down, the vacated
// tail destroyed (~CTexture = Uninitialise: drop the texture pointer), finish pulled back, first returned.
static int g_assigns = 0, g_dtors = 0;

class CTexture { public:
    void*         PD3DTexture;
    unsigned long ByteLength : 28;
    unsigned long AllocationSource : 4;
    CTexture& operator=(const CTexture& o) { ++g_assigns; PD3DTexture = o.PD3DTexture; ByteLength = o.ByteLength; AllocationSource = o.AllocationSource; return *this; }
    ~CTexture() { ++g_dtors; PD3DTexture = 0; }
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
static _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, _Distance*)
{
    for (_Distance __n = (_Distance)(__last - __first); __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

struct CTextureVector {
    CTexture* _M_start;
    CTexture* _M_finish;
    CTexture* _M_end_of_storage;
    CTexture* erase(CTexture* __first, CTexture* __last)
    {
        CTexture* __i = __copy(__last, _M_finish, __first, (int*)0);
        for (CTexture* p = __i; p != _M_finish; ++p) p->~CTexture();
        _M_finish = __i;
        return __first;
    }
};

static char g_tex[16];

int main()
{
    for (int n = 0; n <= 10; ++n) {
        for (int a = 0; a <= n; ++a) {
            for (int b = a; b <= n; ++b) {
                static unsigned char raw[sizeof(CTexture) * 10];
                CTexture* t = (CTexture*)raw;
                for (int i = 0; i < n; ++i) { new (&t[i]) CTexture; t[i].PD3DTexture = &g_tex[i]; t[i].ByteLength = 100 + i; t[i].AllocationSource = i & 15; }
                CTextureVector v; v._M_start = t; v._M_finish = t + n; v._M_end_of_storage = t + 10;
                g_assigns = 0; g_dtors = 0;
                CTexture* r = v.erase(t + a, t + b);
                if (r != t + a) { std::printf("0049b3b0_TEST FAIL\n"); return 1; }
                if (v._M_finish != t + n - (b - a)) { std::printf("0049b3b0_TEST FAIL\n"); return 1; }
                if (g_assigns != n - b || g_dtors != b - a) { std::printf("0049b3b0_TEST FAIL\n"); return 1; }
                for (int i = 0; i < n - (b - a); ++i) {
                    int src = i < a ? i : i + (b - a);
                    if (t[i].PD3DTexture != &g_tex[src] || t[i].ByteLength != (unsigned long)(100 + src)) { std::printf("0049b3b0_TEST FAIL\n"); return 1; }
                }
                for (int i = n - (b - a); i < n; ++i) if (t[i].PD3DTexture != 0) { std::printf("0049b3b0_TEST FAIL\n"); return 1; }
            }
        }
    }
    std::printf("0049b3b0_TEST PASS\n");
    return 0;
}