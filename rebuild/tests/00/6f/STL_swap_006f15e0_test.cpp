// Behaviour test for 0x006F15E0 _STL::swap<CDefendingCombatantInfo::CProng>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("006f15e0_TEST FAIL\n"); return 1; } } while (0)

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

class CThingCreatureBase;
class CDefendingCombatantInfo {
public:
    class CProng {
    public:
        bool                                    Blocked;
        C3DVector                               Prong;
        long                                    ProngBlockageTimer;
        CIntelligentPointer<CThingCreatureBase> POccupier;
        CProng(bool b, const C3DVector& p, long t, CBaseObject* o) : Blocked(b), Prong(p), ProngBlockageTimer(t), POccupier(o) { }
    };
};

template <class _Tp> inline void swap_model(_Tp& __a, _Tp& __b)    // STLport swap
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

int main()
{
    CBaseObject occA, occB;
    occA.Id = 1; occB.Id = 2;
    CDefendingCombatantInfo::CProng a(true, C3DVector(1.0f, 2.0f, 3.0f), 10, &occA);
    CDefendingCombatantInfo::CProng b(false, C3DVector(-1.0f, 0.5f, 9.0f), 20, &occB);
    g_ptrSets = g_ptrDtors = 0;
    swap_model(a, b);
    CHECK(!a.Blocked && Near(a.Prong.X, -1.0f) && Near(a.Prong.Y, 0.5f) && Near(a.Prong.Z, 9.0f) && a.ProngBlockageTimer == 20 && a.POccupier.GetPItem() == &occB);
    CHECK(b.Blocked && Near(b.Prong.X, 1.0f) && Near(b.Prong.Y, 2.0f) && Near(b.Prong.Z, 3.0f) && b.ProngBlockageTimer == 10 && b.POccupier.GetPItem() == &occA);
    CHECK(g_ptrSets == 3 && g_ptrDtors == 1);
    std::printf("006f15e0_TEST PASS\n");
    return 0;
}