#include <cstdio>
// Behaviour test: the Coin copy (member-wise, the intelligent pointer re-registered through SetPItem) on a copy of the model.
class CBaseObject { public: int m_id; };
static int g_setCalls = 0;
class C2DVector { public: float X, Y; C2DVector() : X(0), Y(0) { } C2DVector(float x, float y) : X(x), Y(y) { } };
class Mass {
public:
    C2DVector Pos, Vel; float IMass, APos, AVel, AIMass;
    Mass() : IMass(0), APos(0), AVel(0), AIMass(0) { }
    virtual ~Mass() { }
};
struct CIntelligentPointer {
    CBaseObject* m_pItem;
    CIntelligentPointer() : m_pItem(0) { }
    CIntelligentPointer(const CIntelligentPointer& o) : m_pItem(0) { SetPItem(o.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }
    virtual ~CIntelligentPointer() { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* p) { m_pItem = p; ++g_setCalls; }
};
class Coin : public Mass {
public:
    bool Owned; CIntelligentPointer Thing; float Radius; int Owner; unsigned int Sound;
    Coin() { }
    Coin(const Coin& other)
        : Mass(other), Owned(other.Owned), Thing(other.Thing), Radius(other.Radius), Owner(other.Owner), Sound(other.Sound) { }
    Coin& operator=(const Coin& other)
    {
        Mass::operator=(other);
        Owned = other.Owned; Thing = other.Thing; Radius = other.Radius; Owner = other.Owner; Sound = other.Sound;
        return *this;
    }
};
int main()
{
    CBaseObject obj; obj.m_id = 3;
    Coin a;
    a.Pos = C2DVector(1, 2); a.Vel = C2DVector(3, 4); a.IMass = 5; a.APos = 6; a.AVel = 7; a.AIMass = 8;
    a.Owned = true; a.Thing.SetPItem(&obj); a.Radius = 9; a.Owner = 1; a.Sound = 0xffffffffu;
    g_setCalls = 0;
    Coin c(a);
    if (g_setCalls != 1 || c.Thing.GetPItem() != &obj || !c.Owned || c.Radius != 9 || c.Owner != 1 || c.Sound != 0xffffffffu
        || c.Pos.X != 1 || c.Pos.Y != 2 || c.Vel.X != 3 || c.Vel.Y != 4 || c.IMass != 5 || c.APos != 6 || c.AVel != 7 || c.AIMass != 8)
    { std::printf("008eff40_TEST FAIL\n"); return 1; }
    std::printf("008eff40_TEST PASS\n");
    return 0;
}