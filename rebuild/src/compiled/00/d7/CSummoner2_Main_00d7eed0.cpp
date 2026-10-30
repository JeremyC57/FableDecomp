// NScript::CQ_HeroSoulsArenaScript::CSummoner2::Main() -- Hero Souls arena summoner script: wait (one script frame at a time) until the hero hits this summoner (other than with special ability 14), then flag PlayerHitSummoner2 on the arena quest script
// Leaf: Main
// Identity (naming wave 11): ?Main@CSummoner2@CQ_HeroSoulsArenaScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012D3ECC slot 1 is stored by the factory 0x00D7EE00 registered as "Summoner2"
// Label: catalogued as a second CSummoner1::Main; the body stores PlayerHitSummoner2 and is referenced from the CSummoner2
// vtable (0x012D3ED0), so it is CSummoner2::Main (Ego_r.pdb CSummoner2, same 28-byte layout).
// Retail 0x00D7EED0, 336 bytes, `ret`. Vtable reference 0x012D3ED0.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90, the filter registration 0x00F35B10,
// CScriptThing::~CScriptThing 0x004AA840, IsDone 0x00F35B30, CGameScriptInterface slot 7 NewScriptFrame (+0x1c),
// CCharString::CCharString(const char*, long) 0x0099EBF0 ("SCRIPT_NAME_HERO" 0x0125D1C8), ~CCharString 0x0099EAE0,
// CScriptThing slots 21 MsgIsHitBy (+0x54), 41 MsgIsHitBySpecialAbilityFrom (+0xa4), 42 MsgIsHitByAnySpecialAbilityFrom (+0xa8).
// Layout = Ego_r.pdb CSummoner2 (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
// PMasterData +0x18; CQ_HeroSoulsArenaScript::PlayerHitSummoner2 retail +0x4E (PDB 0x62 - 0x14, as the landed arena rows).
// Shape: as the landed CHeroSoulsEnemy::Main 0x00D7F110 (scoped CIsThingAlive filter, NewScriptFrame, IsDone); the wait
// is a `while (!(hit))` loop whose condition builds three conditional CCharString temporaries (ebx construction flags,
// the condition parked in a frame byte); after the loop an IsDone test guards the quest flag store.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;
namespace NScript {
class CQ_HeroSoulsArenaScript {
public:
    unsigned char _pad00[0x4d];
    bool          PlayerHitSummoner1;                   // +0x4d
    bool          PlayerHitSummoner2;                   // +0x4e
public:
    class CSummoner2;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_HeroSoulsArenaScript;
struct CQ_SunnyvaleMasterData;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_HeroSoulsArenaScript::CSummoner2 : public CEntityScriptBase {
public:
        CQ_HeroSoulsArenaScript* ParentClass;               // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;               // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_HeroSoulsArenaScript::CSummoner2::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!(Me.MsgIsHitBy("SCRIPT_NAME_HERO")
             || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO")))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->PlayerHitSummoner2 = true;
}