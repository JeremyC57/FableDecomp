// behaviour test for 0x00752480 CTCLook::IsAbleToTurnViaAnimation (body verbatim, callees stubbed)
#include <stdio.h>
#include <string.h>
// CTCLook::IsAbleToTurnViaAnimation() -- turning via animation is allowed when the def allows it (combat or normal flag by the creature's combat state), no current action forbids it, turning animations exist, turning via animation is not disabled and the thing can use turning animations
// Grade: RELOCATION_MATCH -- IS_ALIVE as a template taking `T* const&` bound straight to the PParentCreature member: retail then loads it into eax for the null/dead tests and copies it to ecx for the call (`mov ecx,eax`, twice); a by-value pointer parameter loads straight into ecx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00752480, length 137 bytes.
// Callees: CThingCreatureBase combat-state query @ 0x00661E80 (bits 2/3 of the byte at +0x15C; catalog "CText::GetAlignment" is a
// byte twin), CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0, the current action's (+0x100) virtual slot 50 (+0xC8),
// IsAbleToUseTurningAnimations(CThing*) @ 0x0085FCD0 (__fastcall).
// Layout (retail, local model): ParentThing +0x04, PParentCreature +0x10, PDef +0x14 (CLookDef AllowTurningViaAnimation +0x40,
// CombatAllowTurningViaAnimation +0x4C), TurningViaAnimationDisable +0x41, TurningAnims vector begin/end +0x8C/+0x90 (PDB offsets);
// the creature's dead bit is CThing +0x91 bit 0.
// Shape: the def flag is a conditional read into al; IS_ALIVE inlined twice; nested ifs with one shared `xor al,al` exit.
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                         // +0x91 bit 0: dead
};
struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49();
    virtual bool PreventsTurningViaAnimation();      // slot 50 (+0xc8)
};
struct CThingCreatureBase : CThing {
    unsigned char _pad92[0x100 - 0x92];
    CCreatureActionBase* PCurrentAction;             // +0x100
    bool IsInCombatMode() const;                     // 0x00661E80
    bool IsPerformingAnyAction() const;              // 0x006622A0
};
template<class T> inline bool IS_ALIVE(T* const& p)
{
    return p != 0 && !(p->m_flags91 & 1);
}
bool __fastcall IsAbleToUseTurningAnimations(CThing* thing);   // 0x0085FCD0
struct CLookDef {
    unsigned char _pad00[0x40];
    bool  AllowTurningViaAnimation;                  // +0x40
    unsigned char _pad41[0x4c - 0x41];
    bool  CombatAllowTurningViaAnimation;            // +0x4c
};
template<class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CAnimationEntry;
struct CTurningAnim { float Angle; const CAnimationEntry* Anim; };
struct CTCLook {
    virtual void v00();
    CThing* ParentThing;                             // +0x04 (CTCBase)
    unsigned char _base08[0x10 - 0x08];
    CThingCreatureBase* PParentCreature;             // +0x10
    CDefPointer<const CLookDef> PDef;                // +0x14
    unsigned char _pad18[0x41 - 0x18];
    bool  TurningViaAnimationDisable;                // +0x41
    unsigned char _pad42[0x8c - 0x42];
    CTurningAnim* TurningAnimsStart;                 // +0x8c TurningAnims (retail 12-byte vector)
    CTurningAnim* TurningAnimsFinish;                // +0x90
    CTurningAnim* TurningAnimsEnd;                   // +0x94
    bool IsAbleToTurnViaAnimation();
};

bool CTCLook::IsAbleToTurnViaAnimation()
{
    bool allowed;
    if (IS_ALIVE(PParentCreature) && PParentCreature->IsInCombatMode())
        allowed = PDef->CombatAllowTurningViaAnimation;
    else
        allowed = PDef->AllowTurningViaAnimation;
    if (allowed) {
        if (!(IS_ALIVE(PParentCreature) && PParentCreature->IsPerformingAnyAction() && PParentCreature->PCurrentAction->PreventsTurningViaAnimation())) {
            if (TurningAnimsStart != TurningAnimsFinish && !TurningViaAnimationDisable) {
                if (IsAbleToUseTurningAnimations(ParentThing))
                    return true;
            }
        }
    }
    return false;
}


static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static bool combat, performing, prevents, canUse;
static int combatCalls, perfCalls, prevCalls, useCalls;
static CThing* useArg;
bool CThingCreatureBase::IsInCombatMode() const { ++combatCalls; return combat; }
bool CThingCreatureBase::IsPerformingAnyAction() const { ++perfCalls; return performing; }
void CCreatureActionBase::v00() {}
void CCreatureActionBase::v01() {}
void CCreatureActionBase::v02() {}
void CCreatureActionBase::v03() {}
void CCreatureActionBase::v04() {}
void CCreatureActionBase::v05() {}
void CCreatureActionBase::v06() {}
void CCreatureActionBase::v07() {}
void CCreatureActionBase::v08() {}
void CCreatureActionBase::v09() {}
void CCreatureActionBase::v10() {}
void CCreatureActionBase::v11() {}
void CCreatureActionBase::v12() {}
void CCreatureActionBase::v13() {}
void CCreatureActionBase::v14() {}
void CCreatureActionBase::v15() {}
void CCreatureActionBase::v16() {}
void CCreatureActionBase::v17() {}
void CCreatureActionBase::v18() {}
void CCreatureActionBase::v19() {}
void CCreatureActionBase::v20() {}
void CCreatureActionBase::v21() {}
void CCreatureActionBase::v22() {}
void CCreatureActionBase::v23() {}
void CCreatureActionBase::v24() {}
void CCreatureActionBase::v25() {}
void CCreatureActionBase::v26() {}
void CCreatureActionBase::v27() {}
void CCreatureActionBase::v28() {}
void CCreatureActionBase::v29() {}
void CCreatureActionBase::v30() {}
void CCreatureActionBase::v31() {}
void CCreatureActionBase::v32() {}
void CCreatureActionBase::v33() {}
void CCreatureActionBase::v34() {}
void CCreatureActionBase::v35() {}
void CCreatureActionBase::v36() {}
void CCreatureActionBase::v37() {}
void CCreatureActionBase::v38() {}
void CCreatureActionBase::v39() {}
void CCreatureActionBase::v40() {}
void CCreatureActionBase::v41() {}
void CCreatureActionBase::v42() {}
void CCreatureActionBase::v43() {}
void CCreatureActionBase::v44() {}
void CCreatureActionBase::v45() {}
void CCreatureActionBase::v46() {}
void CCreatureActionBase::v47() {}
void CCreatureActionBase::v48() {}
void CCreatureActionBase::v49() {}
bool CCreatureActionBase::PreventsTurningViaAnimation() { return false; }
void CTCLook::v00() {}
struct CAction : CCreatureActionBase {
    void v00() {} void v01() {} void v02() {} void v03() {} void v04() {} void v05() {}
    void v06() {} void v07() {} void v08() {} void v09() {} void v10() {} void v11() {}
    void v12() {} void v13() {} void v14() {} void v15() {} void v16() {} void v17() {}
    void v18() {} void v19() {} void v20() {} void v21() {} void v22() {} void v23() {}
    void v24() {} void v25() {} void v26() {} void v27() {} void v28() {} void v29() {}
    void v30() {} void v31() {} void v32() {} void v33() {} void v34() {} void v35() {}
    void v36() {} void v37() {} void v38() {} void v39() {} void v40() {} void v41() {}
    void v42() {} void v43() {} void v44() {} void v45() {} void v46() {} void v47() {}
    void v48() {} void v49() {}
    bool PreventsTurningViaAnimation() { ++prevCalls; return prevents; }
};
bool __fastcall IsAbleToUseTurningAnimations(CThing* thing) { ++useCalls; useArg = thing; return canUse; }
struct CLook : CTCLook { void v00() {} };

static CThingCreatureBase creature;
static CAction action;
static CThing parent;
static CLookDef def;
static CTurningAnim anims[2];
static CLook look;

static void Reset()
{
    combat = performing = prevents = false; canUse = true;
    combatCalls = perfCalls = prevCalls = useCalls = 0; useArg = 0;
    memset(&creature, 0, sizeof creature);
    creature.PCurrentAction = &action;
    memset(&def, 0, sizeof def);
    def.AllowTurningViaAnimation = true;
    def.CombatAllowTurningViaAnimation = true;
    look.ParentThing = &parent;
    look.PParentCreature = &creature;
    look.PDef.Object = &def;
    look.TurningViaAnimationDisable = false;
    look.TurningAnimsStart = &anims[0];
    look.TurningAnimsFinish = &anims[1];
}

int main()
{
    Reset(); CHECK(look.IsAbleToTurnViaAnimation() == true);
    CHECK(combatCalls == 1 && perfCalls == 1 && prevCalls == 0 && useCalls == 1 && useArg == &parent);
    Reset(); def.AllowTurningViaAnimation = false; CHECK(look.IsAbleToTurnViaAnimation() == false);
    CHECK(perfCalls == 0 && useCalls == 0);
    Reset(); combat = true; def.AllowTurningViaAnimation = false; CHECK(look.IsAbleToTurnViaAnimation() == true);
    Reset(); combat = true; def.CombatAllowTurningViaAnimation = false; CHECK(look.IsAbleToTurnViaAnimation() == false);
    Reset(); creature.m_flags91 = 1; combat = true; def.AllowTurningViaAnimation = true; def.CombatAllowTurningViaAnimation = false;
    CHECK(look.IsAbleToTurnViaAnimation() == true); CHECK(combatCalls == 0 && perfCalls == 0 && useCalls == 1);
    Reset(); look.PParentCreature = 0; CHECK(look.IsAbleToTurnViaAnimation() == true); CHECK(combatCalls == 0 && perfCalls == 0);
    Reset(); performing = true; prevents = true; CHECK(look.IsAbleToTurnViaAnimation() == false);
    CHECK(prevCalls == 1 && useCalls == 0);
    Reset(); performing = true; prevents = false; CHECK(look.IsAbleToTurnViaAnimation() == true); CHECK(prevCalls == 1 && useCalls == 1);
    Reset(); look.TurningAnimsFinish = look.TurningAnimsStart; CHECK(look.IsAbleToTurnViaAnimation() == false); CHECK(useCalls == 0);
    Reset(); look.TurningViaAnimationDisable = true; CHECK(look.IsAbleToTurnViaAnimation() == false); CHECK(useCalls == 0);
    Reset(); canUse = false; CHECK(look.IsAbleToTurnViaAnimation() == false); CHECK(useCalls == 1);
    if (fails == 0) printf("00752480_TEST PASS\n");
    return fails;
}