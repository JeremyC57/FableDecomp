// CHS_Screamer::Main() -- a Hero Souls (Abbey mother) screamer: target the hero; until the mother's soul is collected, whenever no super screamer exists, the new-super-screamer timer has run out and the hero is over 5 units away, become the super screamer (mini-map marker, wait for the hero within 15 units), take scripted control, cast the unholy-power effect and drain the hero's health every timer period while lowering the mother's bar, starting the super attack once; finally play the death effect and remove Me
// Leaf: ?Main@CHS_Screamer@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D83FA0, 1230 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: scripted-thing handle ctor 0x007E72A0 / IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0, CTimer ctor
//   0x00CD4450 / dtor 0x00CD4470, CScriptThing copy 0x004ABE90 / dtor 0x004AA840 / operator= 0x008AB980, ~CBaseClass 0x0099A2E0,
//   operator delete 0x00BFE9BC, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder
//   0x00CBE2FF / IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall), CCharString 0x0099EBF0 / 0x0099EAE0; GameCalls slots 7, 8,
//   70 GetHero, 89/90 SetTimer/GetTimer (through g_pGameScriptInterface 0x0143E8F8), 100 CreateEffect, 108 RemoveThing,
//   263 ChangeHeroHealthBy, 350 MiniMapAddMarker, 451 GiveThingBestEnemyTarget, 542 EntityStartScreamerSuperAttackThing,
//   745 SetIsThingForcePushable, 844 SetIsPushableByHero; CScriptThing slot 6 GetPos.
// Layout: Ego_r.pdb CHS_Screamer (44 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_HeroSoulsMotherScript,
//   retail = PDB - 0x18: BarToFill +0x5c, GotSuperScreamer +0x65, NewSuperScreamerTimer +0x68), PMasterData +0x18
//   (CQ_SunnyvaleMasterData: CollectedSoulFromMother +0x66), SuperScreamer +0x1c, SuperScreamerEffect +0x20. CScriptDef
//   (retail = PDB - 0xb0): HS_ScreamerInc +0xc78 (landed as ScreamerBarIncrease), HS_SecondsBeforeSuperScreamerDamage +0xc7c,
//   HS_SuperScreamerDamagePerSecond +0xc80.
// Shape: in this large body retail calls the handle and CTimer constructors/destructors and the CScriptThing copy/destructor
//   out of line, and expands only the last (discarded) CreateEffect result's destructor inline: CScriptThing keeps its inline
//   destructor and inline_depth limits the earlier sites.
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class C3DVector { public: float X, Y, Z; };
class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const;
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(), PImp(other.PImp) { }
    __forceinline ~CScriptThing() { }                   // out-of-line copy 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);   // 0x008AB980
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
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0
    CScriptThing m_thing;                               // +0x04 (copied and destroyed out of line)
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();
};
}
using NScript::CIsThingAlive;
// The filter's destructor is inlined into Main, but the CScriptThing member it destroys stays an out-of-line call (0x004AA840).
#pragma inline_depth(0)
inline CIsThingAlive::~CIsThingAlive() { }
#pragma inline_depth()
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
__forceinline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
#pragma auto_inline(off)
CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
#pragma auto_inline(on)
class CScriptDef { public:
    unsigned char _pad[0xc78];
    float HS_ScreamerInc;                               // +0xc78
    long  HS_SecondsBeforeSuperScreamerDamage;          // +0xc7c
    float HS_SuperScreamerDamagePerSecond;              // +0xc80
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
struct CQ_HeroSoulsMotherScript {
    unsigned char _pad[0x5c];
    float  BarToFill;                                   // +0x5c (PDB +0x74)
    unsigned char _pad2[0x65 - 0x60];
    bool   GotSuperScreamer;                            // +0x65 (PDB +0x7d)
    CTimer NewSuperScreamerTimer;                       // +0x68 (PDB +0x80)
};
struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0x66];
    bool CollectedSoulFromMother;                       // +0x66
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CHS_Screamer : CEntityScriptBase {
    CQ_HeroSoulsMotherScript* ParentClass;              // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;              // +0x18
    bool                      SuperScreamer;            // +0x1c
    CScriptThing              SuperScreamerEffect;      // +0x20
    void Main();
};

void CHS_Screamer::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (!PMasterData->CollectedSoulFromMother) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!ParentClass->GotSuperScreamer && ParentClass->NewSuperScreamerTimer.GetTime() == 0
            && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 5.0f)) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->GotSuperScreamer = true;
            SuperScreamer = true;
            Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
            while (!(IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f) || PMasterData->CollectedSoulFromMother)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (!PMasterData->CollectedSoulFromMother) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectScriptedThingBase screamer;
                if (screamer.IsValid())
                    screamer.Release();
                while (!Game->StartScriptingEntity(Me, screamer, (EScriptAIPriority)4)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth(0)
                SuperScreamerEffect = Game->CreateEffect("UNHOLY_POWER_CAST_SMALL", Me.GetPos(), "", 0.0f, false, false);
#pragma inline_depth()
                Game->SetIsPushableByHero(Me, false);
                Game->SetIsThingForcePushable(Me, false);
                CTimer damageTimer;
                damageTimer = pGlobalScriptDef->HS_SecondsBeforeSuperScreamerDamage;
                bool attacking = false;
                for (;;) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (PMasterData->CollectedSoulFromMother)
                        break;
                    if (damageTimer.GetTime() == 0) {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->ChangeHeroHealthBy(pGlobalScriptDef->HS_SuperScreamerDamagePerSecond, true, true);
                        damageTimer = 1;
                        if (ParentClass->BarToFill > pGlobalScriptDef->HS_ScreamerInc) {
                            if (IsActiveThreadTerminating())
                                return;
                            ParentClass->BarToFill -= 0.05f;
                        }
                        if (!attacking) {
                            if (IsActiveThreadTerminating())
                                return;
                            Game->EntityStartScreamerSuperAttackThing(Me, Game->GetHero());
                            attacking = true;
                        }
                    }
                    Game->NewScriptFrame();
                }
                if (IsActiveThreadTerminating())
                    return;
                if (IsActiveThreadTerminating())
                    return;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->CreateEffect("SCREAMER_DIE_01", Me.GetPos(), "", 0.0f, false, false);
    Game->RemoveThing(Me, true, true);
}