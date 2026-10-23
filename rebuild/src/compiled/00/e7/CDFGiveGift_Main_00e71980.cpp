// CDF_GiveGift::Main() -- demon-door face (give a gift): take scripted control of Me, make it persistent and immune, find the nearest REGION_EXIT_POINT and shut it; then each frame -- if Me is presented with a romantic gift, speak the opening line, fade the face out, open the door and reactivate the exit; else if the hero talks to the face, speak the intro line
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E71980, 1201 bytes (+ int3 padding), plain `ret`. Literals "TEXT_QST_088_GIVE_GIFT_INTRO" 0x012E9088,
//   "TEXT_QST_088_GIVE_GIFT_OPENING" 0x012E9068, "DD_GiveGift" 0x012E905C (sibling of the matched CDF_CombatMult::Main 0x00E6F710).
// Callees: CBaseIntelligentPointer ctor 0x0099A380 / dtor 0x0099A430, scripted-thing handle IsScripting 0x00CD23B9 / Release 0x00CD2770 /
//   GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / out-of-line dtor 0x007E74D0, movie handle
//   out-of-line dtor 0x006E7B80, CIsThingAlive (vtable 0x012C32F0, out-of-line dtor 0x00E241B0), CScriptThing copy 0x004ABE90 /
//   dtor 0x004AA840, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, operator delete 0x00BFE9BC, CCharString
//   0x0099EBF0 / 0x0099EAE0, CV_DemonDoorsScript::FadeOutFaceAndOpenDoor 0x00E6F040; GameCalls slots 7, 8, 70 GetHero,
//   81 GetNearestWithDefName, 679 IsGiftRomantic, CCharString default ctor 0x0099E4B0, movie ctor 0x006E7B60 (out of line in the gift arm only), 264 GetHealth-sized float query, 370 StartMovieSequence,
//   379 PauseAllNonScriptedEntities, 477 SetThingPersistent, 516/517/526 damageable/killable/combo, 567 SetRegionExitAsActive;
//   CScriptThing slot 27 MsgIsTalkedToBy.
// Layout: Ego_r.pdb CDF_GiveGift (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CV_DemonDoorsScript),
//   PMasterData +0x18. Handle hierarchy as the landed
//   CMeleeThunder::Main 0x00D58080.
// Shape: the CMeleeThunder handle model (implicit handle dtor expanded only at the first return, inline_depth(0) elsewhere).
struct CCharString {
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
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
    VPAD16(ma) VPAD8(mb) VPAD2(mc)                      // slots 1..26
    virtual bool MsgIsTalkedToBy(const CCharString& name) const;   // slot 27 (+0x6c)
    VPAD4(md) VPAD2(me) VPAD1(mf)                       // slots 28..34
    virtual bool MsgIsPresentedWithItem(CCharString& item) const;   // slot 35 (+0x8c)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing();                                    // 0x004AA840
};
struct CIsThingAlive {                                  // vtable 0x012C32F0
    virtual void v00();
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                                   // 0x00E241B0
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
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    // implicit destructor (stores no vptr); out-of-line copy 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    CScriptThing GetScriptThing() const;                // 0x007E7490
    void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // 0x007E7390
    bool IsPerformingScriptTask() const;                // 0x007E7450
};
struct CScriptGameResourceObjectMovieBase : CScriptGameResourceObjectBase {   // vtable 0x01260EF4, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectMovieBase() { }
    ~CScriptGameResourceObjectMovieBase();              // 0x006E7B80
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CGameScriptInterface {
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD1(qe)                              // slots 9..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD8(ra) VPAD2(rb)                                                              // slots 71..80
    virtual CScriptThing GetNearestWithDefName(const CScriptThing& thing, const CCharString& defName) const;   // slot 81 (+0x144)
    VPAD64(sa) VPAD2(sb) VPAD1(sc)                                                   // slots 82..148
    VPAD1(t0)                                                                        // slot 149
    VPAD64(ta) VPAD32(tb) VPAD16(tc) VPAD2(td)                                       // slots 150..263
    virtual float GetHealth(const CScriptThing& thing) const;                        // slot 264 (+0x420)
    VPAD64(ua) VPAD32(ub) VPAD8(uc) VPAD1(ud)                                        // slots 265..369
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    VPAD8(va)                                                                        // slots 371..378
    virtual void PauseAllNonScriptedEntities(bool pause);                            // slot 379 (+0x5ec)
    VPAD64(wa) VPAD32(wb)                                                            // slots 380..475
    VPAD1(wc)                                                                        // slot 476
    virtual void SetThingPersistent(const CScriptThing& thing, bool persistent) const;   // slot 477 (+0x774)
    VPAD32(xa) VPAD4(xb) VPAD2(xc)                                                   // slots 478..515
    virtual void EntitySetAsDamageable(const CScriptThing& thing, bool damageable) const;   // slot 516 (+0x810)
    virtual void EntitySetAsKillable(const CScriptThing& thing, bool killable, bool b) const;   // slot 517 (+0x814)
    VPAD8(ya)                                                                        // slots 518..525
    virtual void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& thing, bool add) const;   // slot 526 (+0x838)
    VPAD32(za) VPAD8(zb)                                                             // slots 527..566
    virtual void SetRegionExitAsActive(const CScriptThing& exit, bool active) const; // slot 567 (+0x8dc)
    VPAD64(zc) VPAD32(zd) VPAD8(ze) VPAD4(zf) VPAD2(zg) VPAD1(zh)                   // slots 568..678
    virtual bool IsGiftRomantic(const CCharString& item) const;                      // slot 679 (+0xa9c)
};
extern const float g_fZero;                            // 0x0122DEDC (0.0f)
struct CV_DemonDoorsScript {
    void FadeOutFaceAndOpenDoor(const CScriptThing& face, const CCharString& doorName);   // 0x00E6F040
};
struct CEntityScriptBase {
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    void RegisterFilter(CIsThingAlive& filter);         // 0x00F35B10
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CDF_GiveGift : CEntityScriptBase {
    CV_DemonDoorsScript* ParentClass;                   // +0x14
    void*                PMasterData;                   // +0x18
    void Main();
};

void CDF_GiveGift::Main()
{
    CScriptGameResourceObjectScriptedThingBase face;
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
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
    Game->SetThingPersistent(Me, true);
    Game->EntitySetAsDamageable(Me, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetAsKillable(Me, false, false);
    CScriptThing exitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
    Game->SetRegionExitAsActive(exitPoint, false);
    while (!IsActiveThreadTerminating()) {
        {
        CCharString gift;
        if (Me.MsgIsPresentedWithItem(gift) && Game->IsGiftRomantic(gift)) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
#pragma inline_depth(0)
            CScriptGameResourceObjectMovieBase movie;
#pragma inline_depth()
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                face.Speak(Game->GetHero(), "TEXT_QST_088_GIVE_GIFT_OPENING", (ETextGroupSelectionMethod)0, false, true, false);
                while (face.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                }
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            }
            {
                CCharString door("DD_GiveGift");
                ParentClass->FadeOutFaceAndOpenDoor(Me, door);
            }
            Game->SetRegionExitAsActive(exitPoint, true);
            game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        } else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                face.Speak(Game->GetHero(), "TEXT_QST_088_GIVE_GIFT_INTRO", (ETextGroupSelectionMethod)0, false, true, false);
                while (face.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                }
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            }
            game->PauseAllNonScriptedEntities(false);
        }
        }
        Game->NewScriptFrame();
    }
#pragma inline_depth(0)
}
#pragma inline_depth()