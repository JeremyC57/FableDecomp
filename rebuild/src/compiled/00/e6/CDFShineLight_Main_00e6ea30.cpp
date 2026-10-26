// CDF_ShineLight::Main() -- demon-door face (shine a light): take scripted control of Me, make it persistent and immune, shut the nearest REGION_EXIT_POINT; each frame -- if the hero is within the trigger distance with the hand lamp lit, play the opening (teleport the hero to MK_DD, wide camera, opening line), fade the face out, open the door and reactivate the exit; else if the hero talks to the face, play the intro (hide the hero, close camera, intro line)
// Leaf: ?Main@CDF_ShineLight@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6EA30, 1537 bytes (+ int3 padding), plain `ret`. Literals "CAM_DD_CLOSE" 0x012E8CA4,
//   "TEXT_QST_088_SHINE_LIGHT_INTRO" 0x012E8C84, "MK_DD" 0x012E8C7C, "CAM_DD_WIDE" 0x012E8C70, "TEXT_QST_088_SHINE_LIGHT_OPENING"
//   0x012E8C4C, "DD_ShineLight" 0x012E8C3C (sibling of the matched CDF_CombatMult::Main 0x00E6F710).
// Callees: CBaseIntelligentPointer ctor 0x0099A380 / dtor 0x0099A430, scripted-thing handle IsScripting 0x00CD23B9 / Release 0x00CD2770 /
//   GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / out-of-line dtor 0x007E74D0, movie handle
//   out-of-line dtor 0x006E7B80, CIsThingAlive (vtable 0x012C32F0, out-of-line dtor 0x00E241B0), CScriptThing copy 0x004ABE90 /
//   dtor 0x004AA840, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, operator delete 0x00BFE9BC, CCharString
//   0x0099EBF0 / 0x0099EAE0, CV_DemonDoorsScript::FadeOutFaceAndOpenDoor 0x00E6F040; GameCalls slots 7, 8, 70 GetHero,
//   81 GetNearestWithDefName, 72 GetThingWithScriptName, 185 IsHeroHandLampLit, 371 FixMovieSequenceCamera, 413 CameraUseCameraPoint, 472 EntityTeleportToThing, 511 EntitySetAsDrawable, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), movie ctor 0x006E7B60 (out of line in the lamp arm), 264 GetHealth-sized float query, 370 StartMovieSequence,
//   379 PauseAllNonScriptedEntities, 477 SetThingPersistent, 516/517/526 damageable/killable/combo, 567 SetRegionExitAsActive;
//   CScriptThing slot 27 MsgIsTalkedToBy.
// Layout: Ego_r.pdb CDF_ShineLight (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CV_DemonDoorsScript),
//   PMasterData +0x18. CScriptDef DemonDoor_ShineLight_TriggerDistance at retail +0x1064. Handle hierarchy as the landed
//   CMeleeThunder::Main 0x00D58080.
// Shape: the CMeleeThunder handle model (implicit handle dtor expanded only at the first return, inline_depth(0) elsewhere).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
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
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    VPAD16(ma) VPAD8(mb) VPAD2(mc)                      // slots 1..26
    virtual bool MsgIsTalkedToBy(const CCharString& name) const;   // slot 27 (+0x6c)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing();                                    // 0x004AA840
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
struct CIsThingAlive : public NScript::CPredicateBase {                                  // vtable 0x012C32F0
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                                   // 0x00E241B0
};
class CBaseObject { public:                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    // implicit destructor (stores no vptr); out-of-line copy 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // 0x007E7390
public:
    virtual CScriptThing GetScriptThing() const;   // 0x007E7490
    virtual bool IsPerformingScriptTask() const;   // 0x007E7450
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectMovieBase() { }
    ~CScriptGameResourceObjectMovieBase();              // 0x006E7B80
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CGameScriptInterface { public:
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD1(qe)                              // slots 9..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD1(r0)                                                                        // slot 71
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;      // slot 72 (+0x120)
    VPAD8(ra)                                                                        // slots 73..80
    virtual CScriptThing GetNearestWithDefName(const CScriptThing& thing, const CCharString& defName) const;   // slot 81 (+0x144)
    VPAD64(sa) VPAD2(sb) VPAD1(sc)                                                   // slots 82..148
    VPAD32(ta) VPAD2(tb) VPAD1(tc) VPAD1(td)                                         // slots 149..184
    virtual bool IsHeroHandLampLit() const;                                          // slot 185 (+0x2e4)
    VPAD64(te) VPAD8(tf) VPAD4(tg) VPAD2(th)                                         // slots 186..263
    virtual float GetHealth(const CScriptThing& thing) const;                        // slot 264 (+0x420)
    VPAD64(ua) VPAD32(ub) VPAD8(uc) VPAD1(ud)                                        // slots 265..369
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    virtual void FixMovieSequenceCamera(bool fix) const;                             // slot 371 (+0x5cc)
    VPAD4(va) VPAD2(vb) VPAD1(vc)                                                    // slots 372..378
    virtual void PauseAllNonScriptedEntities(bool pause);                            // slot 379 (+0x5ec)
    VPAD32(wa)                                                                       // slots 380..412
    VPAD1(wa0)
    virtual void CameraUseCameraPoint(const CScriptThing& point, const CScriptThing& target, float time, long a, long b) const;   // slot 413 (+0x674)
    VPAD32(wb) VPAD16(wc) VPAD8(wd) VPAD2(we)                                        // slots 414..471
    virtual void EntityTeleportToThing(const CScriptThing& thing, const CScriptThing& target, bool a) const;   // slot 472 (+0x760)
    VPAD4(wf)                                                                        // slots 473..476
    virtual void SetThingPersistent(const CScriptThing& thing, bool persistent) const;   // slot 477 (+0x774)
    VPAD32(xa) VPAD1(xb)                                                             // slots 478..510
    virtual void EntitySetAsDrawable(const CScriptThing& thing, bool drawable) const;   // slot 511 (+0x7fc)
    VPAD4(xc)                                                                        // slots 512..515
    virtual void EntitySetAsDamageable(const CScriptThing& thing, bool damageable) const;   // slot 516 (+0x810)
    virtual void EntitySetAsKillable(const CScriptThing& thing, bool killable, bool b) const;   // slot 517 (+0x814)
    VPAD8(ya)                                                                        // slots 518..525
    virtual void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& thing, bool add) const;   // slot 526 (+0x838)
    VPAD32(za) VPAD8(zb)                                                             // slots 527..566
    virtual void SetRegionExitAsActive(const CScriptThing& exit, bool active) const; // slot 567 (+0x8dc)
};
class CScriptDef { public:
    unsigned char _pad[0x1064];
    float DemonDoor_ShineLight_TriggerDistance;         // +0x1064
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
extern const float g_fZero;                            // 0x0122DEDC (0.0f)
struct CV_DemonDoorsScript {
    void FadeOutFaceAndOpenDoor(const CScriptThing& face, const CCharString& doorName);   // 0x00E6F040
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CDF_ShineLight : CEntityScriptBase {
    CV_DemonDoorsScript* ParentClass;                   // +0x14
    void*                PMasterData;                   // +0x18
    void Main();
};

void CDF_ShineLight::Main()
{
    CScriptGameResourceObjectScriptedThingBase face;
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
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
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), pGlobalScriptDef->DemonDoor_ShineLight_TriggerDistance) && Game->IsHeroHandLampLit()) {
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
            Game->FixMovieSequenceCamera(true);
            Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("MK_DD"), false);
            Game->CameraUseCameraPoint(Game->GetThingWithScriptName("CAM_DD_WIDE"), Me, -1.0f, 0, -1);
            if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                face.Speak(Game->GetHero(), "TEXT_QST_088_SHINE_LIGHT_OPENING", (ETextGroupSelectionMethod)0, false, true, false);
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
                CCharString door("DD_ShineLight");
                ParentClass->FadeOutFaceAndOpenDoor(Me, door);
            }
            Game->SetRegionExitAsActive(exitPoint, true);
            Game->FixMovieSequenceCamera(false);
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
            Game->FixMovieSequenceCamera(true);
            Game->EntitySetAsDrawable(Game->GetHero(), false);
            Game->CameraUseCameraPoint(Game->GetThingWithScriptName("CAM_DD_CLOSE"), Me, -1.0f, 0, -1);
            if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                face.Speak(Game->GetHero(), "TEXT_QST_088_SHINE_LIGHT_INTRO", (ETextGroupSelectionMethod)0, false, true, false);
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
            Game->EntitySetAsDrawable(Game->GetHero(), true);
            Game->FixMovieSequenceCamera(false);
            game->PauseAllNonScriptedEntities(false);
        }
        Game->NewScriptFrame();
    }
#pragma inline_depth(0)
}
#pragma inline_depth()