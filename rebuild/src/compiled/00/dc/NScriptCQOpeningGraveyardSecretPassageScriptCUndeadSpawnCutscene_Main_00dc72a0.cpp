// NScript::CQ_OpeningGraveyardSecretPassageScript::CUndeadSpawnCutscene::Main() -- graveyard secret passage marker script: hold a conscious-thing predicate, then wait each frame until the hero comes within 8.0f of the marker and flag UndeadCutsceneStarted on the parent quest
// Leaf: ?Main@CUndeadSpawnCutscene@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Plan phase 4 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line, as retail calls them here). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC72A0, 184 bytes (+ int3 padding); entity-script vtable 0x012DA8AC slot 1 (Main).
// Identity: the factory 0x00DC71C0 ("UndeadSpawnCutscene" at 0x012DA284, `push 1Ch` = Ego_r.pdb sizeof CUndeadSpawnCutscene 28) stores this vtable;
//   CUndeadSpawnCutscene {ParentClass +0x14 = CQ_OpeningGraveyardSecretPassageScript*, PMasterData +0x18}. The catalog label
//   "CStartGravekeeperCutsceneMarker::Main" is copied from the sibling 0x00DC7100; the factory record names this one.
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840, CEntityScriptBase::SetPredicate 0x00F35B10,
//   CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF (`__fastcall`, the
//   distance on the stack), and the game interface (+4) NewScriptFrame (slot 7) / GetHero (slot 70).
// Shape: the CIsThingConscious predicate (vtable 0x012C2FE8) is a scoped frame object (ctor = vtable + CScriptThing copy,
//   inline dtor = vtable store + ~CScriptThing). The wait is the scripted `while (!cond) { yield; if (terminating) return; }`
//   after one leading yield; the `push 8.0f` before the GetHero virtual call is IsDistanceBetweenThingsUnder's stack argument
//   (the virtual takes none). The flag store sits behind a final terminating test.
// Retail layout: UndeadCutsceneStarted +0x50 in CQ_OpeningGraveyardSecretPassageScript (Ego_r.pdb 0x64, retail = PDB - 0x14, as in the landed
//   InitializeVariables 0x00DC3CF0).
// Naming wave 35: identity reg 00dc2c7e "UndeadSpawnCutscene" in 00dc27c0; Alloc 00dc71c0; vtable 012da8ac slot 1; PDB ?Main@CUndeadSpawnCutscene@CQ_OpeningGraveyardSecretPassageScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CUndeadSpawnCutscene::Main (wave3/labels_alloc.tsv); PDB ?Main@CUndeadSpawnCutscene@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CUndeadSpawnCutscene@@QAEXXZ`; this source now defines the PDB member (outer stand-in CQ_OpeningGraveyardSecretPassageScript moved into NScript); same bytes.
namespace NScript { class CPredicateBase; }
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

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

namespace NScript {
class CQ_OpeningGraveyardSecretPassageScript {
public:
    unsigned char _pad[0x50];
    bool          UndeadCutsceneStarted;          // +0x50 (PDB 0x64)
public:
    class CUndeadSpawnCutscene;      // the entity class below (naming wave 35)
};
}
using NScript::CQ_OpeningGraveyardSecretPassageScript;
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase { public:                               // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing          Me;                            // +0x08
    bool IsActiveThreadTerminating() const;              // 0x00F35B30 (PDB: const)
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

namespace NScript {
class CQ_OpeningGraveyardSecretPassageScript::CUndeadSpawnCutscene : public CEntityScriptBase {
public:
    CQ_OpeningGraveyardSecretPassageScript* ParentClass; // +0x14
    CQ_SunnyvaleMasterData*                 PMasterData; // +0x18
private:
    virtual void Main();                          // PDB ?Main@CUndeadSpawnCutscene@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
};
}

void NScript::CQ_OpeningGraveyardSecretPassageScript::CUndeadSpawnCutscene::Main()
{
    {
        CIsThingConscious predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 8.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->UndeadCutsceneStarted = true;
}