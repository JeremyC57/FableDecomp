// Behaviour test for 0x008073D0 std::swap<NPlayerTargeting::CTargetPoint>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("008073d0_TEST FAIL\n"); return 1; } } while (0)

class CBaseObject { public: int Id; };
static int g_ptrSets = 0, g_ptrDtors = 0;
class CBaseIntelligentPointer {                         // test model: vptr + item
public:
    CBaseObject* PItem;
    CBaseIntelligentPointer() : PItem(0) { }
    virtual ~CBaseIntelligentPointer() { ++g_ptrDtors; PItem = 0; }
    CBaseObject* GetPItem() const { return PItem; }
    void SetPItem(CBaseObject* item) { PItem = item; ++g_ptrSets; }
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(CBaseObject* item) { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
static bool Near(float a, float b) { return a - b < 0.001f && b - a < 0.001f; }

class CThing;
namespace NPlayerTargeting {
class CTargetPoint {
public:
    CIntelligentPointer<CThing> PThing;
    long                        Index;
    C3DVector                   TargetingPosition;
    long                        FrameRejected;
    CTargetPoint(CBaseObject* t, long i, const C3DVector& p, long f) : PThing(t), Index(i), TargetingPosition(p), FrameRejected(f) { }
};
}

template <class _Tp> inline void swap_model(_Tp& __a, _Tp& __b)    // STLport swap
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

int main()
{
    CBaseObject t1, t2;
    t1.Id = 5; t2.Id = 6;
    NPlayerTargeting::CTargetPoint a(&t1, 3, C3DVector(4.0f, 5.0f, 6.0f), 100);
    NPlayerTargeting::CTargetPoint b(&t2, 7, C3DVector(-4.0f, -5.0f, -6.0f), 200);
    g_ptrSets = g_ptrDtors = 0;
    swap_model(a, b);
    CHECK(a.PThing.GetPItem() == &t2 && a.Index == 7 && Near(a.TargetingPosition.X, -4.0f) && Near(a.TargetingPosition.Z, -6.0f) && a.FrameRejected == 200);
    CHECK(b.PThing.GetPItem() == &t1 && b.Index == 3 && Near(b.TargetingPosition.Y, 5.0f) && b.FrameRejected == 100);
    CHECK(g_ptrSets == 3 && g_ptrDtors == 1);
    std::printf("008073d0_TEST PASS\n");
    return 0;
}