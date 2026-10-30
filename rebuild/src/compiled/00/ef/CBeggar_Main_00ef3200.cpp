// NScript::CPersonalScript_EmptyScript::CBeggar::Main() -- the beggar entity: hold an alive filter over Me, then loop: wait until the hero talks to the beggar, sit him on the floor, wait until the hero talks to him again, stand him up
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF3200, 443 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy ctor @ 0x004ABE90, CEntityScriptBase::SetPredicate @ 0x00F35B10, ~CIsThingAlive @ 0x00E241B0
//          (out of line in this module), CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30, CCharString(const char*, long)
//          @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0; CScriptThing slot 27 MsgIsTalkedToBy (0x004AABD0); CGameScriptInterface
//          slots 7 NewScriptFrame and 493 EntitySetAsSittingOnFloor (0x008A8C60, ghidra_out/cgsi_slots_aligned.tsv).
//          Literal "SCRIPT_NAME_HERO" @ 0x0125D1C8.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8 (12-byte CScriptThing, vtable 0x01238C8C)} as in the landed entity Mains
//         (CSickChildFishingSpot::Main 0x00ECD530); the CIsThingAlive filter (vtable 0x012C32F0) holds a CScriptThing copy.
// Shape: the landed alive-filter prelude, then `while (!IsActiveThreadTerminating())` around two scripted
//        WAIT_UNTIL(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) loops written as plain `while (!x)` (retail's neg/sbb/inc bool),
//        each followed by a terminating test and the sit/stand call.
// Leaf: Main
// Identity (naming wave 11): ?Main@CBeggar@CPersonalScript_EmptyScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012F636C slot 1 is stored by the factory 0x00EF3120 registered as "Beggar"
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
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
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CPersonalScript_EmptyScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CBeggar;                        // the entity class below (naming wave 11)
};
}
using NScript::CPersonalScript_EmptyScript;
namespace NScript {
class CPersonalScript_EmptyScript::CBeggar : public CEntityScriptBase {
public:
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private
public:
};
}

void NScript::CPersonalScript_EmptyScript::CBeggar::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAsSittingOnFloor(Me, true);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAsSittingOnFloor(Me, false);
        Game->NewScriptFrame();
    }
}