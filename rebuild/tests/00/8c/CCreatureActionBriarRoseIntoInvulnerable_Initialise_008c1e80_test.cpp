// Behaviour test for 0x008C1E80 CCreatureAction_BriarRoseIntoInvulnerable::Initialise.
#include <cstdio>
#include <cstring>
struct CThing;
struct CModeBase { virtual ~CModeBase() {} };
struct CModeBriarRoseTeleportInvulnerable : CModeBase { CThing* o; CThing* e; CModeBriarRoseTeleportInvulnerable(CThing* a, CThing* b) : o(a), e(b) {} };
struct CTCCreatureModeManager { CModeBase* added; void AddMode(CModeBase* m) { added = m; } };
struct CThing {
    bool has; unsigned char m_flags91; CTCCreatureModeManager mm;
    bool GetInterface(CTCCreatureModeManager** pp, long t) { if (t == 0x31 && has) { *pp = &mm; return true; } return false; }
};
struct CBaseIntelligentPointer { CThing* PItem; CThing* GetPItem() const { return PItem; } operator CThing*() const { return GetPItem(); } };
struct CCharString { const char* t; CCharString(const char* s, long = -1) : t(s) {} };
struct CCreatureAction_BriarRoseIntoInvulnerable {
    CBaseIntelligentPointer PCreature, PEnemy; const char* anim; int baseInit;
    void SetAnimationName(const CCharString& n) { anim = n.t; }
    void Initialise()
    {
        { CCharString name("INVULNERABLE_INTO"); SetAnimationName(name); }
        ++baseInit;
        CTCCreatureModeManager* modeManager;
        if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31) && PEnemy.GetPItem() && !(PEnemy.GetPItem()->m_flags91 & 1))
            modeManager->AddMode(new CModeBriarRoseTeleportInvulnerable(PCreature, PEnemy.GetPItem()));
    }
};
int main()
{
    CThing o, e; o.has = true; o.m_flags91 = 0; o.mm.added = 0; e.has = false; e.m_flags91 = 0;
    CCreatureAction_BriarRoseIntoInvulnerable a; a.PCreature.PItem = &o; a.PEnemy.PItem = &e; a.baseInit = 0;
    a.Initialise();
    CModeBriarRoseTeleportInvulnerable* m = (CModeBriarRoseTeleportInvulnerable*)o.mm.added;
    bool ok = m && m->o == &o && m->e == &e && !std::strcmp(a.anim, "INVULNERABLE_INTO") && a.baseInit == 1;
    delete m;
    o.mm.added = 0; e.m_flags91 = 1; a.Initialise(); ok = ok && o.mm.added == 0;
    e.m_flags91 = 0; a.PEnemy.PItem = 0; a.Initialise(); ok = ok && o.mm.added == 0 && a.baseInit == 3;
    if (!ok) { std::printf("008c1e80_TEST FAIL\n"); return 1; }
    std::printf("008c1e80_TEST PASS\n");
    return 0;
}