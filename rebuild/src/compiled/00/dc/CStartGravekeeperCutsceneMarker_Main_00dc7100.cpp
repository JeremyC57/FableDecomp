// CStartGravekeeperCutsceneMarker::Main() -- graveyard secret passage marker script: hold a conscious-thing predicate, then wait each frame until the hero comes within 5.0f of the marker and flag GravekeeperCutsceneStart on the parent quest
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC7100, 184 bytes (+ int3 padding); entity-script vtable 0x012DA890 slot 1 (Main).
// Leaf: Main
// Identity: the factory 0x00DC7020 ("StartGravekeeperCutsceneMarker" at 0x012DA298, `push 1Ch` = Ego_r.pdb sizeof CStartGravekeeperCutsceneMarker 28) stores this vtable;
//   CStartGravekeeperCutsceneMarker {ParentClass +0x14 = CQ_OpeningGraveyardSecretPassageScript*, PMasterData +0x18}.
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840, CEntityScriptBase::SetPredicate 0x00F35B10,
//   CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF (`__fastcall`, the
//   distance on the stack), and the game interface (+4) NewScriptFrame (slot 7) / GetHero (slot 70).
// Shape: the CIsThingConscious predicate (vtable 0x012C2FE8) is a scoped frame object (ctor = vtable + CScriptThing copy,
//   inline dtor = vtable store + ~CScriptThing). The wait is the scripted `while (!cond) { yield; if (terminating) return; }`
//   after one leading yield; the `push 5.0f` before the GetHero virtual call is IsDistanceBetweenThingsUnder's stack argument
//   (the virtual takes none). The flag store sits behind a final terminating test.
// Retail layout: GravekeeperCutsceneStart +0x4e in CQ_OpeningGraveyardSecretPassageScript (Ego_r.pdb 0x62, retail = PDB - 0x14, as in the landed
//   InitializeVariables 0x00DC3CF0).
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                                    // 12 bytes, vtable 0x01238C8C
    unsigned char _body[0xc - 4];
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    virtual ~CScriptThing();                                     // 0x004AA840
};

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

namespace NScript {
class CPredicateBase
{
public:
    virtual bool operator()() const;                     // slot 0 (0x00CDDAB0)
};
}
struct CIsThingConscious : public NScript::CPredicateBase {                               // NScript predicate, vtable 0x012C2FE8: Thing alive and not unconscious
    CScriptThing Thing;                                  // +0x04
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) {}
    ~CIsThingConscious() {}
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CQ_OpeningGraveyardSecretPassageScript {
    unsigned char _pad[0x4e];
    bool          GravekeeperCutsceneStart;          // +0x4e (PDB 0x62)
};
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase { public:                               // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing          Me;                            // +0x08
    bool IsActiveThreadTerminating() const;              // 0x00F35B30 (PDB: const)
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

struct CStartGravekeeperCutsceneMarker : CEntityScriptBase {
    CQ_OpeningGraveyardSecretPassageScript* ParentClass; // +0x14
    CQ_SunnyvaleMasterData*                 PMasterData; // +0x18
    void Main();
};

void CStartGravekeeperCutsceneMarker::Main()
{
    {
        CIsThingConscious predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 5.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->GravekeeperCutsceneStart = true;
}