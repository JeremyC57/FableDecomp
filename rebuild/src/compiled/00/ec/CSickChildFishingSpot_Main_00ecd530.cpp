// NScript::CV_SickChildScript::CSickChildFishingSpot::Main() -- sick-child quest fishing-spot entity: register an alive-filter over Me, start a frame, then wait frame by frame while the fishing spot stays enabled; once it is disabled (and the script is not done) set the parent's GotFishingSpotMushroom flag
// Leaf: Main
// Identity (naming wave 11): ?Main@CSickChildFishingSpot@CV_SickChildScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012F2420 slot 1 is stored by the factory 0x00ECD450 registered as "SickChildFishingSpot"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ECD530, length 145 bytes (slot of the entity vtable at 0x012F2424 references it).
// Callees: CScriptThing copy ctor @ 0x004ABE90, the filter registration @ 0x00F35B10, ~CIsThingAlive @ 0x00E241B0 (out of line in
// this module), IsDone @ 0x00F35B30; CGameScriptInterface slots 7 NewScriptFrame and 243 IsFishingSpotEnabled (0x008958F0).
// Layout = Ego_r.pdb CSickChildFishingSpot (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18;
// the parent's flag byte is at +0x69 in retail (PDB CV_SickChildScript::GotFishingSpotMushroom +0x7d).
// Shape: the landed CHHUndead::Main 0x00E95E80 prelude; the first IsDone test is explicit and the second is the rotated
// `while (!IsActiveThreadTerminating())` entry test; the disabled-spot exit re-tests IsDone before the flag store and returns.
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
namespace NScript {
class CV_SickChildScript {
public:
    unsigned char _pad[0x69];
    bool          GotFishingSpotMushroom;          // +0x69 (PDB +0x7d; retail script base is 0x14 smaller)
public:
    class CSickChildFishingSpot;                        // the entity class below (naming wave 11)
};
}
using NScript::CV_SickChildScript;
struct CQ_SunnyvaleMasterData;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CV_SickChildScript::CSickChildFishingSpot : public CEntityScriptBase {
public:
        CV_SickChildScript*     ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CV_SickChildScript::CSickChildFishingSpot::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        if (!Game->IsFishingSpotEnabled(Me)) {
            if (!IsActiveThreadTerminating())
                ParentClass->GotFishingSpotMushroom = true;
            return;
        }
        Game->NewScriptFrame();
    }
}