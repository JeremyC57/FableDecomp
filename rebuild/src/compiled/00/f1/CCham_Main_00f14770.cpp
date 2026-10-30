// CCham::Main() -- arena Cham (CQ_ArenaScript): unset Me as an opinion source, put Me in FACTION_HERO, make Me unkillable
//   and keep him out of the combo multiplier, wait until the arena sets ChamLeaving, take Me under scripted control
//   (priority 4, retried each frame), walk to GuardingDoorMarkerLeft and remove Me once the walk is done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F14770, 517 bytes (528 with padding); entity-script vtable slot 1 (Main).
// Callees: CBaseObject ctor 0x0099A380 / dtor 0x0099A430, scripted-thing handle (vtable 0x0127094C) IsValid 0x00CD23B9 /
//   Release 0x00CD2770 / MoveToPosition 0x007E72F0 (slot 4) / IsPerformingScriptTask 0x007E7450 (slot 26) / dtor 0x007E74D0,
//   CScriptThing dtor 0x004AA840 / slot 6 GetPos, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0, operator delete 0x00BFE9BC,
//   CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity,
//   72 GetThingWithScriptName, 108 RemoveThing, 517 EntitySetAsKillable, 526 EntitySetAsToAddToComboMultiplierWhenHit,
//   601 EntitySetInFaction, 623 EntityUnsetAsOpinionSource. Literals "FACTION_HERO" 0x0126C3A4, "GuardingDoorMarkerLeft" 0x012F8FD0.
// Layout: Ego_r.pdb CCham (28 bytes, the CQ_ArenaScript one): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18; CQ_ArenaScript::ChamLeaving PDB +0xcc = retail +0xb4 (CScriptBase is 0x14 smaller in retail).
// Shape: the inline-once destructor band (landed 0x00EDD060 idiom): the first wait loop's terminating return expands the
//   handle's generated destructor, every other exit calls the out-of-line copy 0x007E74D0 (tail-merged with the normal end);
//   the handle is a destructor-less view destroyed by hand per exit.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                // CCountedPointer's shared block
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4
    void* Data;                                        // +8
};
struct CCountedPointerWords {                          // CCountedPointer<T> {Data, Info}
    void*           Data;
    CCPPointerInfo* Info;
    __forceinline void ReleaseInline()                 // ~CCountedPointer body
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
struct CBase {
    virtual ~CBase();                                  // 0x0099A2E0
};
class CScriptThing : public CBase { public:                          // vtable 0x01238C8C, 12 bytes
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;           // slot 6 (+0x18)
    CCountedPointerWords PImp;                         // +0x04
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CBaseObject { public:                                   // vtable 0x0129A7D8 {vptr, intelligent-pointer link}
    virtual void b000();
    void* IntelligentPointer;                          // +0x04
protected:
    CBaseObject();                                     // 0x0099A380
public:
};
struct CBaseObjectDtor {                               // the same root, as its destructor sees it
    void* IntelligentPointer;
    virtual ~CBaseObjectDtor();                        // 0x0099A430
};
struct CScriptGameResourceObjectBaseDtor : CBaseObjectDtor {   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBaseDtor() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 16 bytes
    CCountedPointerWords PImp;                         // +0x08
    CScriptGameResourceObjectScriptedThingBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void s001(); virtual void s002(); virtual void s003();
    virtual void MoveToPosition(const C3DVector& pos, float speed, EScriptEntityMoveType type, bool a, bool b);   // slot 4 (+0x10), 0x007E72F0
    virtual void s005(); virtual void s006(); virtual void s007(); virtual void s008(); virtual void s009(); virtual void s010(); virtual void s011(); virtual void s012();
    virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019(); virtual void s020();
    virtual void s021(); virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025();
    virtual bool IsPerformingScriptTask() const;       // slot 26 (+0x68), 0x007E7450
    bool IsValid() const;                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    void Destroy();                                    // ~CScriptGameResourceObjectScriptedThingBase, out of line 0x007E74D0
    __forceinline void DestroyInline()                 // the compiler-generated destructor: member release, then the base
    {
        PImp.ReleaseInline();
        reinterpret_cast<CScriptGameResourceObjectBaseDtor*>(this)->CScriptGameResourceObjectBaseDtor::~CScriptGameResourceObjectBaseDtor();
    }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CQ_ArenaScript {
    char _pad[0xb4];
    bool ChamLeaving;                                  // +0xb4
};
struct CCham : CEntityScriptBase {
    CQ_ArenaScript* ParentClass;                       // +0x14
    void*           PMasterData;                       // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};

void CCham::Main()
{
    CScriptGameResourceObjectScriptedThingBase scripted;
    Game->EntityUnsetAsOpinionSource(Me, false);
    Game->EntitySetInFaction(Me, "FACTION_HERO");
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    while (!ParentClass->ChamLeaving) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) {
            scripted.DestroyInline();
            return;
        }
    }
    if (IsActiveThreadTerminating()) {
        scripted.Destroy();
        return;
    }
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) {
            scripted.Destroy();
            return;
        }
    }
    if (IsActiveThreadTerminating()) {
        scripted.Destroy();
        return;
    }
    scripted.MoveToPosition(Game->GetThingWithScriptName("GuardingDoorMarkerLeft").GetPos(), 1.0f, (EScriptEntityMoveType)1, false, true);
    while (scripted.IsPerformingScriptTask()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) {
            scripted.Destroy();
            return;
        }
    }
    if (IsActiveThreadTerminating()) {
        scripted.Destroy();
        return;
    }
    Game->RemoveThing(Me, false, true);
    scripted.Destroy();
}