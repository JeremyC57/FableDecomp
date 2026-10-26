// Behaviour test for 0095a2c0: the model below is a copy of src/0095a2c0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#include <cmath>
using std::sqrt;
static int g_ctors = 0, g_dtors = 0, g_sets = 0, g_gets = 0, g_validCalls = 0, g_enemyValid = 1;
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float Length() const { return (float)sqrt(X * X + Y * Y); }
};
static C3DVector g_pos = { 0, 0, 0 };
class CThing { public:
    const C3DVector* GetPosition() { return &g_pos; }
};
class CThingCreatureBase : public CThing {};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctors; }
    ~CBaseIntelligentPointer() { ++g_dtors; }
    CThing* GetPItem() const { ++g_gets; return (CThing*)m_pItem; }
    void SetPItem(void* item) { ++g_sets; m_pItem = item; }
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00() {}
    CIntelligentPointer() : CBaseIntelligentPointer() {}
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
static CThingCreatureBase* g_enemy = 0;
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() {}
    virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {}
    virtual bool IsRunnable() { return false; }
    virtual void v12() {} virtual void v13() {} virtual void v14() {}
    virtual bool IsEnemyValid() { ++g_validCalls; return g_enemyValid != 0; }
    void* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};
struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CIntelligentPointer<CThingCreatureBase> PTargetThing;
    CThingCreatureBase* GetEnemy() { return g_enemy; }
};
struct CAIStateGroup_HoverAttackArea : CAIStateGroup_EnemyCreatureBase {
    C3DVector StartPos;
    virtual bool IsRunnable();
};

bool CAIStateGroup_HoverAttackArea::IsRunnable()
{
    CThingCreatureBase* enemy = GetEnemy();
    if (enemy) {
        PTargetThing = enemy;
        if (IsEnemyValid()) {
            const C3DVector* pos = PTargetThing.GetPItem()->GetPosition();
            C2DVector delta(pos->X - StartPos.X, pos->Y - StartPos.Y);
            if (!(delta.Length() > 20.0f))
                return true;
        }
    }
    return false;
}

static bool Run(bool hasEnemy, int valid, float px, float py, bool expect, int expectValidCalls)
{
    static CThingCreatureBase creature;
    g_enemy = hasEnemy ? &creature : 0; g_enemyValid = valid; g_pos.X = px; g_pos.Y = py; g_pos.Z = 99.0f;
    CAIStateGroup_HoverAttackArea s; s.PTargetThing.m_pItem = 0; s.StartPos.X = 10.0f; s.StartPos.Y = 10.0f; s.StartPos.Z = 0.0f;
    g_ctors = g_dtors = g_sets = g_gets = g_validCalls = 0;
    bool r = s.IsRunnable();
    if (r != expect || g_validCalls != expectValidCalls) return false;
    if (hasEnemy) {
        if (s.PTargetThing.m_pItem != &creature) return false;      // the target was assigned through the temporary
        if (g_ctors != 1 || g_dtors != 1 || g_sets != 2) return false;
    } else if (g_ctors != 0 || g_sets != 0) return false;
    return true;
}
int main()
{
    bool ok = true;
    ok = ok && Run(true, 1, 15.0f, 10.0f, true, 1);     // 5 away: in range
    ok = ok && Run(true, 1, 30.0f, 10.0f, true, 1);     // exactly 20 away: not > 20, still in range
    ok = ok && Run(true, 1, 40.0f, 30.0f, false, 1);    // 36 away: out of range
    ok = ok && Run(true, 0, 15.0f, 10.0f, false, 1);    // enemy invalid
    ok = ok && Run(false, 1, 15.0f, 10.0f, false, 0);   // no enemy: nothing assigned, validity not asked
    if (!ok) { std::printf("0095a2c0_TEST FAIL\n"); return 1; }
    std::printf("0095a2c0_TEST PASS\n");
    return 0;
}