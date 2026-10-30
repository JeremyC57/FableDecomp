// CRaceMarker::Main() -- guild-training race marker script: each frame, once the hero comes within 3 units of the marker (and the script is still running) flag the parent quest's platform as reached and remove the marker from the minimap
// Leaf: Main@CRaceMarker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D40AA0, length 97 bytes.
// Callees: IsDone @ 0x00F35B30 (the entity script's IsDone; catalog label propagated), IsDistanceBetweenThingsUnder
// (const CScriptThing&, const CScriptThing&, float) @ 0x00CBE2FF (`__fastcall`); CGameScriptInterface slots 70 GetHero,
// 352 MiniMapRemoveMarker (0x00891FF0), 7 NewScriptFrame.
// Layout = Ego_r.pdb CRaceMarker (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass
// +0x14 (CQ_GuildTrainingScript; retail quest scripts sit 0x14 below the PDB, so +0x4d is ReachedPlatform, PDB
// +0x61), PMasterData +0x18. Interface model from the landed CHTG_AmbushPoint::Main 0x00D73610.
// Shape: `while (!IsActiveThreadTerminating())` rotated into a bottom test after an entry test; the inner IsDone check after the
// distance test leaves the loop.
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                              // vtable 0x01238C8C, 12 bytes
    void* __vftable;
    unsigned char _body[8];
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_GuildTrainingScript {
    unsigned char _pad[0x4d];
    bool          ReachedPlatform;                 // +0x4d (PDB +0x61)
};
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CRaceMarker : public CEntityScriptBase {
public:
    void*                   __vftable;             // +0x00
    CGameScriptInterface*   Game;                  // +0x04
    CScriptThing            Me;                    // +0x08
    CQ_GuildTrainingScript* ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    void Main();
};

void CRaceMarker::Main()
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