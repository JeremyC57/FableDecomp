// CSUMMONED_CREATURE::Main() -- an arena summoned creature: register a consciousness filter over Me, count it into the arena's creature totals, then every frame score the crowd when the hero hits it (a flourish +5 or a decapitation +7, each also playing the nearest "ArenaSpawn" crowd-loop sound and restarting the crowd timer when that timer allows; any other hit by the hero, or a special ability other than ability 14, +2)
// Timer shape (advisor): the timer goes through inline CTimer accessors: `GetTime()` reads the handle through a
// `const long&` (retail's `add eax,100h ; mov eax,[eax]` before the GetTimer push) and `SetTime()` stores it directly. That
// reference temp is what tips VC7.1's allocator: the conditional-temporary flags word stays in its frame home (immediate
// zero-store at entry, re-read before the hit chain), `bl` goes to the flourish/decapitate results, and ebx/ebp shrink-wrap
// after the first two exits. With a bare `GetTimer(ParentClass->GlobalCrowdTimer.Handle)` the flags take ebx from entry
// (DIFFER 921v907).
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F1B220, length 907 bytes, plain `ret`.
// Callees: CIsThingConscious filter (vtable 0x012C2FE8) over the CScriptThing copy ctor 0x004ABE90, its out-of-line dtor
// 0x00EF7AB0, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30; CCharString 0x0099EBF0 / 0x0099EAE0;
// ~CScriptThing 0x004AA840; CScriptThing slots 21 MsgIsHitBy, 22 MsgIsHitByWithFlourish, 23 MsgIsHitByWithDecapitate,
// 41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom; CGameScriptInterface slots 7 NewScriptFrame,
// 70 GetHero, 77 GetNearestWithScriptName, 89 SetTimer, 90 GetTimer, 689 PlayCriteriaSoundOnThing (the timer pair through
// g_pGameScriptInterface 0x0143E8F8). Literals "SCRIPT_NAME_HERO" 0x0125D1C8, "ArenaSpawn" 0x01259BC4.
// Layout: Ego_r.pdb CSUMMONED_CREATURE (28 bytes, the CQ_ArenaScript one): CEntityScriptBase {vptr, Game +4, Me +8},
// ParentClass +0x14, PMasterData +0x18. CQ_ArenaScript is retail = PDB - 0x14 before Rounds (12-byte STLport vector) and
// - 0x18 after it: CrowdLoopTags +0x48, TotalCreatures +0xd0, ExtraCreatures +0xdc, NewCrowdPoints +0xf4,
// NewCrowdBaseLevel +0xfc, GlobalCrowdTimer +0x100.
// Shape: the sibling CRansomRevenge::Main 0x00DE3830 hit chain (conditional string temporaries with flag bits 1, 2, 4,
// here spilled to the frame); the watch loop is a rotated `while (!IsActiveThreadTerminating())`.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator!=(const char* text) const;           // 0x0099E960
};
enum EHeroAbility { HERO_ABILITY_14 = 14 };
struct CScriptThing {                                  // 12 bytes, vtable 0x01238C8C
    virtual void t00(); virtual void t01(); virtual void t02();
    virtual CCharString GetDataString() const;         // slot 3 (+0x0c)
    virtual void t04(); virtual void t05(); virtual void t06(); virtual void t07(); virtual void t08(); virtual void t09();
    virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15();
    virtual void t16(); virtual void t17(); virtual void t18(); virtual void t19(); virtual void t20();
    virtual bool MsgIsHitBy(const CCharString& name) const;                                   // slot 21 (+0x54)
    virtual bool MsgIsHitByWithFlourish(const CCharString& name) const;                       // slot 22 (+0x58)
    virtual bool MsgIsHitByWithDecapitate(const CCharString& name) const;                     // slot 23 (+0x5c)
    virtual void t24(); virtual void t25(); virtual void t26(); virtual void t27();
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
struct CIsThingConscious {                             // vtable 0x012C2FE8: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingConscious(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingConscious();                              // 0x00EF7AB0
};
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct CGameScriptInterface {
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD2(qe)                              // slots 8..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD4(ra) VPAD2(rb)                                                              // slots 71..76
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing& from, const CCharString& name) const;   // slot 77 (+0x134)
    VPAD8(sa) VPAD2(sb) VPAD1(sc)                                                    // slots 78..88
    virtual void SetTimer(long timer, long time);                                    // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;                                         // slot 90 (+0x168)
    VPAD256(ta) VPAD256(tb) VPAD64(tc) VPAD16(td) VPAD4(te) VPAD2(tf)             // slots 91..688
    virtual unsigned long PlayCriteriaSoundOnThing(const CScriptThing& thing, const CCharString& criteria) const;   // slot 689 (+0xac4)
};
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
struct CTimer {                                        // a script timer: the interface's timer handle
    long Handle;
    long GetTime() const { const long& handle = Handle; return g_pGameScriptInterface->GetTimer(handle); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Handle, time); }
};
struct CQ_ArenaScript {                                // retail = Ego_r.pdb - 0x14 up to Rounds, - 0x18 after it
    unsigned char _pad00[0x48];
    CCharString   CrowdLoopTags[4][5];                 // +0x48
    unsigned char _pad98[0xd0 - 0x98];
    long          TotalCreatures[3];                   // +0xd0
    long          ExtraCreatures;                      // +0xdc
    bool          MissionSucceeded;                    // +0xe0
    bool          MissionFailed;                       // +0xe1
    bool          ArenaSpawnNeeded[16];                // +0xe2
    long          NewCrowdPoints;                      // +0xf4
    long          NewCrowdHappiness;                   // +0xf8
    long          NewCrowdBaseLevel;                   // +0xfc
    CTimer        GlobalCrowdTimer;                    // +0x100
};
struct CEntityScriptBase {
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void RegisterFilter(CIsThingConscious& filter);    // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CSUMMONED_CREATURE : CEntityScriptBase {
    CQ_ArenaScript* ParentClass;                       // +0x14
    void*           PMasterData;                       // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};

void CSUMMONED_CREATURE::Main()
{
    {
        CIsThingConscious filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    ++ParentClass->TotalCreatures[0];
    ++ParentClass->ExtraCreatures;
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsHitByWithFlourish("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->GlobalCrowdTimer.GetTime() <= 0) {
                if (IsActiveThreadTerminating())
                    return;
                Game->PlayCriteriaSoundOnThing(Game->GetNearestWithScriptName(Game->GetHero(), "ArenaSpawn"),
                                               ParentClass->CrowdLoopTags[ParentClass->NewCrowdBaseLevel][3]);
                ParentClass->GlobalCrowdTimer.SetTime(10);
                ParentClass->NewCrowdPoints += 5;
            }
        } else if (Me.MsgIsHitByWithDecapitate("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->GlobalCrowdTimer.GetTime() <= 8) {
                if (IsActiveThreadTerminating())
                    return;
                Game->PlayCriteriaSoundOnThing(Game->GetNearestWithScriptName(Game->GetHero(), "ArenaSpawn"),
                                               ParentClass->CrowdLoopTags[ParentClass->NewCrowdBaseLevel][3]);
                ParentClass->GlobalCrowdTimer.SetTime(10);
                ParentClass->NewCrowdPoints += 7;
            }
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                   || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                       && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->NewCrowdPoints += 2;
        }
        Game->NewScriptFrame();
    }
}