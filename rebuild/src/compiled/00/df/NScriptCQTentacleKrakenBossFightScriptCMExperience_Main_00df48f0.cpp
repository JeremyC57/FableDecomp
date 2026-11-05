// NScript::CQ_TentacleKrakenBossFightScript::CM_Experience::Main() -- Kraken boss-fight experience marker: set a CIsThingConscious(Me) predicate, wait a frame at a time for the master data's MotherDodgingKraken flag, then wait while "TheKraken" is alive and drop an experience orb (PrisonExperienceForKillingKraken) 5 units above this marker
// Leaf: ?Main@CM_Experience@CQ_TentacleKrakenBossFightScript@NScript@@EAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF48F0, 348 bytes, plain `ret`.
// Identity: Ego_r.pdb ?Main@CM_Experience@CQ_TentacleKrakenBossFightScript@NScript@@EAEXXZ (private virtual; the PDB also has
//   ??0 / ?Alloc@ / ?GetMasterData@ / ?GetParentScript@ of CM_Experience); the body reads the Kraken script's master data
//   and names "TheKraken" (0x012DE55C).
// Callees: CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30 (engine/CEntityScriptBase.h);
//   CScriptThing copy ctor 0x004ABE90 / ~CScriptThing 0x004AA840; NScript::CIsThingConscious (vtable 0x012C2FE8,
//   engine/CIsThingConscious.h; ctor and dtor inline here); CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0; CGameScriptInterface slots 7 NewScriptFrame, 72 GetThingWithScriptName(const CCharString&), 103
//   CreateExperienceOrb(const C3DVector&, long) (engine/CGameScriptInterface.h); CScriptThing slot 6 GetPos (on Me) and the
//   inline IsAlive (0x004AB130 body, devirtualised on the local: PImp->IsAlive() through slot 75).
// Globals: pGlobalScriptDef 0x0143E90C (the established bound spelling; Ego_r.pdb names it
//   NGlobalScriptDefs::pGlobalScriptDef, a CDefPointer<CScriptDef> whose first word is read here); 5.0f 0x012C26D8.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8} (engine/CEntityScriptBase.h); Ego_r.pdb CM_Experience ParentClass +0x14,
//   PMasterData +0x18; CQ_SunnyvaleMasterData MotherDodgingKraken +0x84 (Ego_r.pdb +132); CScriptDef
//   PrisonExperienceForKillingKraken at retail +0xDE0 (Ego_r.pdb +0xE90; the landed CScriptDef ctor 0x00F28F90 has the
//   retail +0xDE0), so CScriptDef and the master data are local partial models (no shared headers exist).
// Shape: the predicate is a CIsThingConscious temporary passed to SetPredicate (inline ctor: vtable store, then the Thing
//   copy; inline dtor: vtable re-store, then ~CScriptThing). With the shared NScript headers (CIsThingConscious :
//   CPredicateBase, CEntityScriptBase's real Me member) VC7.1 keeps &Me in edi across the function as retail does; the
//   earlier local models held it in ebx. ~CScriptThing stays out of line (an inline empty dtor would expand the
//   CCountedPointer release). The script name converts implicitly so the CCharString is built before Game is loaded; the
//   orb position is `Me.GetPos() + C3DVector(0, 0, 5.0f)` (the `+ 0` terms fold: X copied as an integer).
class CThing;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
// CCharString: the engine string's data pointer and the two members used (no shared header with these spellings).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CGameScriptInterface.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CIsThingConscious.h"
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingConscious::~CIsThingConscious() { }
class CScriptDef { public:
    unsigned char _pad0[0xde0];
    long          PrisonExperienceForKillingKraken;    // +0xDE0 retail (Ego_r.pdb +0xE90)
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C
namespace NScript {
class CQ_SunnyvaleMasterData { public:
    unsigned char _pad0[0x84];
    bool          MotherDodgingKraken;                 // +0x84
};
class CQ_TentacleKrakenBossFightScript { public:
    class CM_Experience : public CEntityScriptBase {
    private:
        virtual void Main();
    public:
        CQ_TentacleKrakenBossFightScript* ParentClass;  // +0x14
        CQ_SunnyvaleMasterData*           PMasterData;  // +0x18
        CQ_SunnyvaleMasterData* GetMasterData() { return PMasterData; }
    };
};

void CQ_TentacleKrakenBossFightScript::CM_Experience::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!GetMasterData()->MotherDodgingKraken) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing kraken = Game->GetThingWithScriptName("TheKraken");
    while (kraken.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->CreateExperienceOrb(Me.GetPos() + C3DVector(0.0f, 0.0f, 5.0f), pGlobalScriptDef->PrisonExperienceForKillingKraken);
}
}