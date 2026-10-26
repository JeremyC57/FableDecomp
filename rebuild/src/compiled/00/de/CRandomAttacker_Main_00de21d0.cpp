// CRandomAttacker::Main() -- the ransom-victim quest's random attacker: register an alive-filter over Me, wait until the hero is within a random 8..19 units, then (when "Victim" is alive, at even odds, and Me's data string is not "HeroOnly") turn on the victim, else on the hero; wait until Me is hit by the hero (plainly, or by any of his special abilities other than ability 14), then clear the target and turn on the hero
// Leaf: Main
// Label correction: the catalog calls this CRansomRevenge::Main; the factory 0x00DE2100 that stores vtable 0x012DD238
// is registered as "RandomAttacker" (0x00DE0AF4), PDB ?Main@CRandomAttacker@CQ_RansomVictimScript@NScript@@EAEXXZ.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE21D0, length 809 bytes, plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0, inline dtor) over the CScriptThing copy ctor 0x004ABE90 /
// ~CScriptThing 0x004AA840, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30;
// IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall); rand 0x00BFEB16; CCharString 0x0099EBF0 / 0x0099EAE0 and
// operator!=(const char*) 0x0099E960; CScriptThing slots 3 GetDataString, 21 MsgIsHitBy, 41
// MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom, 75 IsAlive; CGameScriptInterface slots 7
// NewScriptFrame, 70 GetHero, 72 GetThingWithScriptName, 451 GiveThingBestEnemyTarget, 452 ClearThingBestEnemyTarget.
// Literals "Victim" 0x012CA958, "HeroOnly" 0x012DD254, "SCRIPT_NAME_HERO" 0x0125D1C8.
// Layout: Ego_r.pdb CRandomAttacker (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
// PMasterData +0x18 (neither used here).
// Shape: the two "turn on the hero" arms are identical and VC7.1 cross-jumps them; the conditional string temporaries
// of the `&&`/`||` chains carry ebx flag bits (1, 2, 4, 8) and their results land in one byte slot.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator!=(const char* text) const;           // 0x0099E960
};
enum EHeroAbility { HERO_ABILITY_14 = 14 };
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void t00(); virtual void t01(); virtual void t02();
    virtual CCharString GetDataString() const;         // slot 3 (+0x0c)
    virtual void t04(); virtual void t05(); virtual void t06(); virtual void t07(); virtual void t08(); virtual void t09();
    virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15();
    virtual void t16(); virtual void t17(); virtual void t18(); virtual void t19(); virtual void t20();
    virtual bool MsgIsHitBy(const CCharString& name) const;                                   // slot 21 (+0x54)
    virtual void t22(); virtual void t23(); virtual void t24(); virtual void t25(); virtual void t26(); virtual void t27();
    virtual void t28(); virtual void t29(); virtual void t30(); virtual void t31(); virtual void t32(); virtual void t33();
    virtual void t34(); virtual void t35(); virtual void t36(); virtual void t37(); virtual void t38(); virtual void t39();
    virtual void t40();
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;                     // slot 42 (+0xa8)
    virtual void t43(); virtual void t44(); virtual void t45(); virtual void t46(); virtual void t47(); virtual void t48();
    virtual void t49(); virtual void t50(); virtual void t51(); virtual void t52(); virtual void t53(); virtual void t54();
    virtual void t55(); virtual void t56(); virtual void t57(); virtual void t58(); virtual void t59(); virtual void t60();
    virtual void t61(); virtual void t62(); virtual void t63(); virtual void t64(); virtual void t65(); virtual void t66();
    virtual void t67(); virtual void t68(); virtual void t69(); virtual void t70(); virtual void t71(); virtual void t72();
    virtual void t73(); virtual void t74();
    virtual bool IsAlive() const;                      // slot 75 (+0x12c)
    void* PImp[2];
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
};
struct CIsThingAlive {                                 // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern "C" int __cdecl rand();                         // 0x00BFEB16
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CGameScriptInterface { public:
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD2(qe)                              // slots 8..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD1(ra)                                                                        // slot 71
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;      // slot 72 (+0x120)
    VPAD256(sa) VPAD64(sb) VPAD32(sc) VPAD16(sd) VPAD8(se) VPAD2(sf)                 // slots 73..450
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const;   // slot 451 (+0x70c)
    virtual void ClearThingBestEnemyTarget(const CScriptThing& thing) const;        // slot 452 (+0x710)
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void RegisterFilter(CIsThingAlive& filter);        // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CRandomAttacker : CEntityScriptBase {
    void* ParentClass;                                 // +0x14 CQ_RansomVictimScript*
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};

void CRandomAttacker::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    float distance = (float)(rand() % 12 + 8);
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), distance)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (Game->GetThingWithScriptName("Victim").IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        if (rand() % 2 == 0 && Me.GetDataString() != "HeroOnly") {
            if (IsActiveThreadTerminating())
                return;
            Game->GiveThingBestEnemyTarget(Me, Game->GetThingWithScriptName("Victim"));
        } else {
            if (IsActiveThreadTerminating())
                return;
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    }
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_14, "SCRIPT_NAME_HERO")))
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->ClearThingBestEnemyTarget(Me);
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    if (IsActiveThreadTerminating())
        return;
}