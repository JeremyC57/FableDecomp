// NScript::CV_TravellingHeroesScript::CTH_Trigger_Oakvale::Main() -- Travelling Heroes Oakvale trigger: register an alive-filter over Me, start a script frame, make Me persistent; between frames 900 and 2300 of the master data's clock, unless the whisper is already handled, spawn the rival hero's whisper creature at Me (script "TH_Whisper"); then remove Me
// Leaf: Main
// Identity (naming wave 11): ?Main@CTH_Trigger_Oakvale@CV_TravellingHeroesScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012F4364 slot 1 is stored by the factory 0x00EE0930 registered as "TH_Trigger_Oakvale"
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE0A20, 307 bytes (+13 int3), `ret`.
// Identity: the factory record at 0x00EDF0C0 ("TH_Trigger_Oakvale" @ 0x012F40F4, creator 0x00EE0930) builds the object
//   whose vtable 0x012F4364 has this Main in slot 1 (its sibling 0x00EE06B0 is CTH_Trigger_Oakvale_Gossip::Main).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, the filter registration @ 0x00F35B10, the
//   filter's out-of-line destructor @ 0x00E241B0, IsDone @ 0x00F35B30, CScriptThing (vtable 0x01238C8C) slot 6 GetPos,
//   ~CScriptThing @ 0x004AA840, CCharString(const char*, long) @ 0x0099EBF0 ("TH_Whisper" @ 0x012F4160,
//   "CREATURE_RIVAL_HERO_WHISPER" @ 0x012F4380), ~CCharString @ 0x0099EAE0; CGameScriptInterface slots 7 NewScriptFrame,
//   91 CreateCreature(const CCharString&, const C3DVector&, const CCharString&, bool) (CScriptThing by value), 108
//   RemoveThing(const CScriptThing&, bool, bool), 477 SetThingPersistent(const CScriptThing&, bool).
// Layout = Ego_r.pdb CTH_Trigger_Oakvale (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass
//   +0x14 (CV_TravellingHeroesScript*), PMasterData +0x18 (a frame clock at +4 and a done flag at +0x74, unnamed).
// Shape: the gossip trigger's body (0x00EE06B0) with the creature spawn in place of the gossip set-up: the two literal
//   names are implicit CCharString conversions built before the other arguments (right to left), the created creature a
//   discarded temporary destroyed first; the handled arm keeps its own RemoveThing + return.
namespace NScript { class CPredicateBase; }
class C3DVector { public: float X, Y, Z; };
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
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
struct CTravellingHeroesMasterData {
    unsigned char _pad00[4];
    long          Clock;                           // +0x04 (unnamed)
    unsigned char _pad08[0x74 - 0x08];
    bool          WhisperHandled;                  // +0x74 (unnamed)
};
namespace NScript {
class CV_TravellingHeroesScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CTH_Trigger_Oakvale;                        // the entity class below (naming wave 11)
};
}
using NScript::CV_TravellingHeroesScript;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CV_TravellingHeroesScript::CTH_Trigger_Oakvale : public CEntityScriptBase {
public:
        CV_TravellingHeroesScript* ParentClass;        // +0x14
    CTravellingHeroesMasterData* PMasterData;      // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CV_TravellingHeroesScript::CTH_Trigger_Oakvale::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingPersistent(Me, true);
    if (PMasterData->Clock > 900 && PMasterData->Clock < 2300) {
        if (IsActiveThreadTerminating())
            return;
        if (PMasterData->WhisperHandled) {
            if (IsActiveThreadTerminating())
                return;
            Game->RemoveThing(Me, false, true);
            return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->CreateCreature("CREATURE_RIVAL_HERO_WHISPER", Me.GetPos(), "TH_Whisper", false);
        Game->RemoveThing(Me, false, true);
    }
}