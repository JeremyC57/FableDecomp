// CCampHostage2::Main() -- bandit-camp boss battle, the second hostage: hold an alive filter over Me, take Me under scripted
//   control (priority 4, retried each frame); until the hostages are rescued or one is killed, whenever the cry timer has run
//   out and the hero is within 8 units, turn to the hero, play CS_DISAPPOINTED and cry out (alternating the FIRST/SECOND
//   lines in a new conversation with the hero), then rearm the timer for 8 seconds; finally release Me if not terminating
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D08790, 730 bytes (736 with padding); entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 (CIsThingAlive filter, vtable 0x012C32F0, inline dtor here),
//   CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CBaseObject ctor 0x0099A380,
//   scripted-thing handle (vtable 0x0127094C) IsValid 0x00CD23B9 / Release 0x00CD2770 / PlayAnimation 0x007E73D0 (slot 18)
//   / dtor 0x007E74D0, CTimer dtor 0x00CD4470, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CCharString ctor
//   0x0099EBF0 / dtor 0x0099EAE0; through Game (+4): 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero,
//   364 AddNewConversation, 365 AddPersonToConversation, 366 AddLineToConversation, 475 EntitySetFacingAngleTowardsThing;
//   through the global interface 0x0143E8F8: 87 RegisterTimer, 89 SetTimer, 90 GetTimer. The PlayAnimation flag read from
//   the global byte 0x01375748 (unnamed). Literals "CS_DISAPPOINTED" 0x012C90EC,
//   "TEXT_QST_009_HOSTAGE_SECOND_CRY_FIRST" 0x012C909C, "TEXT_QST_009_HOSTAGE_SECOND_CRY_SECOND" 0x012C90C4.
// Layout: Ego_r.pdb CCampHostage2 (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18;
//   CQ_BanditCampBossBattleScript HostagesRescued PDB +0x62 / HostageKilled +0x63 = retail +0x4e / +0x4f.
namespace NScript { class CPredicateBase; }
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CScriptThing { public:                                  // vtable 0x01238C8C, 12 bytes
    virtual void t000();
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase
{
public:
    virtual void f00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
extern bool g_PlayAnimationDefaultFlag;                // 0x01375748
class CTimer { public:                                        // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
    ~CTimer();                                         // 0x00CD4470
    long GetTime() const { return g_pGameScriptInterface->GetTimer(Index); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
};
struct CQ_BanditCampBossBattleScript {
    char _pad[0x4e];
    bool HostagesRescued;                              // +0x4e
    bool HostageKilled;                                // +0x4f
};
struct CCampHostage2 : CEntityScriptBase {
    CQ_BanditCampBossBattleScript* ParentClass;        // +0x14
    void*                          PMasterData;        // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};

// R&D I-3 sweep: a same-TU auto_inline(off) body for the timer dtor 0x00CD4470 (it only reads its own index) lets the
// timer index and the constant 0 stay cached in ebx/ebp across the loop's calls, as in retail.
long UnregisterScriptTimer(long index);
#pragma auto_inline(off)
CTimer::~CTimer()
{
    UnregisterScriptTimer(Index);
}
#pragma auto_inline(on)
void CCampHostage2::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating()) {
        CTimer cryTimer;
        cryTimer.SetTime(0);
        long cry = 0;
        while (!ParentClass->HostagesRescued && !ParentClass->HostageKilled) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (cryTimer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 8.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                scripted.PlayAnimation("CS_DISAPPOINTED", false, false, false, true, g_PlayAnimationDefaultFlag, false, false);
                switch (cry) {
                case 0:
                    Game->AddLineToConversation(conversation, "TEXT_QST_009_HOSTAGE_SECOND_CRY_FIRST", false, Me, Game->GetHero());
                    cry = 1;
                    break;
                case 1:
                    Game->AddLineToConversation(conversation, "TEXT_QST_009_HOSTAGE_SECOND_CRY_SECOND", false, Me, Game->GetHero());
                    cry = 0;
                    break;
                }
                cryTimer.SetTime(8);
            }
        }
        if (!IsActiveThreadTerminating())
            scripted.Release();
    }
}