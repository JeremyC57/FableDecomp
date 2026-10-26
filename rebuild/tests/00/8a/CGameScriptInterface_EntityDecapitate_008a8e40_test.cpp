#include <cstdio>
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    unsigned char _pad44[0x6c - 0x44];
    unsigned char m_flags6c;                    // +0x6c bit 3
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0xbc - 0x92];
    unsigned char m_flagsbc;                    // +0xbc bit 1
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};
class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0x114 - 4];
    ~CCombatActionBase();                       // 0x0062E900
};
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
struct CCreatureAction_Decapitate : CCombatActionBase {       // retail sizeof 0x114 (name not recovered)
    CCreatureAction_Decapitate(CThing* victim);          // 0x0066A560
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                        // slot 75 (+0x12c)
};
class CGameScriptInterface {
public:
    virtual void EntityDecapitate(const CScriptThing& thing) const;
};
void CGameScriptInterface::EntityDecapitate(const CScriptThing& thing) const
{
    if (thing.IsValid())
    {
        CThing* t = thing.GetThing();
        if (t->HasTC(0xd1) && (t->m_flags6c & 0xf) && (t->m_flagsbc & 2))
        {
            CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(t);
            creature->SetCurrentAction(CCreatureAction_Decapitate(t));
        }
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
void CScriptThing::v012() {}
void CScriptThing::v013() {}
void CScriptThing::v014() {}
void CScriptThing::v015() {}
void CScriptThing::v016() {}
void CScriptThing::v017() {}
void CScriptThing::v018() {}
void CScriptThing::v019() {}
void CScriptThing::v020() {}
void CScriptThing::v021() {}
void CScriptThing::v022() {}
void CScriptThing::v023() {}
void CScriptThing::v024() {}
void CScriptThing::v025() {}
void CScriptThing::v026() {}
void CScriptThing::v027() {}
void CScriptThing::v028() {}
void CScriptThing::v029() {}
void CScriptThing::v030() {}
void CScriptThing::v031() {}
void CScriptThing::v032() {}
void CScriptThing::v033() {}
void CScriptThing::v034() {}
void CScriptThing::v035() {}
void CScriptThing::v036() {}
void CScriptThing::v037() {}
void CScriptThing::v038() {}
void CScriptThing::v039() {}
void CScriptThing::v040() {}
void CScriptThing::v041() {}
void CScriptThing::v042() {}
void CScriptThing::v043() {}
void CScriptThing::v044() {}
void CScriptThing::v045() {}
void CScriptThing::v046() {}
void CScriptThing::v047() {}
void CScriptThing::v048() {}
void CScriptThing::v049() {}
void CScriptThing::v050() {}
void CScriptThing::v051() {}
void CScriptThing::v052() {}
void CScriptThing::v053() {}
void CScriptThing::v054() {}
void CScriptThing::v055() {}
void CScriptThing::v056() {}
void CScriptThing::v057() {}
void CScriptThing::v058() {}
void CScriptThing::v059() {}
void CScriptThing::v060() {}
void CScriptThing::v061() {}
void CScriptThing::v062() {}
void CScriptThing::v063() {}
void CScriptThing::v064() {}
void CScriptThing::v065() {}
void CScriptThing::v066() {}
void CScriptThing::v067() {}
void CScriptThing::v068() {}
void CScriptThing::v069() {}
void CScriptThing::v070() {}
void CScriptThing::v071() {}
void CScriptThing::v072() {}
void CScriptThing::v073() {}
void CScriptThing::v074() {}
CThing* CScriptThing::GetThing() const { return 0; }
bool CScriptThing::IsValid() const { return false; }
class CTestScriptThing : public CScriptThing { public: CThing* T; bool Valid; CThing* GetThing() const { return T; } bool IsValid() const { return Valid; } };
static int g_ctor, g_dtor, g_set; static CThing* g_victim; static const CCombatActionBase* g_built; static const CCombatActionBase* g_given; static CThingCreatureBase* g_on;
void CCombatActionBase::v00() {}
CCombatActionBase::~CCombatActionBase() { ++g_dtor; }
CCreatureAction_Decapitate::CCreatureAction_Decapitate(CThing* v) { ++g_ctor; g_victim = v; g_built = this; }
bool CThingCreatureBase::SetCurrentAction(const CCombatActionBase& a) { ++g_set; g_given = &a; g_on = this; return true; }
int main()
{
    CThingCreatureBase c; for (int i = 0; i < 9; ++i) c.UsingComponents[i] = 0;
    c.m_flags6c = 0; c.m_flagsbc = 0;
    CTestScriptThing s; s.T = &c; s.Valid = false;
    CGameScriptInterface gsi;
    c.UsingComponents[6] = 1UL << 17; c.m_flags6c = 4; c.m_flagsbc = 2;
    gsi.EntityDecapitate(s); bool ok = g_ctor == 0;
    s.Valid = true; c.UsingComponents[6] = ~(1UL << 17); gsi.EntityDecapitate(s); ok = ok && g_ctor == 0;
    c.UsingComponents[6] = 1UL << 17; c.m_flags6c = 0xf0; gsi.EntityDecapitate(s); ok = ok && g_ctor == 0;
    c.m_flags6c = 1; c.m_flagsbc = 0xfd; gsi.EntityDecapitate(s); ok = ok && g_ctor == 0;
    c.m_flagsbc = 2; gsi.EntityDecapitate(s);
    ok = ok && g_ctor == 1 && g_set == 1 && g_dtor == 1 && g_victim == &c && g_on == &c && g_given == g_built;
    std::printf(ok ? "008a8e40_TEST PASS\n" : "008a8e40_TEST FAIL\n");
    return ok ? 0 : 1;
}