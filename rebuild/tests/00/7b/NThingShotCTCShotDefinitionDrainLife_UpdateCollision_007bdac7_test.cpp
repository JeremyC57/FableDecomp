// CTCShotDefinitionDrainLife::UpdateCollision(bool* finished) -- base collision update; a drain-life shot that hit its (alive) target heals it by AmountToHeal and ends; a hit on anything else clears the collision type and keeps going; a dead target ends the shot. Ending kills the soul effect.
// Leaf: UpdateCollision
// Retail: Fable.exe 0x007BDAC7, 157 bytes (`ret 4`; size TU with frame: optimize("y", off) + ("s", on)).
// Callees: CTCShotDefinitionBase::UpdateCollision(bool*) @ 0x007BFE30, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// IS_THING_ALIVE<CThing*> @ 0x00436380 (target) and IS_THING_ALIVE<const CThing*> @ 0x0040DCE0 (hit; out-of-line `__fastcall`
// templates taking the pointer by reference; both labelled "<CThingBuilding*>" in the manifest),
// IS_THING_ALIVE<CIntelligentPointer<CThing>> @ 0x004CC340, CThing::Kill(bool) @ 0x004C9B80, the hit thing's virtual
// slot 59 (+0xEC, heal(float amount, bool)).
// Layout: retail CTCShotDefinitionBase sits 4 bytes below the PDB after HitThingsList: PTargetThing +0x20,
// PBestCollisionThing +0x88, BestCollisionType +0x90; retail CTCShotDefinitionDrainLife: AmountToHeal +0xA4,
// PSoulEffect +0xAC (PDB 0xA8 / 0xB0). CThing flag byte +0x6C bit 1 (can be healed).
// Shape: the target is an address-taken local (homed in the dead argument slot); the hit thing stays in edi and a
// named `const CThing*` copy (stored at [ebp-4] right after the fetch) is what IS_THING_ALIVE<const CThing*> gets;
// an if / else-if / else chain with the soul-kill + `*finished = true` tail written in both of the first two arms (VC
// cross-jumps the second copy back into the first, so the dead-target arm is laid first and the heal arm jumps back);
// `BestCollisionType = 0` is the size TU's `and dword ptr [m],0`.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void Heal(float amount, bool showEffect);  // slot 59 (+0xec)
    unsigned char _pad04[0x6c - 0x04];
    unsigned char m_flags6c;                            // +0x6c bit 1: can be healed
    void Kill(bool immediately);                        // 0x004C9B80
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                    // +4
    CThing* GetPItem() const;                           // 0x00A01B50
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);
template <class T> bool __fastcall IS_THING_ALIVE_00436380(const T& thing);   // the 0x00436380 copy (CThing*)
struct CTCShotDefinitionBase {
    void*                   __vftable;                  // +0x00
    CThing*                 ParentThing;                // +0x04
    unsigned char           _pad08[0x20 - 0x08];
    CBaseIntelligentPointer PTargetThing;               // +0x20
    unsigned char           _pad28[0x88 - 0x28];
    CBaseIntelligentPointer PBestCollisionThing;        // +0x88
    long                    BestCollisionType;          // +0x90
    unsigned char           _pad94[0x9c - 0x94];
    void UpdateCollision(bool* finished);               // 0x007BFE30
};
struct CTCShotDefinitionDrainLife : CTCShotDefinitionBase {
    float                   MaxCosAngleToTurn;          // +0x9c
    float                   MaxAngleToTurn;             // +0xa0
    float                   AmountToHeal;               // +0xa4
    float                   Speed;                      // +0xa8
    CBaseIntelligentPointer PSoulEffect;                // +0xac
    void UpdateCollision(bool* finished);
};

void CTCShotDefinitionDrainLife::UpdateCollision(bool* finished)
{
    CTCShotDefinitionBase::UpdateCollision(finished);
    CThing* target = PTargetThing.GetPItem();
    CThing* hit = PBestCollisionThing.GetPItem();
    const CThing* constHit = hit;
    if (!IS_THING_ALIVE_00436380<CThing*>(target)) {
        if (IS_THING_ALIVE<CBaseIntelligentPointer>(PSoulEffect))
            PSoulEffect.GetPItem()->Kill(true);
        *finished = true;
    } else if (hit == target) {
        if (IS_THING_ALIVE<const CThing*>(constHit) && (hit->m_flags6c & 2))
            hit->Heal(AmountToHeal, true);
        if (IS_THING_ALIVE<CBaseIntelligentPointer>(PSoulEffect))
            PSoulEffect.GetPItem()->Kill(true);
        *finished = true;
    } else {
        BestCollisionType = 0;
        *finished = false;
    }
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct TThing : CThing {
    int alive, healed, killed; float amount; bool show;
};
#define VDEF(n) void CThing::v##n() { }
VDEF(00) VDEF(01) VDEF(02) VDEF(03) VDEF(04) VDEF(05) VDEF(06) VDEF(07) VDEF(08) VDEF(09)
VDEF(10) VDEF(11) VDEF(12) VDEF(13) VDEF(14) VDEF(15) VDEF(16) VDEF(17) VDEF(18) VDEF(19)
VDEF(20) VDEF(21) VDEF(22) VDEF(23) VDEF(24) VDEF(25) VDEF(26) VDEF(27) VDEF(28) VDEF(29)
VDEF(30) VDEF(31) VDEF(32) VDEF(33) VDEF(34) VDEF(35) VDEF(36) VDEF(37) VDEF(38) VDEF(39)
VDEF(40) VDEF(41) VDEF(42) VDEF(43) VDEF(44) VDEF(45) VDEF(46) VDEF(47) VDEF(48) VDEF(49)
VDEF(50) VDEF(51) VDEF(52) VDEF(53) VDEF(54) VDEF(55) VDEF(56) VDEF(57) VDEF(58)
void CThing::Heal(float amount, bool showEffect) { TThing* t = (TThing*)this; ++t->healed; t->amount = amount; t->show = showEffect; }
void CThing::Kill(bool) { ++((TThing*)this)->killed; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static int g_aliveCalls = 0, g_baseCalls = 0;
static bool Alive(const CThing* t) { ++g_aliveCalls; return t && ((const TThing*)t)->alive; }
static bool Alive(const CBaseIntelligentPointer& p) { return p.m_pItem && ((TThing*)p.m_pItem)->alive; }
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing) { return Alive(thing); }
template <class T> bool __fastcall IS_THING_ALIVE_00436380(const T& thing) { return Alive(thing); }
void CTCShotDefinitionBase::UpdateCollision(bool* finished) { ++g_baseCalls; *finished = false; }

static TThing* mk() { TThing* t = new TThing; memset((char*)t + 4, 0, sizeof(TThing) - 4); return t; }

int main()
{
    static CTCShotDefinitionDrainLife shot;
    TThing* target = mk(); TThing* other = mk(); TThing* soul = mk();
    shot.AmountToHeal = 7.5f; shot.PSoulEffect.m_pItem = soul; soul->alive = 1;
    bool finished = false;
    // dead target: kill the soul effect, finished
    shot.PTargetThing.m_pItem = target; shot.PBestCollisionThing.m_pItem = other; target->alive = 0;
    shot.BestCollisionType = 5;
    shot.UpdateCollision(&finished);
    CHECK(finished && soul->killed == 1 && shot.BestCollisionType == 5 && g_baseCalls == 1);
    // alive target, hit something else: keep going
    target->alive = 1; other->alive = 1;
    shot.UpdateCollision(&finished);
    CHECK(!finished && shot.BestCollisionType == 0 && soul->killed == 1 && other->healed == 0);
    // alive target hit, healable
    shot.PBestCollisionThing.m_pItem = target; target->m_flags6c = 2;
    shot.UpdateCollision(&finished);
    CHECK(finished && target->healed == 1 && target->amount == 7.5f && target->show && soul->killed == 2);
    // not healable flag
    target->m_flags6c = 0xfd;
    shot.UpdateCollision(&finished);
    CHECK(finished && target->healed == 1 && soul->killed == 3);
    // soul effect dead: no kill
    soul->alive = 0; target->m_flags6c = 2;
    shot.UpdateCollision(&finished);
    CHECK(finished && target->healed == 2 && soul->killed == 3);
    // null target
    shot.PTargetThing.m_pItem = 0; soul->alive = 1;
    shot.UpdateCollision(&finished);
    CHECK(finished && soul->killed == 4 && target->healed == 2);
    if (fails == 0) printf("007bdac7_TEST PASS\n");
    return fails;
}