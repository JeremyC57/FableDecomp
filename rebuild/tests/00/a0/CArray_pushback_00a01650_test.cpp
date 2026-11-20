// Behaviour test for 0x00A01650 vector<CGraphicInfo>::push_back(): appends one default CGraphicInfo (zero header with
// FrameCount 1 and PixelFormat -1, no bank, Index -1, no frames) in place while there is room, else via _M_insert_overflow.
#include <cstdio>
#include <new>

static int gOverflows = 0;
static unsigned int gOverflowLen = 0;
static bool gOverflowAtEnd = false;
static long gOverflowIndex = 0;

class CPixelFormat { public: long FormatIndex; CPixelFormat() : FormatIndex(-1) {} };

class CGraphicHeader {
public:
    unsigned short Width, Height, Depth, FrameWidth, FrameHeight, FrameCount;
    CPixelFormat PixelFormat;
    unsigned char TransparencyType;
    unsigned char MipmapLevels;
    unsigned char IsFlatAnimatedGraphic : 1;
    unsigned long FrameDataSize;
    unsigned long CompressedMipmapSizes[1];
    CGraphicHeader()
        : Width(0), Height(0), Depth(0), FrameWidth(0), FrameHeight(0), FrameCount(1), TransparencyType(0),
          MipmapLevels(0), IsFlatAnimatedGraphic(false), FrameDataSize(0)
    {
        CompressedMipmapSizes[0] = 0;
    }
};

class CGraphicInfo {
public:
    CGraphicHeader Header;
    int TextureType;
    void* Bank;
    long Index;
    void* Frames;
    CGraphicInfo() : Bank(0), Index(-1), Frames(0) {}
};

struct __false_type { __false_type() {} };

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        ++gOverflows; gOverflowLen = fill_len; gOverflowAtEnd = atend && position == _M_finish; gOverflowIndex = x.Index;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    void push_back() { push_back(T()); }
};

int main()
{
    CGraphicInfo storage[2];
    storage[0].Index = 5; storage[0].Header.Width = 3; storage[0].Header.IsFlatAnimatedGraphic = true; storage[0].Bank = storage;
    vector<CGraphicInfo> v;
    v._M_start = storage; v._M_finish = storage; v._M_end_of_storage = storage + 2;
    v.push_back();
    const CGraphicInfo& g = storage[0];
    bool ok = v._M_finish == storage + 1 && g.Header.Width == 0 && g.Header.FrameHeight == 0 && g.Header.FrameCount == 1
           && g.Header.PixelFormat.FormatIndex == -1 && g.Header.TransparencyType == 0 && g.Header.MipmapLevels == 0
           && g.Header.IsFlatAnimatedGraphic == 0 && g.Header.FrameDataSize == 0 && g.Header.CompressedMipmapSizes[0] == 0
           && g.Bank == 0 && g.Index == -1 && g.Frames == 0;
    v.push_back();
    ok = ok && v._M_finish == storage + 2 && gOverflows == 0;
    v.push_back();
    ok = ok && v._M_finish == storage + 2 && gOverflows == 1 && gOverflowLen == 1 && gOverflowAtEnd && gOverflowIndex == -1;
    if (!ok) { std::printf("00a01650_TEST FAIL\n"); return 1; }
    std::printf("00a01650_TEST PASS\n");
    return 0;
}