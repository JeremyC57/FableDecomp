// CEndGameThunder::Main() -- end-game Thunder: make Me undamageable, unkillable, a villager and off the combo count; unless the fight is already over, walk Me up to the hero, open a conversation (married / not married to Lady Grey line, then two more lines); then until the fight is over, ally Me with the hero whenever the hero hits Me, and whenever the hero strays beyond 8 units walk Me back to within 4
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D2C810, 1901 bytes, plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CBaseIntelligentPointer ctor 0x0099A380 / dtor 0x0099A430, scripted-thing handle IsScripting 0x00CD23B9 / Release
//   0x00CD2770 / MoveTo 0x007E72F0 / IsPerformingScriptTask 0x007E7450 / out-of-line dtor 0x007E74D0, IsActiveThreadTerminating
//   0x00F35B30, IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall), operator delete 0x00BFE9BC, CCharString 0x0099EBF0 /
//   0x0099EAE0; GameCalls slots 7, 8, 70, 364-366 conversation, 516 damageable, 517 killable, 526 combo, 599 ally, 601 faction;
//   CScriptThing slots 6 GetPos, 21 MsgIsHitBy, 41/42 special-ability hits. Literals "FACTION_VILLAGERS" 0x0125EAB4,
//   "SCRIPT_NAME_HERO" 0x0125D1C8, TEXT_QST_001_THUNDER_* 0x012CC400 / 0x012CC3D4 / 0x012CC3A4 / 0x012CC374.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (fight-over flag +0x81), PMasterData +0x18 (two
//   married flags +0x74 / +0xD7; +0xD7 is HeroMarriedLadyGrey as in CDF_MarryLadyGrey 0x00E73410); flag names descriptive.
// Shape: no alive filter; the CMeleeThunder handle model with the implicit handle dtor expanded inline only at the return
//   in the second StartScriptingEntity retry loop (every other exit is under inline_depth(0)); the closing
//   `if (IsActiveThreadTerminating()) return;` keeps the call with its test folded away.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
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
struct CScriptThing : CBase {                           // vtable 0x01238C8C, 12 bytes
    VPAD4(ma) VPAD1(mb)                                 // slots 1..5
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
    VPAD8(mh) VPAD4(mi) VPAD2(mj)                       // slots 7..20
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    VPAD4(mc) VPAD1(mg)                                 // slots 22..26
    virtual bool MsgIsTalkedToBy(const CCharString& name) const;   // slot 27 (+0x6c)
    VPAD8(md) VPAD4(me) VPAD1(mf)                       // slots 28..40
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing();                                    // 0x004AA840
};
struct CIsThingAlive {                                  // vtable 0x012C32F0
    virtual void v00();
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
namespace N_0099a380 { struct CBaseIntelligentPointer {                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
    CBaseIntelligentPointer();                          // 0x0099A380
    virtual ~CBaseIntelligentPointer();                 // 0x0099A430
}; } using N_0099a380::CBaseIntelligentPointer;
struct CScriptGameResourceObjectBase : CBaseIntelligentPointer {   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    // implicit destructor (stores no vptr); out-of-line copy 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    void MoveTo(const C3DVector& pos, float speed, bool a, bool b, bool c);   // 0x007E72F0
    bool IsPerformingScriptTask() const;                // 0x007E7450
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CGameScriptInterface {
    VPAD4(p0_0) VPAD2(p0_1) VPAD1(p0_2)                                          // slots 0..6
    virtual void NewScriptFrame() const;                                         // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority); // slot 8 (+0x20)
    VPAD32(p9_0) VPAD16(p9_1) VPAD8(p9_2) VPAD4(p9_3) VPAD1(p9_4)                // slots 9..69
    virtual CScriptThing& GetHero();                                             // slot 70 (+0x118)
    VPAD256(p71_0) VPAD32(p71_1) VPAD4(p71_2) VPAD1(p71_3)                       // slots 71..363
    virtual long AddNewConversation(const CScriptThing& thing, bool a, bool b) const; // slot 364 (+0x5b0)
    virtual void AddPersonToConversation(long conversation, const CScriptThing& thing) const; // slot 365 (+0x5b4)
    virtual void AddLineToConversation(long conversation, const CCharString& text, bool a, const CScriptThing& speaker, const CScriptThing& listener) const; // slot 366 (+0x5b8)
    VPAD128(p367_0) VPAD16(p367_1) VPAD4(p367_2) VPAD1(p367_3)                   // slots 367..515
    virtual void EntitySetAsDamageable(const CScriptThing& thing, bool damageable) const; // slot 516 (+0x810)
    virtual void EntitySetAsKillable(const CScriptThing& thing, bool killable, bool b) const; // slot 517 (+0x814)
    VPAD8(p518_0)                                                                // slots 518..525
    virtual void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& thing, bool add) const; // slot 526 (+0x838)
    VPAD64(p527_0) VPAD8(p527_1)                                                 // slots 527..598
    virtual void EntitySetThingAsAllyOfThing(const CScriptThing& thing, const CScriptThing& other) const; // slot 599 (+0x95c)
    VPAD1(p600_0)                                                                // slots 600..600
    virtual void EntitySetFaction(const CScriptThing& thing, const CCharString& faction) const; // slot 601 (+0x964)
};
struct CThunderScript {
    unsigned char _pad[0x81];
    bool ThunderFightOver;                             // +0x81 (descriptive)
};
struct CMasterData {
    unsigned char _pad[0x74];
    bool HeroMarriedFlag74;                            // +0x74 (descriptive)
    unsigned char _pad75[0xd7 - 0x75];
    bool HeroMarriedLadyGrey;                          // +0xD7
};
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE3EA
struct CEntityScriptBase {
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CEndGameThunder : CEntityScriptBase {
    CThunderScript* ParentClass;                        // +0x14
    CMasterData*    PMasterData;                        // +0x18
    void Main();
};

void CEndGameThunder::Main()
{
    CScriptGameResourceObjectScriptedThingBase face;
    Game->EntitySetAsDamageable(Me, false);
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetFaction(Me, "FACTION_VILLAGERS");
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    if (!ParentClass->ThunderFightOver) {
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        if (face.IsScripting())
            face.Release();
        while (!Game->StartScriptingEntity(Me, face, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
        }
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        face.MoveTo(Game->GetHero().GetPos(), 2.0f, true, false, true);
        while (IsDistanceBetweenThingsOver(Game->GetHero(), Me, 4.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            if (!face.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                face.MoveTo(Game->GetHero().GetPos(), 2.0f, true, false, true);
            }
        }
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        if (!PMasterData->HeroMarriedFlag74 && !PMasterData->HeroMarriedLadyGrey) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            Game->AddLineToConversation(conversation, "TEXT_QST_001_THUNDER_MARRIED_LADY_10", false, Me, Game->GetHero());
        } else {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            Game->AddLineToConversation(conversation, "TEXT_QST_001_THUNDER_NOT_MARRIED_LADY_10", false, Me, Game->GetHero());
        }
        Game->AddLineToConversation(conversation, "TEXT_QST_001_THUNDER_CONVERSATION_FIRST_LINE", false, Me, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_001_THUNDER_CONVERSATION_SECOND_LINE", false, Me, Game->GetHero());
        if (face.IsScripting())
            face.Release();
    }
    while (!ParentClass->ThunderFightOver) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
        }
        if (IsDistanceBetweenThingsOver(Game->GetHero(), Me, 8.0f)) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            if (face.IsScripting())
                face.Release();
            while (!Game->StartScriptingEntity(Me, face, (EScriptAIPriority)4)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            face.MoveTo(Game->GetHero().GetPos(), 2.0f, true, false, true);
            while (IsDistanceBetweenThingsOver(Game->GetHero(), Me, 4.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
                    Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
                }
                if (ParentClass->ThunderFightOver) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    if (face.IsScripting())
                        face.Release();
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                }
                if (!face.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    face.MoveTo(Game->GetHero().GetPos(), 2.0f, true, false, true);
                }
            }
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            if (face.IsScripting())
                face.Release();
        }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
#pragma inline_depth(0)
}
#pragma inline_depth()