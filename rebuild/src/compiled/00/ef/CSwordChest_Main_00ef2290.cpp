// NScript::CPersonalScript_EmptyScript::CSwordChest::Main() -- sword-chest script: wait until the hero uses the chest, then, if "LovelyBigSword" is alive and not already carried by the hero, make the hero carry it in hand
// Leaf: Main
// Identity (naming wave 11): ?Main@CSwordChest@CPersonalScript_EmptyScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012F61C0 slot 1 is stored by the factory 0x00EF21B0 registered as "SwordChest"
// Retail 0x00EF2290, 400 bytes, `ret`.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90, the filter registration 0x00F35B10,
// ~CIsThingAlive 0x00E241B0 (out of line in this TU), IsDone 0x00F35B30, CCharString(const char*, long) 0x0099EBF0
// ("SCRIPT_NAME_HERO" 0x0125D1C8, "LovelyBigSword" 0x012F61DC), ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840;
// CScriptThing slot 20 MsgIsUsedBy (+0x50) on Me, and on the sword (inlined, devirtualised) slot 75 IsAlive (+0x12c) and
// slot 63 IsBeingCarriedBy (+0xfc) forwarded to PImp.Data; CGameScriptInterface slots 7 NewScriptFrame, 72
// GetThingWithScriptName (+0x120, by-value CScriptThing), 142 MakeHeroCarryItemInHand (+0x238).
// Layout = Ego_r.pdb CSwordChest (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
// PMasterData +0x18; CScriptThing per the landed script model (CBanditCronies::Main 0x00E03810: vptr, PImp +4/+8).
// Shape: `!Me.MsgIsUsedBy("...")` is a byte computed (`neg ; sbb ; inc`) before the string's dtor, the rotated wait loop
// repeats it; the sword test `IsAlive() && !IsBeingCarriedBy("...")` keeps a conditional-temporary flag zeroed at entry;
// the forwarding IsBeingCarriedBy needs `__forceinline virtual` (a plain inline virtual is called out of line).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline bool CScriptThing::IsBeingCarriedBy(const CCharString& name) const { return PImp.Data && PImp.Data->IsBeingCarriedBy(name); }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;
namespace NScript {
class CPersonalScript_EmptyScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CSwordChest;                        // the entity class below (naming wave 11)
};
}
using NScript::CPersonalScript_EmptyScript;
struct CEmptyData;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CPersonalScript_EmptyScript::CSwordChest : public CEntityScriptBase {
public:
        CPersonalScript_EmptyScript* ParentClass;           // +0x14
    CEmptyData*                  PMasterData;           // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CPersonalScript_EmptyScript::CSwordChest::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing sword = Game->GetThingWithScriptName("LovelyBigSword");
    if (sword.IsAlive() && !sword.IsBeingCarriedBy("SCRIPT_NAME_HERO"))
        Game->MakeHeroCarryItemInHand(sword, true, false);
}