// Behaviour test for 0x00A4A910: CCountedPointer<CSpeechFrame>::DeleteData (delete the frame: free its frequency array, then the frame)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

static int g_frees = 0;
static void CountingFree(void* p) { ++g_frees; free(p); }
namespace std {
template <class _Tp>
class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { CountingFree(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, (unsigned int)(_M_end_of_storage._M_data - _M_start));
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { }
};
}
class CComplexNumber {
public:
    float Real, Imaginary;
};
template <class T>
class CArray : public std::vector<T> {
};
static int g_frames = 0;
class CSpeechFrame {
public:
    CArray<CComplexNumber> Frequencies;
    float Amplitude;
    float MedianFrequency;
    float BaseFrequency;
    float FrequencyVariance;
    CSpeechFrame() { ++g_frames; }
    ~CSpeechFrame() { --g_frames; }
};
class CCPPointerInfo;
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

int main()
{
    CSpeechFrame* f = new CSpeechFrame;
    f->Frequencies._M_start = (CComplexNumber*)malloc(4 * sizeof(CComplexNumber));
    f->Frequencies._M_finish = f->Frequencies._M_start + 2;
    f->Frequencies._M_end_of_storage._M_data = f->Frequencies._M_start + 4;
    CCountedPointer<CSpeechFrame>::DeleteData(f);
    Check(g_frames == 0 && g_frees == 1);
    CSpeechFrame* g = new CSpeechFrame;              // empty frequency array: nothing to free
    g->Frequencies._M_start = g->Frequencies._M_finish = g->Frequencies._M_end_of_storage._M_data = 0;
    CCountedPointer<CSpeechFrame>::DeleteData(g);
    Check(g_frames == 0 && g_frees == 1);
    if (g_fail) {
        printf("00a4a910_TEST FAIL\n");
        return 1;
    }
    printf("00a4a910_TEST PASS\n");
    return 0;
}