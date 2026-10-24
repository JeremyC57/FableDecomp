// Behaviour test for 0x00A00B20 _STL::__copy<CGraphicInfoFrame*>: [first, last) is assigned forwards onto result (an overlapping shift left
// works) and the end of the destination is returned.
#include <cstdlib>
#include <cstdio>
#include <new>

static int gReleases = 0, gDeletes = 0;

class CBaseClass {
public:
    virtual ~CBaseClass() {}
};

template <class T>
class CIVCountedPointer {
public:
    T* Object;
    CIVCountedPointer() : Object(0) {}
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->AddRef();
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
    ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

class CGraphicInfo;

class CGraphicInfoFrame : public CBaseClass {
public:
    class CFrameData {
    public:
        CFrameData(long refs) : RefCount(refs) {}
        virtual ~CFrameData() {}
        virtual void DeleteThis() { ++gDeletes; }
        long RefCount;
        void AddRef() { ++RefCount; }
        void Release()
        {
            ++gReleases;
            if (--RefCount == 0)
                DeleteThis();
        }
    };
    unsigned short FrameNum;
    signed char SmallMipmapLevel;
    CIVCountedPointer<CFrameData> Data;
    CGraphicInfo* GraphicInfo;
};
typedef CGraphicInfoFrame::CFrameData CFrameData;

static void SetFrame(CGraphicInfoFrame& f, unsigned short num, signed char mip, CFrameData* data, long info)
{
    f.FrameNum = num;
    f.SmallMipmapLevel = mip;
    CIVCountedPointer<CFrameData> p;
    p.Object = data;
    if (data) data->AddRef();
    f.Data = p;
    f.GraphicInfo = (CGraphicInfo*)info;
}

CGraphicInfoFrame* __copy(CGraphicInfoFrame* first, CGraphicInfoFrame* last, CGraphicInfoFrame* result)
{
    for (int n = (int)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    CFrameData d[4] = { CFrameData(1), CFrameData(1), CFrameData(1), CFrameData(1) };
    CGraphicInfoFrame a[4];
    for (int i = 0; i < 4; ++i) SetFrame(a[i], (unsigned short)(i + 1), (signed char)i, &d[i], 0x100 * i);
    gReleases = 0;
    CGraphicInfoFrame* r = __copy(a + 2, a + 4, a + 1);                       // shift [2,4) left by one
    bool ok = r == a + 3 && a[0].FrameNum == 1 && a[1].FrameNum == 3 && a[2].FrameNum == 4 && a[3].FrameNum == 4
           && a[1].Data.Object == &d[2] && a[2].Data.Object == &d[3] && d[1].RefCount == 1 && d[2].RefCount == 2 && gDeletes == 0
           && d[3].RefCount == 3 && a[1].GraphicInfo == (CGraphicInfo*)0x200 && gReleases == 2;
    ok = ok && __copy(a, a, a + 1) == a + 1;
    if (!ok) { std::printf("00a00b20_TEST FAIL\n"); return 1; }
    std::printf("00a00b20_TEST PASS\n");
    return 0;
}