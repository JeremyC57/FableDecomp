// CHSB_MagicBarrier::Main() -- Briar Rose's magic barrier in Hero Souls: register a thing-alive filter, raise the two forcefield idle effects facing the shrine centre, re-raise them whenever both have gone until Briar is killed, then remove them, play the two "off" effects facing the centre and remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D81C50, 1464 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840 / operator= 0x008AB980 (inline copies in the loop), ~CBaseClass 0x0099A2E0,
//   operator delete 0x00BFE9BC, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 /
//   0x0099EAE0; GameCalls slots 7, 72, 100 CreateEffect, 108 RemoveThing, 475 EntitySetFacingAngleTowardsThing; CScriptThing
//   slots 6 GetPos, 10 GetAngleXY, 75 IsAlive (inline through PImp), 76 IsDead (inline, IsAlive through the vtable).
// Layout: Ego_r.pdb CHSB_MagicBarrier (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18
//   (CQ_SunnyvaleMasterData: KilledBriar +0x65, as the PDB).
// Shape: ~CScriptThing and operator= are plain inline bodies (they expand in the loop re-raise; operator= reads the source Info and
//   Data into locals first); the filter holds an out-of-line-copied thing; inline_depth(0) on the two
//   "off" assignments leaves those and every exit out of line.
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
    CCountedPointer& operator=(const CCountedPointer& other)
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
struct CScriptThingImpl {                              // the game-side thing behind CScriptThing::PImp
    virtual void i000() const; virtual void i001() const; virtual void i002() const; virtual void i003() const; virtual void i004() const; virtual void i005() const; virtual void i006() const; virtual void i007() const;
    virtual void i008() const; virtual void i009() const; virtual void i010() const; virtual void i011() const; virtual void i012() const; virtual void i013() const; virtual void i014() const; virtual void i015() const;
    virtual void i016() const; virtual void i017() const; virtual void i018() const; virtual void i019() const; virtual void i020() const; virtual void i021() const; virtual void i022() const; virtual void i023() const;
    virtual void i024() const; virtual void i025() const; virtual void i026() const; virtual void i027() const; virtual void i028() const; virtual void i029() const; virtual void i030() const; virtual void i031() const;
    virtual void i032() const; virtual void i033() const; virtual void i034() const; virtual void i035() const; virtual void i036() const; virtual void i037() const; virtual void i038() const; virtual void i039() const;
    virtual void i040() const; virtual void i041() const; virtual void i042() const; virtual void i043() const; virtual void i044() const; virtual void i045() const; virtual void i046() const; virtual void i047() const;
    virtual void i048() const; virtual void i049() const; virtual void i050() const; virtual void i051() const; virtual void i052() const; virtual void i053() const; virtual void i054() const; virtual void i055() const;
    virtual void i056() const; virtual void i057() const; virtual void i058() const; virtual void i059() const; virtual void i060() const; virtual void i061() const; virtual void i062() const; virtual void i063() const;
    virtual void i064() const; virtual void i065() const; virtual void i066() const; virtual void i067() const; virtual void i068() const; virtual void i069() const; virtual void i070() const; virtual void i071() const;
    virtual void i072() const; virtual void i073() const; virtual void i074() const;
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class C3DVector { public: float X, Y, Z; };
class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const;
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    virtual void s007() const; virtual void s008() const; virtual void s009() const;
    virtual float GetAngleXY() const;   // slot 10 (+0x28)
    virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const;
    virtual void s019() const; virtual void s020() const; virtual void s021() const; virtual void s022() const; virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const;
    virtual void s027() const; virtual void s028() const; virtual void s029() const; virtual void s030() const; virtual void s031() const; virtual void s032() const; virtual void s033() const; virtual void s034() const;
    virtual void s035() const; virtual void s036() const; virtual void s037() const; virtual void s038() const; virtual void s039() const; virtual void s040() const; virtual void s041() const; virtual void s042() const;
    virtual void s043() const; virtual void s044() const; virtual void s045() const; virtual void s046() const; virtual void s047() const; virtual void s048() const; virtual void s049() const; virtual void s050() const;
    virtual void s051() const; virtual void s052() const; virtual void s053() const; virtual void s054() const; virtual void s055() const; virtual void s056() const; virtual void s057() const; virtual void s058() const;
    virtual void s059() const; virtual void s060() const; virtual void s061() const; virtual void s062() const; virtual void s063() const; virtual void s064() const; virtual void s065() const; virtual void s066() const;
    virtual void s067() const; virtual void s068() const; virtual void s069() const; virtual void s070() const; virtual void s071() const; virtual void s072() const; virtual void s073() const; virtual void s074() const;
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c)
    virtual bool IsDead() const { return !IsAlive(); }   // slot 76 (+0x130)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(), PImp(other.PImp) { }
    ~CScriptThing() { }                                 // out-of-line copy 0x004AA840
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }   // out-of-line copy 0x008AB980
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_SunnyvaleMasterData {
    char pad00[0x65];
    bool KilledBriar;                                   // +0x65
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CHSB_MagicBarrier : CEntityScriptBase {
    void* ParentClass;                                  // +0x14 CQ_HeroSoulsBriarScript*
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    void Main();
};

void CHSB_MagicBarrier::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing effect1 = Game->CreateEffect("BRIAR_ROSE_FORCEFIELD_IDLE_01", Me.GetPos(), "", Me.GetAngleXY(), false, false);
    CScriptThing effect2 = Game->CreateEffect("BRIAR_ROSE_FORCEFIELD_IDLE_02", Me.GetPos(), "", Me.GetAngleXY(), false, false);
    CScriptThing middle = Game->GetThingWithScriptName("HS_MiddleOfShrine");
    if (effect1.IsAlive() && effect2.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetFacingAngleTowardsThing(effect1, middle, true);
        Game->EntitySetFacingAngleTowardsThing(effect2, middle, true);
    }
    while (!PMasterData->KilledBriar) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (effect1.IsDead() && effect2.IsDead()) {
            if (IsActiveThreadTerminating())
                return;
            effect1 = Game->CreateEffect("BRIAR_ROSE_FORCEFIELD_IDLE_01", Me.GetPos(), "", Me.GetAngleXY(), false, false);
            effect2 = Game->CreateEffect("BRIAR_ROSE_FORCEFIELD_IDLE_02", Me.GetPos(), "", Me.GetAngleXY(), false, false);
            Game->EntitySetFacingAngleTowardsThing(effect1, middle, true);
            Game->EntitySetFacingAngleTowardsThing(effect2, middle, true);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(effect1, false, true);
    Game->RemoveThing(effect2, false, true);
#pragma inline_depth(0)
    effect1 = Game->CreateEffect("BRIAR_ROSE_FORCEFIELD_IDLE_01_OFF", Me.GetPos(), "", 0.0f, false, false);
    effect2 = Game->CreateEffect("BRIAR_ROSE_FORCEFIELD_IDLE_02_OFF", Me.GetPos(), "", 0.0f, false, false);
#pragma inline_depth()
    Game->EntitySetFacingAngleTowardsThing(effect1, middle, true);
    Game->EntitySetFacingAngleTowardsThing(effect2, middle, true);
    Game->RemoveThing(Me, false, true);
}