// CSecretWhisperingFriend::Main() -- the secret whispering man: look up "SecretWhisperingMan", take Me under scripted control, remove Me if the soul-mother quest is active, then while the quest stage (parent +0x48) is 5 give Me the hero as best enemy target, and while it is 1 walk Me back to its stored position and face the "M_SecretWhisperLookAtPos" marker
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC0BA0, 1114 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Literals "SecretWhisperingMan" 0x012F0FD0, "Q_HeroSoulsMother" 0x012D3210, "M_SecretWhisperLookAtPos" 0x012F11C0.
// Callees: CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840, ~CIsThingAlive 0x00E241B0, RegisterFilter 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseIntelligentPointer ctor 0x0099A380, handle IsScripting 0x00CD23B9 /
//   Release 0x00CD2770 / MoveToPosition 0x007E72F0 / ClearCommands 0x007E7360 / IsPerformingScriptTask 0x007E7450 /
//   dtor 0x007E74D0, IsDistanceFromThingToPositionOver 0x00CBE45C (__fastcall); GameCalls slots 7, 8, 70, 72, 108, 284, 451, 475;
//   CScriptThing slots 6 GetPos, 7 GetHomePos (results unused).
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (quest stage +0x48), home position C3DVector +0x1c.
namespace NScript { class CPredicateBase; }
class C3DVector { public: float X, Y, Z; };
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
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CScriptThing : public CBaseClass { public:
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const;
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    virtual C3DVector GetHomePos() const;   // slot 7 (+0x1c)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing() { }                                 // out-of-line copy 0x004AA840
};
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
    virtual ~CIsThingAlive();                                   // 0x00E241B0
};
}
using NScript::CIsThingAlive;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum EScriptEntityMoveType { SCRIPT_MOVE_DEFAULT };
class CBaseObject { public:                        // vtable 0x0129A7D8
    virtual void b000();
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C
    void* PImpData;                                     // +0x08
    void* PImpInfo;                                     // +0x0c
    CScriptGameResourceObjectScriptedThingBase() { PImpData = 0; PImpInfo = 0; }
    virtual ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    void ClearCommands();                               // 0x007E7360
public:
    virtual bool IsPerformingScriptTask() const;   // 0x007E7450
    virtual void MoveToPosition(const C3DVector&, float, EScriptEntityMoveType, bool, bool);   // 0x007E72F0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
struct CQuestParent {
    unsigned char _pad00[0x48];
    long Stage;                                         // +0x48
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CSecretWhisperingFriend : CEntityScriptBase {
    CQuestParent* ParentClass;                          // +0x14
    void*         PMasterData;                          // +0x18
    C3DVector     HomePosition;                         // +0x1c
    void Main();
};

void CSecretWhisperingFriend::Main()
{
    CScriptThing man = Game->GetThingWithScriptName("SecretWhisperingMan");
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
#pragma inline_depth(0)
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (Game->IsQuestActive("Q_HeroSoulsMother")) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, false, false);
    }
    if (IsActiveThreadTerminating())
        return;
    do {
        if (ParentClass->Stage == 5) {
            if (IsActiveThreadTerminating())
                return;
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            if (scripted.IsScripting())
                scripted.Release();
            while (ParentClass->Stage == 5) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (scripted.IsScripting())
                scripted.Release();
            while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (ParentClass->Stage == 1) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Me.GetHomePos();
            Me.GetPos();
            while (IsDistanceFromThingToPositionOver(Me, HomePosition, 0.5f) && ParentClass->Stage == 1) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                if (!scripted.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating())
                        return;
                    scripted.MoveToPosition(HomePosition, 0.0f, (EScriptEntityMoveType)0, false, false);
                }
            }
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->Stage != 5) {
                if (IsActiveThreadTerminating())
                    return;
                scripted.ClearCommands();
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("M_SecretWhisperLookAtPos"), false);
                while (ParentClass->Stage == 1) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
#pragma inline_depth()