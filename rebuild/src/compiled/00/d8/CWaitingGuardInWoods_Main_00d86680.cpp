// CWaitingGuardInWoods::Main() -- a guard waiting in the Guildmaster's woods: alive filter on Me, scripted control (priority 4, retried each frame), wait for the guildmaster cutscene flag, put a small red orb on the mini-map, then idle until the distance stage is reached, the hero hits him (directly, or with any special ability but number 14) or the hero comes close (or within the outer range while he is on screen); then target the hero and idle forever
// Leaf: ?Main@CWaitingGuardInWoods@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D86680, 762 bytes (+ int3 padding); entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30,
//   CBaseObject ctor 0x0099A380, the scripted-thing handle's IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0,
//   CCharString 0x0099EBF0 / 0x0099EAE0, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall); CScriptThing slots 6
//   GetPosition, 21 MsgIsHitBy, 41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom; GameCalls slots 7
//   NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 350 MiniMapAddMarker, 421 IsCameraPosOnScreen, 451
//   GiveThingBestEnemyTarget. Strings "HUD_ORB_RED_SMALL" 0x012D4BF0, "SCRIPT_NAME_HERO" 0x0125D1C8; pGlobalScriptDef 0x0143E90C.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_HeroSoulsGuildmasterScript: PlayedGuildMasterCS
//   PDB +0x5d = retail +0x49, DistanceReached PDB +0x68 = retail +0x54; the 0x14 shift of the landed sibling
//   CMuteGuardOutsideWoods::Main 0x00D86150), PMasterData +0x18. The two CScriptDef longs at retail +0xc94 / +0xc98 are the
//   guildmaster-woods guard ranges (HS_GM* block of the PDB; exact names not provable from the bytes, named descriptively).
// Shape: the sibling's model; the wait condition owns three conditional CCharString temporaries (flag word in ebx,
//   destroyed in reverse) and is materialised as a bool before the branch.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void s000();   // slot 0 (+0x0)
    virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual const C3DVector& GetPosition() const;   // slot 6 (+0x18)
    virtual void s007(); virtual void s008();
    virtual void s009(); virtual void s010(); virtual void s011(); virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016();
    virtual void s017(); virtual void s018(); virtual void s019(); virtual void s020();
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025(); virtual void s026(); virtual void s027(); virtual void s028(); virtual void s029();
    virtual void s030(); virtual void s031(); virtual void s032(); virtual void s033(); virtual void s034(); virtual void s035(); virtual void s036(); virtual void s037();
    virtual void s038(); virtual void s039(); virtual void s040();
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CCPPointerInfo { public:                         // CCountedPointer's shared block (Ego_r.pdb CCPPointerInfo)
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4 destroys the pointee
    void* Data;                                        // +8
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
template <class T> class CCountedPointer { public:     // retail CCountedPointer<T>: Data +0, Info +4; the release
    T*              Data;                              // dtor is 0x007E74D0's inline PImp release (wave 6 GRO rule:
    CCPPointerInfo* Info;                              // VC7.1's per-site inlining of the implicit STB dtor follows
    CCountedPointer() : Data(0), Info(0) {}            // from this size)
    ~CCountedPointer()
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
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a predicate holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CQ_HeroSoulsGuildmasterScript {
    unsigned char _pad[0x48];
    bool PlayedBriarCS;                                // +0x48
    bool PlayedGuildMasterCS;                          // +0x49
    bool PlayedGuildCS;                                // +0x4a
    bool OutsideWoodGuardsAttack;                      // +0x4b
    bool GiveWarningCutscene;                          // +0x4c
    long GuardInWoodsCount;                            // +0x50
    long DistanceReached;                              // +0x54
};
class CScriptDef { public:
    unsigned char _pad[0xc94];
    long GuildmasterWoodsGuardOuterDistance;           // +0xc94
    long GuildmasterWoodsGuardInnerDistance;           // +0xc98
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

struct CWaitingGuardInWoods : CEntityScriptBase {
    CQ_HeroSoulsGuildmasterScript* ParentClass;        // +0x14
    void*                          PMasterData;        // +0x18
    void Main();
};

void CWaitingGuardInWoods::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (!ParentClass->PlayedGuildMasterCS) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->MiniMapAddMarker(Me, "HUD_ORB_RED_SMALL");
    while (ParentClass->DistanceReached != 2
           && !Me.MsgIsHitBy("SCRIPT_NAME_HERO")
           && (!Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
               || Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))
           && !IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->GuildmasterWoodsGuardInnerDistance)
           && (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->GuildmasterWoodsGuardOuterDistance)
               || !Game->IsCameraPosOnScreen(Me.GetPosition()))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (scripted.IsValid())
        scripted.Release();
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}