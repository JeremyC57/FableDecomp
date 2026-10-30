// NScript::CPersonalScript_GlobalThingsScript::CKG_Gate::Main() -- gate entity script: register the alive-filter over Me, run a script frame and, unless done, open the gate once either hero-souls quest ("Q_HeroSoulsThunder" / "Q_HeroSoulsArena") is completed
// Leaf: Main
// Identity (naming wave 11): ?Main@CKG_Gate@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012C32FC slot 1 is stored by the factory 0x00CDF190 registered as "KG_Gate"
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDF260, 250 bytes (+ int3 padding). Entity-script slot 1 (Main) per docs/engine/QUEST_SCRIPTS.md.
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 (the CIsThingAlive filter, vtable 0x012C32F0), the filter
//   registration 0x00F35B10 and IsDone 0x00F35B30 on `this` (manifest labels propagated: AllocateMapThingUID / Initialise),
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CGameScriptInterface slots 7 NewScriptFrame (+0x1c),
//   286 IsQuestCompleted (+0x478), 425 OpenDoor (+0x6a4) (slot names from the CGameScriptInterface vtable 0x01260F0C).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8} (the landed CHTE_Prisoner model).
// Shape: the `a || b` of two quest tests keeps both string temporaries to the end of the full expression (ebx construction
//   flags 1/3, reverse-order destruction); the quest names are IMPLICIT CCharString conversions -- an explicit
//   `CCharString("...")` argument pushes the ctor's eax and hoists the vtable load (GOTCHAS chunk 214).
namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
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
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CPersonalScript_GlobalThingsScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CKG_Gate;                        // the entity class below (naming wave 11)
};
}
using NScript::CPersonalScript_GlobalThingsScript;
namespace NScript {
class CPersonalScript_GlobalThingsScript::CKG_Gate : public CEntityScriptBase {
public:
private:
        virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CPersonalScript_GlobalThingsScript::CKG_Gate::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    bool open = Game->IsQuestCompleted("Q_HeroSoulsThunder")
             || Game->IsQuestCompleted("Q_HeroSoulsArena");
    if (open && !IsActiveThreadTerminating())
        Game->OpenDoor(Me);
}