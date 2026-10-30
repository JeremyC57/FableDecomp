// CSummonerAttacker::Main() -- a summoner guarding the Fire Heart: hide Me, wait until the quest activates this summoner's wave, fade Me in (with a blue health bar for summoner 1), then forever: script Me; while the hero is more than 15 units away keep the lightning-orb attack on the Fire Heart going and, when the hero hits Me, respawn either dead wardog minion near the hero; once the hero closes in, release Me, drop its minimap marker and wait until the hero backs off
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF2380, 1494 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Literals "HUD_ORB_QUEST_CORE" 0x01258AB0, "HUD_QUEST_ICON_SUMMONER_BLUE" 0x012DE298, "FireHeart" 0x012DE154,
//   "SCRIPT_NAME_HERO" 0x0125D1C8, "SummonerMinion" 0x012DE170, "CREATURE_MINION_WARDOG" 0x012CB9F4.
// Callees: CIsThingAlive filter (vtable 0x012C32F0) over CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840,
//   CBaseClass ctor 0x0099A2D0, CScriptThing::operator= 0x008AB980, RegisterFilter 0x00F35B10, IsActiveThreadTerminating
//   0x00F35B30, CBaseIntelligentPointer ctor 0x0099A380, handle IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   SummonerLightningOrbAttackTarget 0x007E72D0 / IsPerformingScriptTask 0x007E7450 / dtor 0x007E74D0 (direct calls on the
//   local handle), FadeInThing 0x00CBE7AA / IsDistanceBetweenThingsUnder 0x00CBE2FF / IsDistanceBetweenThingsOver 0x00CBE3EA
//   (__fastcall), CCharString 0x0099EBF0 / 0x0099EAE0; GameCalls slots 7, 8, 70 GetHero, 72 GetThingWithScriptName,
//   92 CreateCreatureNearby, 325 AddQuestInfoBarHealth, 350 MiniMapAddMarker, 352 MiniMapRemoveMarker, 453 EntitySetInLimbo,
//   510 EntitySetAlpha; CScriptThing slots 6 GetPos, 21 MsgIsHitBy, 41/42 special-ability hits, 75 IsAlive.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (summoner quest: WaveSummoner +0x4c, WaveActive
//   +0x51), PMasterData +0x18, SummonerIndex +0x1c.
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
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
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
class C3DVector { public: float X, Y, Z; };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImpl;
class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const;
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const;
    virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const;
    virtual bool MsgIsHitBy(const CCharString& hitter) const;   // slot 21 (+0x54)
    virtual void s022() const; virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const;
    virtual void s030() const; virtual void s031() const; virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const;
    virtual void s038() const; virtual void s039() const; virtual void s040() const;
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& from) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& from) const;   // slot 42 (+0xa8)
    virtual void s043() const; virtual void s044() const; virtual void s045() const; virtual void s046() const; virtual void s047() const; virtual void s048() const; virtual void s049() const; virtual void s050() const;
    virtual void s051() const; virtual void s052() const; virtual void s053() const; virtual void s054() const; virtual void s055() const; virtual void s056() const; virtual void s057() const; virtual void s058() const;
    virtual void s059() const; virtual void s060() const; virtual void s061() const; virtual void s062() const; virtual void s063() const; virtual void s064() const; virtual void s065() const; virtual void s066() const;
    virtual void s067() const; virtual void s068() const; virtual void s069() const; virtual void s070() const; virtual void s071() const; virtual void s072() const; virtual void s073() const; virtual void s074() const;
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    __forceinline CScriptThing() { PImp.Data = 0; PImp.Info = 0; }
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                                    // 0x004AA840
    CScriptThing& operator=(const CScriptThing& other); // 0x008AB980
};
inline bool IsThingAlive(const CScriptThing& thing) { return thing.IsAlive(); }   // a virtual call (not devirtualised)
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    virtual void h001() const;
    virtual void SummonerLightningOrbAttackTarget(const CScriptThing& target);   // slot 2 (+0x8)
    virtual void h003() const; virtual void h004() const; virtual void h005() const; virtual void h006() const; virtual void h007() const; virtual void h008() const; virtual void h009() const; virtual void h010() const;
    virtual void h011() const; virtual void h012() const; virtual void h013() const; virtual void h014() const; virtual void h015() const; virtual void h016() const; virtual void h017() const; virtual void h018() const;
    virtual void h019() const; virtual void h020() const; virtual void h021() const; virtual void h022() const; virtual void h023() const; virtual void h024() const; virtual void h025() const;
    virtual bool IsPerformingScriptTask() const;   // slot 26 (+0x68)
    void* PImpData;                                     // +0x08
    void* PImpInfo;                                     // +0x0c
    __forceinline CScriptGameResourceObjectScriptedThingBase() : PImpData(0), PImpInfo(0) { }
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void __fastcall FadeInThing(const CScriptThing& thing, float time);                               // 0x00CBE7AA
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
struct CQ_SummonerScript {
    unsigned char _pad00[0x4c];
    long WaveSummoner;                                  // +0x4c
    unsigned char _pad50;
    bool WaveActive;                                    // +0x51
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CSummonerAttacker : CEntityScriptBase {
    CQ_SummonerScript* ParentClass;                     // +0x14
    void*              PMasterData;                     // +0x18
    long               SummonerIndex;                   // +0x1c
    void Main();
};

void CSummonerAttacker::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAlpha(Me, 0.0f, true);
    Game->EntitySetInLimbo(Me, true, true);
    while (!(ParentClass->WaveActive && ParentClass->WaveSummoner == SummonerIndex)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    Game->EntitySetInLimbo(Me, false, true);
    FadeInThing(Me, 2.0f);
    if (SummonerIndex == 1) {
        if (IsActiveThreadTerminating())
            return;
        Game->AddQuestInfoBarHealth(Me, CRGBColour(0xff, 0, 0, 0xff), "HUD_QUEST_ICON_SUMMONER_BLUE", 1.0f);
    }
    CScriptGameResourceObjectScriptedThingBase summoner;
    CScriptThing fireHeart = Game->GetThingWithScriptName("FireHeart");
    CScriptThing minion1;
    CScriptThing minion2;
    if (IsActiveThreadTerminating())
        return;
    do {
        if (summoner.IsValid())
            summoner.Release();
        while (!Game->StartScriptingEntity(Me, summoner, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsOver(Me, Game->GetHero(), 15.0f)) {
            do {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                if (!summoner.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating())
                        return;
                    summoner.SummonerLightningOrbAttackTarget(fireHeart);
                }
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                    || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (!IsThingAlive(minion1)) {
                        if (IsActiveThreadTerminating())
                            return;
                        minion1 = Game->CreateCreatureNearby("SummonerMinion", Game->GetHero().GetPos(), 15.0f, "CREATURE_MINION_WARDOG", true);
                    }
                    if (!IsThingAlive(minion2)) {
                        if (IsActiveThreadTerminating())
                            return;
                        minion2 = Game->CreateCreatureNearby("SummonerMinion", Game->GetHero().GetPos(), 15.0f, "CREATURE_MINION_WARDOG", true);
                    }
                }
            } while (IsDistanceBetweenThingsOver(Me, Game->GetHero(), 15.0f));
        }
        if (IsActiveThreadTerminating())
            return;
        if (summoner.IsValid())
            summoner.Release();
        Game->MiniMapRemoveMarker(Me);
        while (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}