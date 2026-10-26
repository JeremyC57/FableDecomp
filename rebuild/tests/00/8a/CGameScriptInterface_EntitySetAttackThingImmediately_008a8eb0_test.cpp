#include <cstdio>
class CThing { public:
    unsigned char _pad00[0x6c];
    unsigned char m_flags6c;                    // +0x6c bit 3
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};
inline bool IS_THING_ALIVE(const CThing* t)
{
    return t != 0 && !(t->m_flags91 & 1);
}
class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0x114 - 4];
    ~CCombatActionBase();                       // 0x0062E900
};
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
struct CCreatureAction_AttackThing : CCombatActionBase {      // retail sizeof 0x118 (name not recovered)
    unsigned char _body[4];
    CCreatureAction_AttackThing(CThingCreatureBase* owner, CThing* target);   // 0x008AE160
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
};
class CGameScriptInterface {
public:
    virtual void EntitySetAttackThingImmediately(const CScriptThing& attacker, const CScriptThing& target, bool, bool) const;
};
void CGameScriptInterface::EntitySetAttackThingImmediately(const CScriptThing& attacker, const CScriptThing& target, bool, bool) const
{
    CThing* a = attacker.GetThing();
    CThing* t = target.GetThing();
    if (IS_THING_ALIVE(a) && IS_THING_ALIVE(t) && (a->m_flags6c & 8))
    {
        CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(a);
        creature->SetCurrentAction(CCreatureAction_AttackThing(creature, t));
    }
}

void CScriptThing::v000() {}
void CScriptThing::v001() {}
void CScriptThing::v002() {}
void CScriptThing::v003() {}
void CScriptThing::v004() {}
void CScriptThing::v005() {}
void CScriptThing::v006() {}
void CScriptThing::v007() {}
void CScriptThing::v008() {}
void CScriptThing::v009() {}
void CScriptThing::v010() {}
CThing* CScriptThing::GetThing() const { return 0; }
class CTestScriptThing : public CScriptThing { public: CThing* T; CThing* GetThing() const { return T; } };
static int g_ctor, g_dtor, g_set; static CThing* g_owner; static CThing* g_target; static const CCombatActionBase* g_built; static const CCombatActionBase* g_given;
void CCombatActionBase::v00() {}
CCombatActionBase::~CCombatActionBase() { ++g_dtor; }
CCreatureAction_AttackThing::CCreatureAction_AttackThing(CThingCreatureBase* owner, CThing* target) { ++g_ctor; g_owner = owner; g_target = target; g_built = this; }
bool CThingCreatureBase::SetCurrentAction(const CCombatActionBase& a) { ++g_set; g_given = &a; return true; }
int main()
{
    CThingCreatureBase a; CThing t; a.m_flags6c = 8; a.m_flags91 = 0; t.m_flags91 = 0;
    CTestScriptThing sa, st; sa.T = &a; st.T = &t;
    CGameScriptInterface gsi;
    gsi.EntitySetAttackThingImmediately(sa, st, true, false);
    bool ok = g_ctor == 1 && g_set == 1 && g_dtor == 1 && g_owner == &a && g_target == &t && g_given == g_built;
    t.m_flags91 = 1; gsi.EntitySetAttackThingImmediately(sa, st, false, false); ok = ok && g_ctor == 1;
    t.m_flags91 = 0; a.m_flags6c = 0x7; gsi.EntitySetAttackThingImmediately(sa, st, false, false); ok = ok && g_ctor == 1;
    a.m_flags6c = 8; a.m_flags91 = 1; gsi.EntitySetAttackThingImmediately(sa, st, false, false); ok = ok && g_ctor == 1;
    a.m_flags91 = 0; st.T = 0; gsi.EntitySetAttackThingImmediately(sa, st, false, false); ok = ok && g_ctor == 1;
    sa.T = 0; st.T = &t; gsi.EntitySetAttackThingImmediately(sa, st, false, false); ok = ok && g_ctor == 1 && g_set == 1;
    std::printf(ok ? "008a8eb0_TEST PASS\n" : "008a8eb0_TEST FAIL\n");
    return ok ? 0 : 1;
}