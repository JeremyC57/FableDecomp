// Behaviour test for 0x00A016F0 vector<CGraphicInfoFrame>::resize(n): shrinking erases [begin + n, end); growing (or the same
// size) fill-inserts n - size() copies of a default frame (null Data) at end(); the temporary is destroyed once.
#include <cstdio>

static int gBaseDtors = 0, gErases = 0, gFills = 0;
static long gEraseFirst = -1, gEraseLast = -1, gFillPos = -1;
static unsigned int gFillCount = 0;
static bool gFillDataNull = false;

class CBaseClass {
public:
    virtual ~CBaseClass() { ++gBaseDtors; }
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    CIVCountedPointer() : Object(0) {}
    ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

class CGraphicInfoFrame : public CBaseClass {
public:
    class CFrameData {
    public:
        long RefCount;
        void Release() { --RefCount; }
    };
    unsigned short FrameNum;
    signed char SmallMipmapLevel;
    CIVCountedPointer<CFrameData> Data;
    void* GraphicInfo;
    CGraphicInfoFrame() {}
};

template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T* erase(T* first, T* last) { ++gErases; gEraseFirst = (long)(first - _M_start); gEraseLast = (long)(last - _M_start); return first; }
    void _M_fill_insert(T* pos, unsigned int n, const T& x)
    {
        ++gFills; gFillPos = (long)(pos - _M_start); gFillCount = n; gFillDataNull = x.Data.Object == 0;
    }
    void insert(T* pos, unsigned int n, const T& x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int new_size, const T& x)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
    void resize(unsigned int new_size) { resize(new_size, T()); }
};

int main()
{
    CGraphicInfoFrame frames[4];
    vector<CGraphicInfoFrame> v;
    v._M_start = frames; v._M_finish = frames + 3; v._M_end_of_storage = frames + 4;
    gBaseDtors = 0;
    v.resize(1);
    bool ok = gErases == 1 && gEraseFirst == 1 && gEraseLast == 3 && gFills == 0 && gBaseDtors == 1;
    v.resize(5);
    ok = ok && gFills == 1 && gFillPos == 3 && gFillCount == 2 && gFillDataNull && gErases == 1 && gBaseDtors == 2;
    v.resize(3);
    ok = ok && gFills == 2 && gFillCount == 0 && gErases == 1 && gBaseDtors == 3;
    if (!ok) { std::printf("00a016f0_TEST FAIL\n"); return 1; }
    std::printf("00a016f0_TEST PASS\n");
    return 0;
}