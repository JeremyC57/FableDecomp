// CAssassin3::Main() -- bandit-camp boss-battle assassin 3: register the alive filter, take Me under scripted control, then until the assassins are attacked or the assassin cut-scene is triggered: flag the attack when the hero hits Me (plainly or with a special ability other than 14), and when the hero talks to Me play a paused movie sequence in which Me (if alive) speaks its intro line the first time and its repeat line after; finally release Me if the cut-scene was triggered and clear its has-information marker
// Grade: DIFFER(1448v1400) -- dtor inline budget (third exit called out of line): retail expands the movie dtor at all three exits of each movie scope (the 2nd exit then cross-jumps into the 1st), ours calls it out of line at the 2nd exit; tried RAII vs explicit pause, non-virtual base dtor, /Ob1, inline_depth(255) + inline_recursion(on), __forceinline intermediate base dtor, a __forceinline user dtor (adds a vtable store)
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D06130, length 1408 bytes, plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0, inline user dtor), the scripted-thing handle (vtable 0x0127094C;
//   declared dtor 0x007E74D0 called at every exit) IsScripting 0x00CD23B9 / Release 0x00CD2770 / Speak 0x007E7390 /
//   IsPerformingScriptTask 0x007E7450 / GetScriptThing 0x007E7490, the movie handle (vtable 0x01260EF4, inline ctor over
//   CBaseObject 0x0099A380, inline generated dtor: the counted pointer's dtor 0x006E7AB0, base vtable 0x0126008C,
//   ~CBaseObject 0x0099A430), CScriptThing copy 0x004ABE90 / dtor 0x004AA840, CCharString 0x0099EBF0 / 0x0099EAE0.
//   CGameScriptInterface slots 7, 8, 70, 264 GetHealth, 361 ClearThingHasInformation, 370 StartMovieSequence, 379
//   PauseAllNonScriptedEntities; CScriptThing slots 21, 27 MsgIsTalkedToBy, 41, 42.
// Literals: "SCRIPT_NAME_HERO" 0x0125D1C8, "" 0x0122D70E, "TEXT_QST_009_ASSASSIN3_INTRO" 0x012C8DC0, "TEXT_QST_009_ASSASSIN3_REPEAT" 0x012C8DA0.
// Layout: Ego_r.pdb CAssassin3 (28 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_BanditCampBossBattleScript, retail = PDB - 0x14: AssassinsUnderAttack +0x60, AssassinCutsceneTriggered +0x61),
//   PMasterData +0x18.
// Shape: the pause is a scoped guard over Game (one register), so every exit inside the movie block un-pauses first.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CScriptThingImp;
struct CCountedPointer {                               // {Data, Info}
    CScriptThingImp* Data;
    CCPPointerInfo*  Info;
};
enum EHeroAbility { HERO_ABILITY_14 = 14 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_0 };
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct CScriptThing {
    VPAD16(p0x0) VPAD4(p0x1) VPAD1(p0x2)   // slots 0..20
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    VPAD4(p1x0) VPAD1(p1x1)   // slots 22..26
    virtual bool MsgIsTalkedToBy(const CCharString& name) const;   // slot 27 (+0x6c)
    VPAD8(p2x0) VPAD4(p2x1) VPAD1(p2x2)   // slots 28..40
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    CCountedPointer PImp;                              // +0x04
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
};
struct CIsThingAlive {                                 // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
struct CBaseObject {                                   // {vptr, intelligent-pointer link}
    void* IntelligentPointer;                          // +0x04
    CBaseObject();                                     // 0x0099A380
    virtual ~CBaseObject();                            // 0x0099A430
};
struct CScriptGameResourceObjectBase : CBaseObject {   // vtable 0x0126008C
    ~CScriptGameResourceObjectBase() { }
};
struct CCountedPointerWords { void* Data; void* Info; };
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C, 16 bytes
    CCountedPointerWords PImp;                         // +0x08
    CScriptGameResourceObjectScriptedThingBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // 0x007E7390
    bool IsPerformingScriptTask() const;               // 0x007E7450
    CScriptThing GetScriptThing() const;               // 0x007E7490
};
struct CMovieImpPointer {                              // CCountedPointer<movie imp>: {Data, Info}
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) { }
    ~CMovieImpPointer();                               // 0x006E7AB0
};
struct CScriptGameResourceObjectMovieBase : CScriptGameResourceObjectBase {   // vtable 0x01260EF4, 16 bytes
    CMovieImpPointer PImp;                             // +0x08
};
struct CGameScriptInterface {
    VPAD4(p0x0) VPAD2(p0x1) VPAD1(p0x2)   // slots 0..6
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(p1x0) VPAD16(p1x1) VPAD8(p1x2) VPAD4(p1x3) VPAD1(p1x4)   // slots 9..69
    virtual const CScriptThing& GetHero() const;   // slot 70 (+0x118)
    VPAD128(p2x0) VPAD64(p2x1) VPAD1(p2x2)   // slots 71..263
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420)
    VPAD64(p3x0) VPAD32(p3x1)   // slots 265..360
    virtual void ClearThingHasInformation(const CScriptThing& thing) const;   // slot 361 (+0x5a4)
    VPAD8(p4x0)   // slots 362..369
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    VPAD8(p5x0)   // slots 371..378
    virtual void PauseAllNonScriptedEntities(bool pause);   // slot 379 (+0x5ec)
};

struct CQ_BanditCampBossBattleScript {
    unsigned char _pad[0x60];
    bool AssassinsUnderAttack;                         // +0x60
    bool AssassinCutsceneTriggered;                    // +0x61
};
struct CPauseNonScripted {                             // scoped PauseAllNonScriptedEntities(true/false)
    CGameScriptInterface* G;
    CPauseNonScripted(CGameScriptInterface* game) : G(game) { G->PauseAllNonScriptedEntities(true); }
    ~CPauseNonScripted() { G->PauseAllNonScriptedEntities(false); }
};
struct CQ_SunnyvaleMasterData;
class CAssassin3 {
public:
    void*                          __vftable;          // +0x00
    CGameScriptInterface*          Game;               // +0x04
    CScriptThing                   Me;                 // +0x08
    CQ_BanditCampBossBattleScript* ParentClass;        // +0x14
    CQ_SunnyvaleMasterData*        PMasterData;        // +0x18
    void RegisterFilter(CIsThingAlive& filter);        // 0x00F35B10
    bool IsActiveThreadTerminating();                  // 0x00F35B30
    void Main();
};

void CAssassin3::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    bool introduced = false;
    while (!ParentClass->AssassinsUnderAttack && !ParentClass->AssassinCutsceneTriggered) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->AssassinsUnderAttack = true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (!introduced) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CPauseNonScripted pause(Game);
                if (Game->GetHealth(scripted.GetScriptThing()) > 0.0f) {
                    scripted.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN3_INTRO", (ETextGroupSelectionMethod)0, false, true, false);
                    while (scripted.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                introduced = true;
            } else {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CPauseNonScripted pause(Game);
                if (Game->GetHealth(scripted.GetScriptThing()) > 0.0f) {
                    scripted.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN3_REPEAT", (ETextGroupSelectionMethod)0, false, true, false);
                    while (scripted.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (ParentClass->AssassinCutsceneTriggered) {
        if (IsActiveThreadTerminating())
            return;
        scripted.Release();
    }
    Game->ClearThingHasInformation(Me);
}
