// Behaviour test for 0x0078E600 CTCDivineWrath::PerEffectInfo copy constructor.
#include <cstdio>
#include <cstring>

class CBaseObject {
public:
    int Id;
};
class CBaseIntelligentPointer {
public:
    CBaseObject* PData;
    static int s_sets;
    CBaseIntelligentPointer() : PData(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; ++s_sets; }
};
int CBaseIntelligentPointer::s_sets = 0;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    explicit CIntelligentPointer(CBaseObject* p) { SetPItem(p); }
    CIntelligentPointer(const CIntelligentPointer<T>& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
};
class CThing;
class CThingAICreature;
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CTCDivineWrath {
public:
    struct PerEffectInfo {
        C3DVector                   Position;
        CIntelligentPointer<CThing> Effect;
        PerEffectInfo(const C3DVector& p, CBaseObject* t) : Position(p), Effect(t) {}
        PerEffectInfo(const PerEffectInfo& other) : Position(other.Position), Effect(other.Effect) {}
    };
};

int main()
{
    CBaseObject obj; obj.Id = 9;
    CTCDivineWrath::PerEffectInfo a(C3DVector(12.5f, -3.0f, 0.25f), &obj);
    CBaseIntelligentPointer::s_sets = 0;
    CTCDivineWrath::PerEffectInfo b(a);
    if (!(CBaseIntelligentPointer::s_sets == 1 && b.Effect.GetPItem() == &obj && Near(b.Position.X, 12.5f) && Near(b.Position.Y, -3.0f) && Near(b.Position.Z, 0.25f))) {
        std::printf("0078e600_TEST FAIL\n");
        return 1;
    }
    std::printf("0078e600_TEST PASS\n");
    return 0;
}