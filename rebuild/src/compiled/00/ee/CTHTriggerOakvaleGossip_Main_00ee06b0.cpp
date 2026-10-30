// NScript::CV_TravellingHeroesScript::CTH_Trigger_Oakvale_Gossip::Main() -- Travelling Heroes Oakvale gossip trigger: register an alive-filter over Me, start a script frame, make Me persistent; between frames 900 and 2300 of the master data's clock, unless the whisper is already handled, set up the Oakvale guild whisper gossip; then remove Me
// Leaf: Main
// Identity (naming wave 11): ?Main@CTH_Trigger_Oakvale_Gossip@CV_TravellingHeroesScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012F4304 slot 1 is stored by the factory 0x00EE05C0 registered as "TH_Trigger_Oakvale_Gossip"
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE06B0, 298 bytes (+6 int3), `ret`.
// Label correction: the catalog row says CTH_Trigger_Oakvale::Main; the factory record at 0x00EDF119 (`push 1Ch`, the
//   name "TH_Trigger_Oakvale_Gossip" @ 0x012F40D8, creator 0x00EE05C0) builds the object whose vtable 0x012F4304 has this
//   Main in slot 1, so the class is CTH_Trigger_Oakvale_Gossip (CTH_Trigger_Oakvale's Main is 0x00EE0A20).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, the filter registration @ 0x00F35B10, the
//   filter's out-of-line destructor @ 0x00E241B0, IsDone @ 0x00F35B30 (five times); CGameScriptInterface slots 7
//   NewScriptFrame, 477 SetThingPersistent(const CScriptThing&, bool) (vtable 0x01260F0C slot 477 = 0x0088E5A0) and 108
//   RemoveThing(const CScriptThing&, bool, bool); CV_TravellingHeroesScript::GossipSetup(CCharString, CCharString,
//   CCharString, CCharString) @ 0x00EE07E0 (four by-value strings built in their argument slots with CCharString(const
//   char*, long) @ 0x0099EBF0: "TravHero Whisper", "TEXT_AI_GOSSIP_WHISPER_OAKVALE", "VILLAGE_GUILD_COMPLEX_INSIDE",
//   "FACTION_GUILD").
// Layout = Ego_r.pdb CTH_Trigger_Oakvale(_Gossip) (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8},
//   ParentClass +0x14 (CV_TravellingHeroesScript*; WhisperCutscenePlayed at retail +0x7C = PDB +0x90 - 0x14), PMasterData
//   +0x18 (CQ_* master data: a frame clock at +4 and a done flag at +0x74, unnamed).
// Shape: the landed CHHUndead::Main 0x00E95E80 opening; every step re-tests IsActiveThreadTerminating() with a plain return; the three
//   RemoveThing tails are written per branch (VC7.1 merges the first and last, the middle keeps its own copy).
namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
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
struct CTravellingHeroesMasterData {
    unsigned char _pad00[4];
    long          Clock;                           // +0x04 (unnamed)
    unsigned char _pad08[0x74 - 0x08];
    bool          WhisperHandled;                  // +0x74 (unnamed)
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
namespace NScript {
class CV_TravellingHeroesScript {
public:
    unsigned char _pad00[0x7c];
    bool          WhisperCutscenePlayed;           // +0x7c
    void GossipSetup(CCharString name, CCharString text, CCharString region, CCharString faction);   // 0x00EE07E0
public:
    class CTH_Trigger_Oakvale_Gossip;                        // the entity class below (naming wave 11)
};
}
using NScript::CV_TravellingHeroesScript;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CV_TravellingHeroesScript::CTH_Trigger_Oakvale_Gossip : public CEntityScriptBase {
public:
        CV_TravellingHeroesScript* ParentClass;        // +0x14
    CTravellingHeroesMasterData* PMasterData;      // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CV_TravellingHeroesScript::CTH_Trigger_Oakvale_Gossip::Main()
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
        } else if (ParentClass->WhisperCutscenePlayed) {
            if (IsActiveThreadTerminating())
                return;
            Game->RemoveThing(Me, false, true);
        } else {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->GossipSetup("TravHero Whisper", "TEXT_AI_GOSSIP_WHISPER_OAKVALE", "VILLAGE_GUILD_COMPLEX_INSIDE",
                                     "FACTION_GUILD");
            Game->RemoveThing(Me, false, true);
        }
    }
}