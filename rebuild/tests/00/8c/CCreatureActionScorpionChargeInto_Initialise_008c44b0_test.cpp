// Behaviour test for CCreatureAction_ScorpionChargeInto::Initialise (0x008C44B0): CHARGE_INTO, base Initialise, a live
// target's 0x93 component turned toward the scorpion, an obstruction created at position + facing*(radius+0.2) with the
// def radius and kept in PObstruction; the willpower-indicator def is released.
#include <cstdio>
#include <cstring>
#include <cmath>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(s * X, s * Y, s * Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
static char g_anim[32]; static int g_baseInit = 0, g_dirCalls = 0, g_destroyed = 0, g_creates = 0;
class CThing;
static CThing* g_dirTowards = 0; static C3DVector g_ca, g_cb; static float g_cr = 0; static CThing* g_createResult = 0;
class CAICreatureWillPowerIndicatorDef { public: long RefCount; float Radius; void Destroy() { ++g_destroyed; } };
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
};
class CDefClassBase { public:
    CAICreatureWillPowerIndicatorDef* Sub;
    bool GetSubDef(CDefPointer<CAICreatureWillPowerIndicatorDef>* out) { ++Sub->RefCount; out->Object = Sub; return true; }
};
struct CPhysicsBody { C3DVector Facing; C3DVector GetFacing() const { return Facing; } };
struct CTC93 { void SetDirection(CThing* t, bool f) { if (f) { g_dirTowards = t; ++g_dirCalls; } } };
class CThing { public:
    unsigned char m_flags91; CPhysicsBody* PhysicsBody; CDefClassBase* Def; C3DVector Pos; CTC93* C93;
    CPhysicsBody* GetPhysics() const { return PhysicsBody; }
    CDefClassBase* GetDef() const { return Def; }
    const C3DVector& GetPosition() const { return Pos; }
    bool GetInterface(CTC93** o, long) { if (!C93) return false; *o = C93; return true; }
};
CThing* CreateObstruction(const C3DVector& a, const C3DVector& b, float r) { g_ca = a; g_cb = b; g_cr = r; ++g_creates; return g_createResult; }
struct CIntelligentPointerThing {
    CThing* m_pItem;
    CIntelligentPointerThing() : m_pItem(0) {}
    CIntelligentPointerThing(CThing* item) : m_pItem(item) {}
    CThing* GetPItem() const { return m_pItem; }
};
inline bool IS_THING_ALIVE(const CIntelligentPointerThing& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CCharString { public: const char* s; CCharString(const char* t) : s(t) {} };
class CCombatActionBase { public:
    CIntelligentPointerThing PCreature, PTarget;
    void SetCombatAnimation(const CCharString& n) { std::strcpy(g_anim, n.s); }
    void Initialise() { ++g_baseInit; }
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_ScorpionChargeInto : public CCombatActionBase { public: float ChargeDistance; CIntelligentPointerThing PObstruction; void Initialise(); };

void CCreatureAction_ScorpionChargeInto::Initialise()
{
    SetCombatAnimation("CHARGE_INTO");
    CCombatActionBase::Initialise();
    CTC93* direction;
    if (IS_THING_ALIVE(PTarget) && PTarget.GetPItem()->GetInterface(&direction, 0x93))
        direction->SetDirection(GetCreature(), true);
    CDefPointer<CAICreatureWillPowerIndicatorDef> def;
    GetCreature()->GetDef()->GetSubDef(&def);
    C3DVector offset = GetCreature()->GetPhysics()->GetFacing() * (def.Object->Radius + 0.2f);
    CThing* obstruction = CreateObstruction(GetCreature()->GetPosition() + offset, GetCreature()->GetPosition() + offset,
                                            def.Object->Radius);
    if (obstruction)
        PObstruction = CIntelligentPointerThing(obstruction);
}

static int near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    int ok = 1;
    CAICreatureWillPowerIndicatorDef sub; sub.RefCount = 1; sub.Radius = 1.8f;
    CDefClassBase def; def.Sub = &sub; CPhysicsBody body; body.Facing = C3DVector(0, 1, 0);
    CTC93 c93;
    CThing scorpion; scorpion.m_flags91 = 0; scorpion.PhysicsBody = &body; scorpion.Def = &def; scorpion.Pos = C3DVector(10, 20, 30); scorpion.C93 = 0;
    CThing target; target.m_flags91 = 0; target.C93 = &c93;
    CThing obstacle;
    CCreatureAction_ScorpionChargeInto a; a.PCreature.m_pItem = &scorpion; a.PTarget.m_pItem = &target;
    g_createResult = &obstacle;
    a.Initialise();
    if (std::strcmp(g_anim, "CHARGE_INTO") != 0 || g_baseInit != 1 || g_dirCalls != 1 || g_dirTowards != &scorpion) ok = 0;
    if (!near(g_ca.X, 10) || !near(g_ca.Y, 22.0f) || !near(g_ca.Z, 30) || !near(g_cb.Y, 22.0f) || !near(g_cr, 1.8f)) ok = 0;
    if (a.PObstruction.m_pItem != &obstacle || sub.RefCount != 1 || g_destroyed != 0) ok = 0;
    // dead target: no turn; no obstruction keeps PObstruction; last reference destroys the def
    target.m_flags91 = 1; g_createResult = 0; sub.RefCount = 0;
    a.Initialise();
    if (g_dirCalls != 1 || a.PObstruction.m_pItem != &obstacle || g_creates != 2 || g_destroyed != 1) ok = 0;
    std::printf("008c44b0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}