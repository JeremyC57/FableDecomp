// NScript::CQ_SummoningTheShipScript::CSummonerAttacker::Main() -- a summoner guarding the Fire Heart: hide Me, wait until the quest activates this summoner's wave, fade Me in (with a blue health bar for summoner 1), then forever: script Me; while the hero is more than 15 units away keep the lightning-orb attack on the Fire Heart going and, when the hero hits Me, respawn either dead wardog minion near the hero; once the hero closes in, release Me, drop its minimap marker and wait until the hero backs off
// Leaf: Main
// Identity (naming wave 11): ?Main@CSummonerAttacker@CQ_SummoningTheShipScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012DE274 slot 1 is stored by the factory 0x00DF21F0 registered as "SummonerAttacker"
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
#include "engine/CCountedPointer.h"
class C3DVector { public: float X, Y, Z; };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImpl;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::CScriptThing() { PImp.Data = 0; PImp.Info = 0; }
inline bool IsThingAlive(const CScriptThing& thing) { return thing.IsAlive(); }   // a virtual call (not devirtualised)
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
}
namespace NScript {
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
namespace NScript {
class CQ_SummoningTheShipScript {
public:
    unsigned char _pad00[0x4c];
    long WaveSummoner;                                  // +0x4c
    unsigned char _pad50;
    bool WaveActive;                                    // +0x51
public:
    class CSummonerAttacker;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_SummoningTheShipScript;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SummoningTheShipScript::CSummonerAttacker : public CEntityScriptBase {
public:
    CQ_SummoningTheShipScript* ParentClass;                     // +0x14
    void*              PMasterData;                     // +0x18
    long               SummonerIndex;                   // +0x1c
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_SummoningTheShipScript::CSummonerAttacker::Main()
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
                    || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
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