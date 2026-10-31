// Behaviour test for std::__uninitialized_fill_n<NPlayerTargeting::CTargetPoint*> (0x00806C80): fills n raw slots with copies of x (vector words + the intelligent pointer item) and returns first + n; n == 0 returns first.
#include <cstdio>
#include <new>
class CBaseObject { public: int id; };
class CThing;
class CBaseIntelligentPointer {
public:
    void* PData;
    CBaseIntelligentPointer() : PData(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CBaseObject* GetPItem() const { return (CBaseObject*)PData; }
    void SetPItem(CBaseObject* item) { PData = item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { CBaseObject* item = other.GetPItem(); SetPItem(item); }
    virtual ~CIntelligentPointer() {}
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
namespace std_ {
struct __false_type { __false_type() {} };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}
namespace NPlayerTargeting { class CTargetPoint { public: CIntelligentPointer<CThing> PThing; long Index; C3DVector TargetingPosition; long FrameRejected; }; }
typedef NPlayerTargeting::CTargetPoint Elem;
int main()
{
    CBaseObject obj; obj.id = 5;
    Elem x;
    x.TargetingPosition.X = 1.0f; x.TargetingPosition.Y = 2.0f; x.TargetingPosition.Z = 3.0f;
    x.PThing.SetPItem(&obj);
    x.Index = 7; x.FrameRejected = 99;
    static unsigned char raw[sizeof(Elem) * 4 + 16];
    Elem* buf = (Elem*)(void*)(((unsigned long)(raw + 15)) & ~15UL);
    std_::__false_type tag;
    Elem* end = std_::__uninitialized_fill_n(buf, 3u, x, tag);
    bool ok = (end == buf + 3);
    for (int i = 0; i < 3; ++i) {
        if (buf[i].TargetingPosition.X != 1.0f || buf[i].TargetingPosition.Y != 2.0f || buf[i].TargetingPosition.Z != 3.0f) ok = false;
        if (buf[i].PThing.GetPItem() != &obj) ok = false;
        if (buf[i].Index != 7 || buf[i].FrameRejected != 99) ok = false;
    }
    if (std_::__uninitialized_fill_n(buf, 0u, x, tag) != buf) ok = false;
    for (int j = 0; j < 3; ++j) buf[j].~Elem();
    if (!ok) { std::printf("00806c80_TEST FAIL\n"); return 1; }
    std::printf("00806c80_TEST PASS\n");
    return 0;
}