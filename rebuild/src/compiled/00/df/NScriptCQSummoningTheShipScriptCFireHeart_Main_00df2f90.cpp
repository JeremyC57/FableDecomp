// NScript::CQ_SummoningTheShipScript::CFireHeart::Main() -- the summoning-the-ship fire heart: fade Me in once the quest is ready, show its health bar, then while its health stays above 1 play a heartbeat whose speed follows the health, and when the summoner's attackers hit it let the quest comment (every 20 ticks); finally flag the heart destroyed
// Leaf: ?Main@CFireHeart@CQ_SummoningTheShipScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF2F90, 1220 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: IsActiveThreadTerminating 0x00F35B30, CTimer dtor 0x00CD4470, CScriptThing dtor 0x004AA840,
//   CCharString 0x0099EBF0 / 0x0099EAE0, CQ_SummoningTheShipScript::MakeBriarRoseComment 0x00DF20F0; GameCalls slots 7, 72,
//   87/89/90 timers (through pDSTGame 0x0143E8F8), 264 GetHealth, 321, 325, 352, 510, 516, 692 Play2DSound;
//   CScriptThing slots 21/41/42 (hit-by messages).
// Layout: Ego_r.pdb CFireHeart: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_SummoningTheShipScript; retail
//   bools at +0x50/+0x51/+0x52 = PDB - 0x14), PMasterData +0x18.
// Naming wave 35: identity reg 00df157b "FireHeart" in 00df13a0; Alloc 00df2e90; vtable 012de2f0 slot 1; PDB ?Main@CFireHeart@CQ_SummoningTheShipScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CFireHeart::Main (wave3/labels_alloc.tsv); PDB ?Main@CFireHeart@CQ_SummoningTheShipScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CFireHeart@@QAEXXZ`; this source now defines the PDB member (outer stand-in CQ_SummoningTheShipScript moved into NScript); same bytes.
#include "fable_compiler.h"
enum EHeroAbility { HERO_ABILITY_NONE };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
struct CScriptThingImpl;
struct CBase {                                          // vtable 0x01231710
    CBase() { }                                         // out-of-line copy 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
#include "engine/C3DVector.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CScriptGameResourceObjectBase : public CBaseIntelligentPointer { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    FABLE_FORCEINLINE ~CScriptGameResourceObjectBase() { }
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
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#pragma auto_inline(off)
CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#pragma auto_inline(on)
namespace NScript {
class CQ_SummoningTheShipScript {
public:
    char pad00[0x50];
    bool HeartReady;                                    // +0x50
    bool HeartActive;                                   // +0x51
    bool HeartDestroyed;                                // +0x52
    bool MakeBriarRoseComment(const CCharString& comment);   // 0x00DF20F0
public:
    class CFireHeart;                // the entity class below (naming wave 35)
};
}
using NScript::CQ_SummoningTheShipScript;
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
namespace NScript {
class CQ_SummoningTheShipScript::CFireHeart : public CEntityScriptBase {
public:
    CQ_SummoningTheShipScript* ParentClass;             // +0x14
    void* PMasterData;                                  // +0x18
private:
    virtual void Main();                          // PDB ?Main@CFireHeart@CQ_SummoningTheShipScript@NScript@@EAEXXZ
};
}

void NScript::CQ_SummoningTheShipScript::CFireHeart::Main()
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
    beatTimer = 3;
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
                beatTimer = 1;
            } else if (Game->GetHealth(Me) <= 1500.0f) {
                if (IsActiveThreadTerminating())
                    return;
                Game->Play2DSound("SND_MM_FIREHEART_BEAT_MEDIUM");
                beatTimer = 2;
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->Play2DSound("SND_MM_FIREHEART_BEAT_SLOW");
                beatTimer = 3;
            }
        }
        if (Me.MsgIsHitBy("SummonerAttacker")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SummonerAttacker") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            if (commentTimer.GetTime() == 0 && ParentClass->MakeBriarRoseComment("FIREHEART")) {
                if (IsActiveThreadTerminating())
                    return;
                commentTimer = 20;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->HeartDestroyed = true;
}