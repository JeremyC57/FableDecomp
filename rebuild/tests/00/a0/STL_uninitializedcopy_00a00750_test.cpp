// Behaviour test for 0x00A00750 _STL::__uninitialized_copy<CGraphicInfoFrame*>: [first, last) is copy-constructed into raw storage at result (each
// copy taking a reference on its frame data) and the end of the destination is returned.
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

CGraphicInfoFrame* __uninitialized_copy(CGraphicInfoFrame* first, CGraphicInfoFrame* last, CGraphicInfoFrame* result)
{
    CGraphicInfoFrame* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CGraphicInfoFrame(*first);
    return cur;
}

int main()
{
    CFrameData d0(1), d1(1);
    CGraphicInfoFrame src[3];
    SetFrame(src[0], 1, 0, &d0, 0x10);
    SetFrame(src[1], 2, 1, 0, 0x20);
    SetFrame(src[2], 3, 2, &d1, 0x30);
    void* raw = std::malloc(3 * sizeof(CGraphicInfoFrame));
    CGraphicInfoFrame* dst = (CGraphicInfoFrame*)raw;
    long r0 = d0.RefCount, r1 = d1.RefCount;
    bool ok = __uninitialized_copy(src, src, dst) == dst;
    CGraphicInfoFrame* end = __uninitialized_copy(src, src + 3, dst);
    ok = ok && end == dst + 3 && d0.RefCount == r0 + 1 && d1.RefCount == r1 + 1 && dst[1].Data.Object == 0
            && dst[2].FrameNum == 3 && dst[2].SmallMipmapLevel == 2 && dst[0].GraphicInfo == (CGraphicInfo*)0x10;
    for (int i = 0; i < 3; ++i) dst[i].~CGraphicInfoFrame();
    std::free(raw);
    if (!ok) { std::printf("00a00750_TEST FAIL\n"); return 1; }
    std::printf("00a00750_TEST PASS\n");
    return 0;
}