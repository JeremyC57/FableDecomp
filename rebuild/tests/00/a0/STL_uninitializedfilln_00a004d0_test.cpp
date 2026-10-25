// Behaviour test for 0x00A004D0 std::__uninitialized_fill_n<CGraphicInfoFrame*>: n copies of x are copy-constructed into raw storage (each taking a
// reference on x's frame data) and first + n is returned; n == 0 constructs nothing.
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

CGraphicInfoFrame* __uninitialized_fill_n(CGraphicInfoFrame* first, unsigned int n, const CGraphicInfoFrame& x)
{
    CGraphicInfoFrame* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CGraphicInfoFrame(x);
    return cur;
}

int main()
{
    CFrameData data(1);
    CGraphicInfoFrame x;
    SetFrame(x, 7, -2, &data, 0x40);
    void* raw = std::malloc(3 * sizeof(CGraphicInfoFrame));
    CGraphicInfoFrame* first = (CGraphicInfoFrame*)raw;
    long refs = data.RefCount;
    bool ok = __uninitialized_fill_n(first, 0, x) == first && data.RefCount == refs;
    CGraphicInfoFrame* end = __uninitialized_fill_n(first, 3, x);
    ok = ok && end == first + 3 && data.RefCount == refs + 3 && first[2].FrameNum == 7 && first[1].SmallMipmapLevel == -2
            && first[0].Data.Object == &data && first[2].GraphicInfo == (CGraphicInfo*)0x40;
    for (int i = 0; i < 3; ++i) first[i].~CGraphicInfoFrame();
    ok = ok && data.RefCount == refs;
    std::free(raw);
    if (!ok) { std::printf("00a004d0_TEST FAIL\n"); return 1; }
    std::printf("00a004d0_TEST PASS\n");
    return 0;
}