// NScript::CQ_GuildTrainingScript::CRaceMarker::Main() -- guild-training race marker script: each frame, once the hero comes within 3 units of the marker (and the script is still running) flag the parent quest's platform as reached and remove the marker from the minimap
// Leaf: ?Main@CRaceMarker@CQ_GuildTrainingScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D40AA0, length 97 bytes.
// Callees: IsDone @ 0x00F35B30 (the entity script's IsDone; catalog label propagated), IsDistanceBetweenThingsUnder
// (const CScriptThing&, const CScriptThing&, float) @ 0x00CBE2FF (`__fastcall`); CGameScriptInterface slots 70 GetHero,
// 352 MiniMapRemoveMarker (0x00891FF0), 7 NewScriptFrame.
// Layout = Ego_r.pdb CRaceMarker (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass
// +0x14 (CQ_GuildTrainingScript; retail quest scripts sit 0x14 below the PDB, so +0x4d is ReachedPlatform, PDB
// +0x61), PMasterData +0x18. Interface model from the landed CHTG_AmbushPoint::Main 0x00D73610.
// Shape: `while (!IsActiveThreadTerminating())` rotated into a bottom test after an entry test; the inner IsDone check after the
// distance test leaves the loop.
// Naming wave 35: identity reg 00d3c0ef "RaceMarker" in 00d3bc60; Alloc 00d409e0; vtable 012cdf44 slot 1; PDB ?Main@CRaceMarker@CQ_GuildTrainingScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CRaceMarker::Main (wave3/labels_alloc.tsv); PDB ?Main@CRaceMarker@CQ_GuildTrainingScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CRaceMarker@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic); outer stand-in CQ_GuildTrainingScript moved into NScript); same bytes.
namespace NScript { class CPredicateBase; }
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_GuildTrainingScript {
public:
    unsigned char _pad[0x4d];
    bool          ReachedPlatform;                 // +0x4d (PDB +0x61)
public:
    class CRaceMarker;               // the entity class below (naming wave 35)
};
}
using NScript::CQ_GuildTrainingScript;
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

namespace NScript {
class CQ_GuildTrainingScript::CRaceMarker : public CEntityScriptBase {
public:
        CGameScriptInterface*   Game;                  // +0x04
    CScriptThing            Me;                    // +0x08
    CQ_GuildTrainingScript* ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
private:
    virtual void Main();                          // PDB ?Main@CRaceMarker@CQ_GuildTrainingScript@NScript@@EAEXXZ
};
}

void NScript::CQ_GuildTrainingScript::CRaceMarker::Main()
{
    while (!IsActiveThreadTerminating()) {
        if (IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 3.0f)) {
            if (IsActiveThreadTerminating())
                break;
            ParentClass->ReachedPlatform = true;
            Game->MiniMapRemoveMarker(Me);
        }
        Game->NewScriptFrame();
    }
}