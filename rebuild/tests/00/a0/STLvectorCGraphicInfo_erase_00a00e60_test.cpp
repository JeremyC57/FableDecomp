// Behaviour test for 0x00A00E60 std::vector<CGraphicInfo>::erase(CGraphicInfo* first, CGraphicInfo* last).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00a00e60_TEST FAIL\n"); return 1; } } while (0)

struct CGraphicHeader {
    unsigned short Width, Height, Depth, FrameWidth, FrameHeight, FrameCount;
    long           PixelFormat;
    unsigned char  TransparencyType, MipmapLevels, Flags, Padding;
    unsigned long  FrameDataSize;
    unsigned long  CompressedMipmapSizes;
};
enum ETextureType { TEXTURE_TYPE_NONE = 0, TEXTURE_TYPE_2D = 1 };
class CGraphicDataBank { public: int Id; };
class CGraphicInfoFrame;
static int g_graphicDtors = 0;
class CGraphicInfo {
public:
    CGraphicHeader     Header;
    ETextureType       TextureType;
    CGraphicDataBank*  Bank;
    long               Index;
    CGraphicInfoFrame* Frames;
    CGraphicInfo(CGraphicDataBank* b, long i) : TextureType(TEXTURE_TYPE_2D), Bank(b), Index(i), Frames(0)
    { std::memset(&Header, 0, sizeof(Header)); Header.Width = (unsigned short)(i * 16); Header.FrameCount = 1; }
    ~CGraphicInfo() { Bank = 0; Frames = 0; ++g_graphicDtors; }
};

#include <new>
template <class _Tp>
class vector_model {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector_model(unsigned int capacity)
    {
        _M_start = (_Tp*)::operator new(sizeof(_Tp) * capacity);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + capacity;
    }
    ~vector_model() { _Destroy(_M_start, _M_finish); ::operator delete(_M_start); }
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    static void _Destroy(_Tp* __first, _Tp* __last) { for (; __first != __last; ++__first) __first->~_Tp(); }
    static _Tp* copy(_Tp* __first, _Tp* __last, _Tp* __result)
    {
        int __n;
        for (__n = (int)(__last - __first); __n > 0; --__n) { *__result = *__first; ++__first; ++__result; }
        return __result;
    }
    void push_back(const _Tp& __x) { new (_M_finish) _Tp(__x); ++_M_finish; }
    // STLport erase(first, last)
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = copy(__last, _M_finish, __first);
        _Destroy(__i, _M_finish);
        _M_finish = __i;
        return __first;
    }
    // STLport erase(position)
    _Tp* erase(_Tp* __position)
    {
        if (__position + 1 != end())
            copy(__position + 1, _M_finish, __position);
        --_M_finish;
        _M_finish->~_Tp();
        return __position;
    }
    // _M_fill_insert at the end with enough capacity (what resize needs here)
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x)
    {
        if (__pos == _M_finish && _M_finish + __n <= _M_end_of_storage)
            for (; __n != 0; --__n) { new (_M_finish) _Tp(__x); ++_M_finish; }
    }
    void insert(_Tp* __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    // STLport resize(n, x) and resize(n)
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};

int main()
{
    int i;
    CGraphicDataBank bank;
    bank.Id = 7;
    {
        vector_model<CGraphicInfo> v(8);
        for (i = 0; i < 6; ++i) v.push_back(CGraphicInfo(&bank, i));
        g_graphicDtors = 0;
        CGraphicInfo* r = v.erase(v.begin() + 1, v.begin() + 3);
        CHECK(r == v.begin() + 1 && v.size() == 4 && g_graphicDtors == 2);
        long expect[4] = { 0, 3, 4, 5 };
        for (i = 0; i < 4; ++i) CHECK(v._M_start[i].Index == expect[i] && v._M_start[i].Header.Width == expect[i] * 16 && v._M_start[i].Bank == &bank);
        CHECK(v._M_finish[0].Bank == 0 && v._M_finish[1].Bank == 0);   // the destroyed tail
        r = v.erase(v.begin() + 2, v.begin() + 2);      // empty range
        CHECK(r == v.begin() + 2 && v.size() == 4 && g_graphicDtors == 2);
        r = v.erase(v.begin(), v.end());
        CHECK(v.size() == 0 && g_graphicDtors == 6);
    }
    std::printf("00a00e60_TEST PASS\n");
    return 0;
}