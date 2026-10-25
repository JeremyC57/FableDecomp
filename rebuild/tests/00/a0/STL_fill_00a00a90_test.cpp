// Behaviour test for 0x00A00A90 std::fill<CGraphicInfoFrame*>: every element of [first, last) is assigned val (self-assignment of shared data
// skips the release).
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

void fill(CGraphicInfoFrame* first, CGraphicInfoFrame* last, const CGraphicInfoFrame& val)
{
    for (; first != last; ++first)
        *first = val;
}

int main()
{
    CFrameData shared(1), other(1);
    CGraphicInfoFrame a[3], v;
    SetFrame(a[0], 1, 0, &other, 0);
    SetFrame(a[1], 2, 0, &shared, 0);
    SetFrame(a[2], 3, 0, 0, 0);
    SetFrame(v, 9, 4, &shared, 0x99);
    gReleases = 0;
    long refs = shared.RefCount;
    fill(a, a + 3, v);
    bool ok = a[0].FrameNum == 9 && a[2].SmallMipmapLevel == 4 && a[1].GraphicInfo == (CGraphicInfo*)0x99
           && a[0].Data.Object == &shared && a[2].Data.Object == &shared && shared.RefCount == refs + 2
           && other.RefCount == 1 && gReleases == 1;
    fill(a, a, v);
    ok = ok && shared.RefCount == refs + 2;
    if (!ok) { std::printf("00a00a90_TEST FAIL\n"); return 1; }
    std::printf("00a00a90_TEST PASS\n");
    return 0;
}