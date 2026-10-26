#include <cstdio>
#include <new>

class CBaseObject { public: int m_id; };
static int g_ctors = 0, g_gets = 0, g_sets = 0;

struct CBaseIntelligentPointer {
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) { ++g_ctors; }
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { ++g_gets; return PData; }
    void SetPItem(CBaseObject* item) { ++g_sets; PData = item; }
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
class CThing;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CTargetPoint {
    CIntelligentPointer<CThing> PThing;
    long                        Index;
    C3DVector                   TargetingPosition;
    long                        FrameRejected;
    CTargetPoint() : Index(0), FrameRejected(0) { }
    CTargetPoint(const CTargetPoint& other)
        : PThing(other.PThing), Index(other.Index), TargetingPosition(other.TargetingPosition),
          FrameRejected(other.FrameRejected) { }
};
struct __false_type { __false_type() { } };

static CTargetPoint* UninitCopy(const CTargetPoint* first, const CTargetPoint* last, CTargetPoint* result, const __false_type&)
{
    CTargetPoint* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CTargetPoint(*first);
    return cur;
}

int main()
{
    CBaseObject objs[3];
    CTargetPoint src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].PThing.PData = &objs[i];
        src[i].Index = 5 + i;
        src[i].TargetingPosition.X = 0.5f * i; src[i].TargetingPosition.Y = -1.0f; src[i].TargetingPosition.Z = 8.0f + i;
        src[i].FrameRejected = 1000 + i;
    }
    g_ctors = 0;
    static unsigned char storage[3 * sizeof(CTargetPoint)];
    CTargetPoint* dst = (CTargetPoint*)storage;
    __false_type tag;
    CTargetPoint* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && g_ctors == 3 && g_gets == 3 && g_sets == 3;
    for (int i = 0; i < 3; ++i)
        if (dst[i].PThing.PData != &objs[i] || dst[i].Index != 5 + i || dst[i].TargetingPosition.X != 0.5f * i
            || dst[i].TargetingPosition.Z != 8.0f + i || dst[i].FrameRejected != 1000 + i) ok = false;
    if (UninitCopy(src, src, dst + 1, tag) != dst + 1 || g_ctors != 3) ok = false;
    for (int i = 0; i < 3; ++i) dst[i].~CTargetPoint();
    if (!ok) { std::printf("00807980_TEST FAIL\n"); return 1; }
    std::printf("00807980_TEST PASS\n");
    return 0;
}