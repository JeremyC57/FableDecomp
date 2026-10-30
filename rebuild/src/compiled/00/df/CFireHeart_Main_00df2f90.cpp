// CFireHeart::Main() -- the summoning-the-ship fire heart: fade Me in once the quest is ready, show its health bar, then while its health stays above 1 play a heartbeat whose speed follows the health, and when the summoner's attackers hit it let the quest comment (every 20 ticks); finally flag the heart destroyed
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF2F90, 1220 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: IsActiveThreadTerminating 0x00F35B30, CTimer dtor 0x00CD4470, CScriptThing dtor 0x004AA840,
//   CCharString 0x0099EBF0 / 0x0099EAE0, CQ_SummoningTheShipScript::MakeBriarRoseComment 0x00DF20F0; GameCalls slots 7, 72,
//   87/89/90 timers (through g_pGameScriptInterface 0x0143E8F8), 264 GetHealth, 321, 325, 352, 510, 516, 692 Play2DSound;
//   CScriptThing slots 21/41/42 (hit-by messages).
// Layout: Ego_r.pdb CFireHeart: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_SummoningTheShipScript; retail
//   bools at +0x50/+0x51/+0x52 = PDB - 0x14), PMasterData +0x18.
enum EHeroAbility { HERO_ABILITY_NONE };
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
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
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
struct CBase {                                          // vtable 0x01231710
    CBase() { }                                         // out-of-line copy 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const; virtual void s006() const; virtual void s007() const; virtual void s008() const;
    virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const;
    virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const;
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022() const; virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const;
    virtual void s030() const; virtual void s031() const; virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const;
    virtual void s038() const; virtual void s039() const; virtual void s040() const;
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBase(), PImp(other.PImp) { }
    virtual ~CScriptThing();                                    // 0x004AA840
};
struct CBaseIntelligentPointer {                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
    CBaseIntelligentPointer();                          // 0x0099A380
    virtual ~CBaseIntelligentPointer();                 // 0x0099A430
};
class CScriptGameResourceObjectBase : public CBaseIntelligentPointer { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CRGBColour { public:                                     // B,G,R,A bytes
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
class CTimer { public:
    long TimerIndex;
    CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
    ~CTimer();                                          // 0x00CD4470
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
#pragma auto_inline(off)
CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
#pragma auto_inline(on)
struct CQ_SummoningTheShipScript {
    char pad00[0x50];
    bool HeartReady;                                    // +0x50
    bool HeartActive;                                   // +0x51
    bool HeartDestroyed;                                // +0x52
    bool MakeBriarRoseComment(const CCharString& comment);   // 0x00DF20F0
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CFireHeart : CEntityScriptBase {
    CQ_SummoningTheShipScript* ParentClass;             // +0x14
    void* PMasterData;                                  // +0x18
    void Main();
};

void CFireHeart::Main()
{
    Game->EntitySetAlpha(Me, 0.0f, true);
    while (!ParentClass->HeartReady) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->MiniMapRemoveMarker(Me);
    while (!ParentClass->HeartActive) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsDamageable(Me, true);
    Game->DisplayQuestInfo(true);
    Game->EntitySetAlpha(Me, 1.0f, true);
    CTimer commentTimer;
    CScriptThing fireHeart = Game->GetThingWithScriptName("FireHeart");
    Game->AddQuestInfoBarHealth(fireHeart, CRGBColour(0, 255, 0, 255), "HUD_ICON_FIRE_HEART", 1.0f);
    CTimer beatTimer;
    beatTimer.SetTime(3);
    while (Game->GetHealth(Me) > 1.0f) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (beatTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return;
            if (Game->GetHealth(Me) <= 750.0f) {
                if (IsActiveThreadTerminating())
                    return;
                Game->Play2DSound("SND_MM_FIREHEART_BEAT_FAST");
                beatTimer.SetTime(1);
            } else if (Game->GetHealth(Me) <= 1500.0f) {
                if (IsActiveThreadTerminating())
                    return;
                Game->Play2DSound("SND_MM_FIREHEART_BEAT_MEDIUM");
                beatTimer.SetTime(2);
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->Play2DSound("SND_MM_FIREHEART_BEAT_SLOW");
                beatTimer.SetTime(3);
            }
        }
        if (Me.MsgIsHitBy("SummonerAttacker")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SummonerAttacker") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            if (commentTimer.GetTime() == 0 && ParentClass->MakeBriarRoseComment("FIREHEART")) {
                if (IsActiveThreadTerminating())
                    return;
                commentTimer.SetTime(20);
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->HeartDestroyed = true;
}