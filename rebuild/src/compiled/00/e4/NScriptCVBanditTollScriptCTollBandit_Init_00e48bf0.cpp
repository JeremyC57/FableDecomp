// NScript::CV_BanditTollScript::CTollBandit::Init() -- reset the toll state, take the pay-me distance from the script def, find the bandit escape marker, then put the bandit in the friendly bandit faction and stop it wanting money
// Leaf: ?Init@CTollBandit@CV_BanditTollScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E48BF0, length 271 bytes (+ int3 padding).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, ~CBaseClass @ 0x0099A2E0, operator
// delete @ 0x00BFE9BC, CGameScriptInterface slots 72 GetThingWithScriptName (+0x120), 601 EntitySetInFaction (+0x964) and
// 478 EntitySetThingAsWantingMoney (+0x778) (ghidra_out/cgsi_slots_aligned.tsv).
// Globals: pGlobalScriptDef @ 0x0143E90C (TollBanditPayMeDistance at +0x414); strings 0x012E4DA4 "M_BanditEscapePoint",
// 0x012D218C "FACTION_BANDITS_FRIENDLY".
// Layout: Ego_r.pdb CTollBandit (= retail): Game +0x04, Me +0x08 (the entity-script base, as the landed CM_TollBandit::Main
// 0x00E48A30), ParentClass +0x14, PMasterData +0x18, TollState +0x1c, TollBanditOnlyCollectsToll +0x20,
// KilledReluctantTrader +0x21, TollBanditPayMeDistance +0x24, BanditEscapeMarker +0x28 (CScriptThing).
// Shape: the marker is assigned from the GetThingWithScriptName temporary through the inline CScriptThing / CCountedPointer
// operator= (the source's fields read first, the old block released, the new one add-ref'd), then the temporary's
// inline destructor (vtable store, release, zeroing, ~CBaseClass) and the name's; both names are implicit conversions.
// Naming wave 35: identity reg 00e47af5 "TollBandit" in 00e479b0; Alloc 00e48b40; vtable 012e4d88 slot 2; PDB ?Init@CTollBandit@CV_BanditTollScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CTollBandit::Init (wave3/labels_alloc.tsv); PDB ?Init@CTollBandit@CV_BanditTollScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CTollBandit@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own)); same bytes.
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"                              // CCharString (agentNF60: was a local stand-in)
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// agentNF60: the local CScriptDef model named these members descriptively; engine/CScriptDef.h (Ego_r / FableWin names at the
//   retail offsets) names them: TollBanditPayMeDistance -> TollBanditPayDistance (+0x414).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>; agentNF60: was a local CScriptDef model and `CScriptDef* pGlobalScriptDef`)
// Literal (NF420, 2026-11-25): made-up data symbol(s) replaced by retail's pooled literal (/GF-merged ??_C@) at each use site:
//   g_strBanditEscapePoint (table ?g_strBanditEscapePoint@@3QBDB -> 0x012E4DA4) -> "M_BanditEscapePoint"
//   g_strFactionBanditsFriendly (table ?g_strFactionBanditsFriendly@@3QBDB -> 0x012D218C) -> "FACTION_BANDITS_FRIENDLY"

namespace NScript { class CV_BanditTollScript; } using NScript::CV_BanditTollScript;
class CQ_SunnyvaleMasterData;
namespace NScript {
class CV_BanditTollScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CTollBandit;
};
}
namespace NScript {
class CV_BanditTollScript::CTollBandit {
public:
        CGameScriptInterface*   Game;                        // +0x04
    CScriptThing            Me;                          // +0x08
    CV_BanditTollScript*    ParentClass;                 // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                 // +0x18
    long                    TollState;                   // +0x1c
    bool                    TollBanditOnlyCollectsToll;  // +0x20
    bool                    KilledReluctantTrader;       // +0x21
    long                    TollBanditPayMeDistance;     // +0x24
    CScriptThing            BanditEscapeMarker;          // +0x28
private:
    virtual void Init();                          // PDB ?Init@CTollBandit@CV_BanditTollScript@NScript@@EAEXXZ
};
}

void NScript::CV_BanditTollScript::CTollBandit::Init()
{
    TollState = 1;
    TollBanditOnlyCollectsToll = false;
    KilledReluctantTrader = false;
    TollBanditPayMeDistance = pGlobalScriptDef->TollBanditPayDistance;
    BanditEscapeMarker = Game->GetThingWithScriptName("M_BanditEscapePoint");
    Game->EntitySetInFaction(Me, "FACTION_BANDITS_FRIENDLY");
    Game->EntitySetThingAsWantingMoney(Me, 0);
}