// NScript::CQ_SecretPassageScript::CSP_SecretPassageGuard::Main() -- the secret-passage guard (first variant): register an alive-filter over Me, take Me under scripted control, and until the hero is seen or heard now and then mutter a line while the hero is within 30 units; then shout the spotted line and set the hero as its best enemy target
// Leaf: ?Main@CSP_SecretPassageGuard@CQ_SecretPassageScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DED810, length 639 bytes, plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0) over CScriptThing copy ctor 0x004ABE90 / ~CScriptThing 0x004AA840,
// RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30; scripted-thing handle (vtable 0x0127094C) inline
// ctor over CBaseObject 0x0099A380, IsValid 0x00CD23B9, Release 0x00CD2770,,
// dtor 0x007E74D0; IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall); rand 0x00BFEB16; CCharString 0x0099EBF0 /
// 0x0099EAE0; CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 364 AddNewConversation,
// 366 AddLineToConversation, 451 GiveThingBestEnemyTarget, 632 CanThingBe_Seen_ByOtherThing, 635
// CanThingBe_Heard_ByOtherThing. Literals "TEXT_QST_055_GUARD_MOANING_ABOUT_STUFF" 0x012DC424,
// "TEXT_QST_055_GUARD_SURPRISED_BY_HERO" 0x012DC3FC.
// Layout: Ego_r.pdb CSP_SecretPassageGuard (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
// PMasterData +0x18 (neither used here).
// Shape: the sibling CSP_InvestigatingGuard::Main 0x00DEDB80; the landed CHTE_Audience::Main 0x00D6B240 handle (inline ctor, out-of-line dtor at every exit) and the
// CRockTrollTrigger::Main 0x00E0AD40 scoped filter.
// Naming wave 35: identity reg 00decbe8 "SP_SecretPassageGuard" in 00decad0; Alloc 00ded720; vtable 012dddc8 slot 1; PDB ?Main@CSP_SecretPassageGuard@CQ_SecretPassageScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CSP_SecretPassageGuard::Main (wave3/labels_alloc.tsv); PDB ?Main@CSP_SecretPassageGuard@CQ_SecretPassageScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CSP_SecretPassageGuard@@QAEXXZ`; this source now defines the PDB member (class model unchanged); same bytes.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
#include "engine/C3DVector.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
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
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern "C" int __cdecl rand();                         // 0x00BFEB16
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CEntityScriptBase { public:
    virtual void e00();
    class CGameScriptInterface* Game;                 // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_SecretPassageScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CSP_SecretPassageGuard;
};
}
using NScript::CQ_SecretPassageScript;
namespace NScript {
class CQ_SecretPassageScript::CSP_SecretPassageGuard : public CEntityScriptBase {
public:
    void* ParentClass;                                 // +0x14 CQ_SecretPassageScript*
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
private:
    virtual void Main();                          // PDB ?Main@CSP_SecretPassageGuard@CQ_SecretPassageScript@NScript@@EAEXXZ
};
}

void NScript::CQ_SecretPassageScript::CSP_SecretPassageGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, (EScriptAIPriority)3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(), Me) && !Game->CanThingBe_Heard_ByOtherThing(Game->GetHero(), Me)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 30.0f) && rand() % 250 == 0) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddLineToConversation(conversation, "TEXT_QST_055_GUARD_MOANING_ABOUT_STUFF", false, Me, Game->GetHero());
        }
    }
    if (IsActiveThreadTerminating())
        return;
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddLineToConversation(conversation, "TEXT_QST_055_GUARD_SURPRISED_BY_HERO", false, Me, Game->GetHero());
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    if (handle.IsValid())
        handle.Release();
}