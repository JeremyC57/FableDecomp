#include <cstdio>

struct CBaseObject { int m_id; };
static int g_gets = 0, g_sets = 0;

struct CBaseIntelligentPointer {
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) { }
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { ++g_gets; return PData; }
    void SetPItem(CBaseObject* item) { ++g_sets; PData = item; }
};
template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CThing;
struct C3DVector {
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
    CTargetPoint& operator=(const CTargetPoint& other)
    {
        PThing = other.PThing;
        Index = other.Index;
        TargetingPosition = other.TargetingPosition;
        FrameRejected = other.FrameRejected;
        return *this;
    }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };

static CTargetPoint* Copy(const CTargetPoint* first, const CTargetPoint* last, CTargetPoint* result, const random_access_iterator_tag&, int*)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    CBaseObject objs[3];
    CTargetPoint src[3], dst[4];
    for (int i = 0; i < 3; ++i) {
        src[i].PThing.PData = &objs[i];
        src[i].Index = i + 1;
        src[i].TargetingPosition.X = 1.5f * i; src[i].TargetingPosition.Y = 2.0f; src[i].TargetingPosition.Z = -3.0f * i;
        src[i].FrameRejected = 100 + i;
    }
    random_access_iterator_tag tag;
    CTargetPoint* r = Copy(src, src + 3, dst, tag, 0);
    bool ok = r == dst + 3 && g_gets == 3 && g_sets == 3;
    for (int i = 0; i < 3; ++i)
        if (dst[i].PThing.PData != &objs[i] || dst[i].Index != i + 1 || dst[i].TargetingPosition.X != 1.5f * i
            || dst[i].TargetingPosition.Z != -3.0f * i || dst[i].FrameRejected != 100 + i) ok = false;
    if (dst[3].PThing.PData != 0 || dst[3].Index != 0) ok = false;
    if (Copy(src, src, dst + 2, tag, 0) != dst + 2 || g_sets != 3) ok = false;
    if (!ok) { std::printf("00806ea0_TEST FAIL\n"); return 1; }
    std::printf("00806ea0_TEST PASS\n");
    return 0;
}