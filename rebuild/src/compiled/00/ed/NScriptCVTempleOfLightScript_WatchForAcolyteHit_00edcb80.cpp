// CV_TempleOfLightScript::WatchForAcolyteHit(CScriptThing acolyte) -- with an is-alive predicate on the acolyte, every frame he is hit by the hero flag the attack and, the first time while the altar lives, have the altar speak TEXT_QST_064_WEAPON_02 to the hero
// Leaf: ?WatchForAcolyteHit@CV_TempleOfLightScript@NScript@@QAEXVCScriptThing@@@Z
// Label: (catalog says CV_ChapelOfEvilScript, a
//   propagated label: the body writes +0x5b/+0x5c = Ego_r CV_TempleOfLightScript AttackedByHero / AcolyteAttackMessageComplete
//   at PDB 111/112; ChapelOfEvil's pair sits at +0x5a/+0x5b. Neighbours 0x00EDBD80 WatchForInsideTemple, 0x00EDC720 CTOLAcolyteGood.)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDCB80, 0x2D3 bytes, `ret 0Ch` (the by-value CScriptThing).
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90, CScriptBase::SetPredicate 0x00CB7920, ~CIsThingAlive 0x00E241B0,
//   IsActiveThreadTerminating 0x00CB7940, CCharString 0x0099EBF0 / ~ 0x0099EAE0, ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC, ~CScriptThing 0x004AA840 (out of line at the in-branch exits); Game slots 7 NewScriptFrame, 70 GetHero,
//   72 GetThingWithScriptName, 364 AddNewConversation, 365 AddPersonToConversation, 366 AddLineToConversation; the thing's
//   slots 21 MsgIsHitBy, 43 MsgIsHitByAnyAggressiveSpecialAbilityFrom, 75 IsAlive behind the null-safe wrappers.
// Shape: the 0x00ED7320 WatchForDeathDuringSwordPull model (b18 by-value thing, inline_depth(0) in-branch exits).
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, AttackedByHero +0x5b, AcolyteAttackMessageComplete +0x5c.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); CScriptThing is class-keyed too (Ego_r ??0CScriptThing@@QAE@ABV0@@Z), which the Leaf line already
//   spelled (V); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
inline bool CScriptThing::MsgIsKilledBy(const CCharString& killer) const { if (!PImp.Data) return false; return PImp.Data->MsgIsKilledBy(killer); }
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
namespace NScript {
class __declspec(novtable) CPredicateBase { public:               // vtable 0x012C2FF4 (never stored: novtable)
    virtual bool IsTrue() = 0;
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:            // vtable 0x012C32F0, 16 bytes
    CScriptThing Thing;                                     // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}
    virtual ~CIsThingAlive();                                       // 0x00E241B0
    virtual bool IsTrue();
};
}
using NScript::CIsThingAlive;



// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_TempleOfLightScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* PMasterData;                                  // +0x44
    unsigned char _pad48[0x5b - 0x48];
    bool AttackedByHero;                                // +0x5b
    bool AcolyteAttackMessageComplete;                  // +0x5c
    void WatchForAcolyteHit(CScriptThing acolyte);
};
} // namespace NScript
using NScript::CV_TempleOfLightScript;

void NScript::CV_TempleOfLightScript::WatchForAcolyteHit(CScriptThing acolyte)
{
    SetPredicate(CIsThingAlive(acolyte));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing altar = Game->GetThingWithScriptName("M_SoundAltar");
    AcolyteAttackMessageComplete = false;
    while (!IsActiveThreadTerminating()) {
        if (acolyte.MsgIsHitBy("SCRIPT_NAME_HERO") || acolyte.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            AttackedByHero = true;
            if (!AcolyteAttackMessageComplete && altar.IsAlive()) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                long conversation = Game->AddNewConversation(altar, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_064_WEAPON_02", false, altar, Game->GetHero());
                AcolyteAttackMessageComplete = true;
            }
        }
        Game->NewScriptFrame();
    }
}