#include <cstdio>
#include <new>

class CBaseObject { public: int m_id; };
static int g_ctors = 0, g_sets = 0;

struct CBaseIntelligentPointer {
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) { ++g_ctors; }
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { ++g_sets; PData = item; }
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
class CThingCreatureBase;
#pragma pack(push, 1)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)
struct CProng {
    bool                                    Blocked;
    C3DVector                               Prong;
    long                                    ProngBlockageTimer;
    CIntelligentPointer<CThingCreatureBase> POccupier;
    CProng() : Blocked(false), ProngBlockageTimer(0) { }
};
struct __false_type { __false_type() { } };

static CProng* UninitCopy(const CProng* first, const CProng* last, CProng* result, const __false_type&)
{
    CProng* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CProng(*first);
    return cur;
}

int main()
{
    CBaseObject attackers[3];
    CProng src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].Blocked = (i == 1);
        src[i].Prong.X = 1.0f + i; src[i].Prong.Y = 2.0f * i; src[i].Prong.Z = -4.0f;
        src[i].ProngBlockageTimer = 30 * i;
        src[i].POccupier.PData = &attackers[i];
    }
    g_ctors = 0;
    static unsigned char storage[3 * sizeof(CProng)];
    CProng* dst = (CProng*)storage;
    __false_type tag;
    CProng* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && g_ctors == 3 && g_sets == 3;
    for (int i = 0; i < 3; ++i)
        if (dst[i].Blocked != (i == 1) || dst[i].Prong.X != 1.0f + i || dst[i].Prong.Y != 2.0f * i || dst[i].Prong.Z != -4.0f
            || dst[i].ProngBlockageTimer != 30 * i || dst[i].POccupier.PData != &attackers[i]) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst || g_ctors != 3) ok = false;
    for (int i = 0; i < 3; ++i) dst[i].~CProng();
    if (!ok) { std::printf("006f10f0_TEST FAIL\n"); return 1; }
    std::printf("006f10f0_TEST PASS\n");
    return 0;
}