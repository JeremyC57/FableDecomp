// Behaviour test for vector<CGraphicInfo>::resize(n) (0x00A01820): growing appends default-constructed elements after the kept
// prefix, shrinking erases the tail, resizing to the current size changes nothing.
#include <cstdio>
#include <cstdlib>
#include <new>

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
}

template <class T> class TVector {          // STLport vector stand-in: start, finish, end of storage
public:
    T* Start;
    T* Finish;
    T* End;
    TVector() : Start(0), Finish(0), End(0) { }
    ~TVector() { Clear(); std::free(Start); }
    void Clear()
    {
        for (T* p = Start; p != Finish; ++p)
            p->~T();
        Finish = Start;
    }
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    T* erase(T* first, T* last)
    {
        T* d = first;
        for (T* s = last; s != Finish; ++s, ++d)
            *d = *s;
        for (T* p = d; p != Finish; ++p)
            p->~T();
        Finish = d;
        return first;
    }
    void fill_insert_at_end(unsigned int n, const T& x)
    {
        if ((unsigned int)(End - Finish) < n) {
            unsigned int len = size() + n + 1;
            T* mem = (T*)std::malloc(len * sizeof(T));
            T* cur = mem;
            for (T* p = Start; p != Finish; ++p, ++cur)
                new (cur) T(*p);
            for (; n != 0; --n, ++cur)
                new (cur) T(x);
            Clear();
            std::free(Start);
            Start = mem;
            Finish = cur;
            End = mem + len;
            return;
        }
        for (; n != 0; --n, ++Finish)
            new (Finish) T(x);
    }
    // the row's logic: STLport resize(n, x) and the one-argument resize(n) { resize(n, _Tp()); }
    void resize(unsigned int n, const T& x)
    {
        if (n < size())
            erase(Start + n, Finish);
        else
            fill_insert_at_end(n - size(), x);
    }
    void resize(unsigned int n) { resize(n, T()); }
};

static bool Near(float a, float b) { float d = a - b; return d < 1e-6f && d > -1e-6f; }

class CGraphicDataBank;
class CGraphicInfoFrame;
class CPixelFormat {
public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) { }
};
class CGraphicHeader {
public:
    unsigned short Width, Height, Depth, FrameWidth, FrameHeight, FrameCount;
    CPixelFormat   PixelFormat;
    unsigned char  TransparencyType;
    unsigned char  MipmapLevels;
    unsigned char  IsFlatAnimatedGraphic : 1;
    unsigned long  FrameDataSize;
    unsigned long  CompressedMipmapSizes[1];
    CGraphicHeader()
        : Width(0), Height(0), Depth(0), FrameWidth(0), FrameHeight(0), FrameCount(1), TransparencyType(0),
          MipmapLevels(0), IsFlatAnimatedGraphic(false), FrameDataSize(0)
    {
        CompressedMipmapSizes[0] = 0;
    }
};
enum ETextureType { TEXTURE_TYPE_NONE };
class CGraphicInfo {
public:
    CGraphicHeader     Header;
    ETextureType       TextureType;
    CGraphicDataBank*  Bank;
    long               Index;
    CGraphicInfoFrame* Frames;
    CGraphicInfo() : Bank(0), Index(-1), Frames(0) { }
};
typedef CGraphicInfo E;

int main()
{
    int fails = 0;
    {
    TVector<E> v;
    v.resize(3);
    if (v.size() != 3u) ++fails;
    for (int i = 0; i < 3; ++i) { const E& e = v.Start[i]; if (!(e.Bank == 0 && e.Index == -1 && e.Frames == 0 && e.Header.FrameCount == 1 && e.Header.Width == 0 && e.Header.PixelFormat.FormatIndex == -1 && e.Header.IsFlatAnimatedGraphic == 0 && e.Header.CompressedMipmapSizes[0] == 0)) ++fails; }
    for (int i = 0; i < 3; ++i) { E& e = v.Start[i]; e.Index = 20 + i; e.Header.Width = (unsigned short)(64 << i); }
    v.resize(6);
    if (v.size() != 6u) ++fails;
    for (int i = 0; i < 3; ++i) { const E& e = v.Start[i]; if (!(e.Index == 20 + i && e.Header.Width == (unsigned short)(64 << i) && e.Header.FrameCount == 1)) ++fails; }
    for (int i = 3; i < 6; ++i) { const E& e = v.Start[i]; if (!(e.Bank == 0 && e.Index == -1 && e.Frames == 0 && e.Header.FrameCount == 1 && e.Header.Width == 0 && e.Header.PixelFormat.FormatIndex == -1 && e.Header.IsFlatAnimatedGraphic == 0 && e.Header.CompressedMipmapSizes[0] == 0)) ++fails; }
    v.resize(2);
    if (v.size() != 2u) ++fails;
    for (int i = 0; i < 2; ++i) { const E& e = v.Start[i]; if (!(e.Index == 20 + i && e.Header.Width == (unsigned short)(64 << i) && e.Header.FrameCount == 1)) ++fails; }
    v.resize(2);
    if (v.size() != 2u) ++fails;
    }
    if (fails) { std::printf("00a01820_TEST FAIL\n"); return 1; }
    std::printf("00a01820_TEST PASS\n");
    return 0;
}