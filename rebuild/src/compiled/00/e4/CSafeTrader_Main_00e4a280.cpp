// CSafeTrader::Main() -- the bandit-toll quest's safe trader: register an alive-filter over Me, take Me under scripted control, then until Me reaches the "SafeTraderEscape" marker keep walking there, warn the hero once when he comes near (a hostile line if he already attacked), and when the hero hits Me turn scared and run the precalculated route; finally fade out and kill Me
// Leaf: ?Main@CSafeTrader@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4A280, length 992 bytes, plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0, out-of-line dtor 0x00E241B0) over the CScriptThing copy ctor
// 0x004ABE90 / ~CScriptThing 0x004AA840, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30;
// scripted-thing handle (vtable 0x0127094C) inline ctor over CBaseObject 0x0099A380, IsScripting 0x00CD23B9, Release
// 0x00CD2770, MoveToThing 0x007E7300, FollowPreCalculatedRoute 0x007E7310, IsPerformingScriptTask 0x007E7450, dtor
// 0x007E74D0; IsDistanceBetweenThingsOver 0x00CBE3EA / IsDistanceBetweenThingsUnder 0x00CBE2FF / FadeOutAndKillEntity
// 0x00CBE716 (all __fastcall); CCharString 0x0099EBF0 / 0x0099EAE0; CScriptThing slots 21 MsgIsHitBy, 41
// MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom; CGameScriptInterface slots 7 NewScriptFrame, 8
// StartScriptingEntity, 70 GetHero, 72 GetThingWithScriptName, 364 AddNewConversation, 365 AddPersonToConversation,
// 366 AddLineToConversation, 496 EntitySetAsScared. pGlobalScriptDef 0x0143E90C (+0x42c long warning distance, +0x43c
// float escape distance, +0x440 float fade time; the retail CScriptDef differs from the Ego_r.pdb one here).
// Literals "SafeTraderEscape" 0x012E4F80, "TEXT_QST_011_SAFE_TRADER_WARNING_10" 0x012E4F5C,
// "TEXT_QST_011_SAFE_TRADER_WARNING_10_HATE" 0x012E4F30, "SCRIPT_NAME_HERO" 0x0125D1C8.
// Layout: Ego_r.pdb CSafeTrader (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
// (CV_BanditTollScript*, byte +0x4d set on the warning), PMasterData +0x18, WarnedHero +0x1c, AttackedByHero +0x1d.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
enum EHeroAbility { HERO_ABILITY_14 = 14 };
struct CScriptThing {                                  // 12 bytes, vtable 0x01238C8C
    virtual void t00(); virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05();
    virtual void t06(); virtual void t07(); virtual void t08(); virtual void t09();
    virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15();
    virtual void t16(); virtual void t17(); virtual void t18(); virtual void t19(); virtual void t20();
    virtual bool MsgIsHitBy(const CCharString& name) const;                                   // slot 21 (+0x54)
    virtual void t22(); virtual void t23(); virtual void t24(); virtual void t25(); virtual void t26(); virtual void t27();
    virtual void t28(); virtual void t29(); virtual void t30(); virtual void t31(); virtual void t32(); virtual void t33();
    virtual void t34(); virtual void t35(); virtual void t36(); virtual void t37(); virtual void t38(); virtual void t39();
    virtual void t40();
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;                     // slot 42 (+0xa8)
    void* PImp[2];
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
};
struct CIsThingAlive {                                 // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                                  // 0x00E241B0
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_0 };
struct CCountedPointerWords { void* Data; void* Info; };
struct CBaseObject {
    void* m_pIntelligentPointer;
    CBaseObject();                                     // 0x0099A380
    virtual ~CBaseObject();
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C
    CCountedPointerWords PImp;
    CScriptGameResourceObjectScriptedThingBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    void MoveToThing(const CScriptThing& thing, float distance, EScriptEntityMoveType moveType, bool a, bool b, bool c, bool d);   // 0x007E7300
    void FollowPreCalculatedRoute(const CScriptThing& thing, EScriptEntityMoveType moveType, bool a, bool b);   // 0x007E7310
    bool IsPerformingScriptTask() const;               // 0x007E7450
};
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool kill, float seconds, bool removeOnFade);  // 0x00CBE716
struct CScriptDef {
    unsigned char _pad0[0x42c];
    long  SafeTraderWarningDistance;                   // +0x42C
    unsigned char _pad1[0x43c - 0x430];
    float SafeTraderEscapeDistance;                    // +0x43C
    float SafeTraderFadeTime;                          // +0x440
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
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
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD1(qe)                              // slots 9..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD1(ra)                                                                        // slot 71
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;      // slot 72 (+0x120)
    VPAD256(sa) VPAD32(sb) VPAD2(sc) VPAD1(sd)                                       // slots 73..363
    virtual long AddNewConversation(const CScriptThing& speaker, bool a, bool b) const;   // slot 364 (+0x5b0)
    virtual void AddPersonToConversation(long conversation, const CScriptThing& person) const;   // slot 365 (+0x5b4)
    virtual void AddLineToConversation(long conversation, const CCharString& text, bool a, const CScriptThing& speaker, const CScriptThing& listener) const;   // slot 366 (+0x5b8)
    VPAD128(ta) VPAD1(tb)                                                            // slots 367..495
    virtual void EntitySetAsScared(const CScriptThing& thing, bool scared) const;    // slot 496 (+0x7c0)
};
struct CV_BanditTollScript {
    unsigned char _pad[0x4d];
    bool SafeTraderWarnedHero;                         // +0x4D
};
struct CEntityScriptBase {
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void RegisterFilter(CIsThingAlive& filter);        // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CSafeTrader : CEntityScriptBase {
    CV_BanditTollScript* ParentClass;                  // +0x14
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
    bool  WarnedHero;                                  // +0x1C
    bool  AttackedByHero;                              // +0x1D
    void Main();
};

void CSafeTrader::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsScripting())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing escape = Game->GetThingWithScriptName("SafeTraderEscape");
    long warningDistance = pGlobalScriptDef->SafeTraderWarningDistance;
    while (IsDistanceBetweenThingsOver(Me, escape, pGlobalScriptDef->SafeTraderEscapeDistance)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!handle.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating())
                return;
            handle.MoveToThing(escape, 1.0f, (EScriptEntityMoveType)0, false, true, false, true);
        }
        if (!WarnedHero && IsDistanceBetweenThingsUnder(Game->GetHero(), Me, (float)warningDistance)) {
            if (IsActiveThreadTerminating())
                return;
            WarnedHero = true;
            ParentClass->SafeTraderWarnedHero = true;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            if (!AttackedByHero) {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(conversation, "TEXT_QST_011_SAFE_TRADER_WARNING_10", false, Me, Game->GetHero());
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(conversation, "TEXT_QST_011_SAFE_TRADER_WARNING_10_HATE", false, Me, Game->GetHero());
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            AttackedByHero = true;
            Game->EntitySetAsScared(Me, true);
            handle.FollowPreCalculatedRoute(escape, (EScriptEntityMoveType)1, false, true);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    FadeOutAndKillEntity(Me, true, pGlobalScriptDef->SafeTraderFadeTime, true);
}