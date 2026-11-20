// CCreatureAction_SheatheWeapons::PerformAction() -- sheathe all of the creature's weapons, drop creature mode 4 through the mode manager (interface type 0x31), then make the "SIMPLE_NOISE" noise
// Leaf: ?PerformAction@CCreatureAction_SheatheWeapons@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7B10, length 133 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), CThingCreatureBase::SheatheAllWeapons @ 0x006647F0,
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the bitset dword at +0x24),
// CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0, CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal 0x01268A5C
// "SIMPLE_NOISE"), CThingCreatureBase::MakeNoise(const CCharString&) @ 0x00662110, CCharString::~CCharString @ 0x0099EAE0.
// Shape: one `push ecx` frame word shared by the lookup key and the noise-name string; the string is a scoped named local (its address is
// re-derived with `lea` for the argument, GOTCHAS); esi is pushed only around the lookup. &PCreature rides in edi.
// Names (NF119, PRIORITY q6158): Ego_r.pdb 0x0053CF90 + FableWin.pdb 0x01D9C460 ?MakeNoise@CThingCreatureBase@@QAEXABVCCharString@@@Z and
//   ?SheatheAllWeapons@CThingCreatureBase@@QAEXXZ (PCreature is CIntelligentPointer<CThingCreatureBase> in Ego_r); was a local CThing
//   model (?MakeNoise@CThing@@ / ?SheatheAllWeapons@CThing@@ stand-ins). Now on the shared headers (engine/CThingCreatureBase.h,
//   CCreatureAction_SheatheWeapons.h, CTCCreatureModeManager.h; the expanded GetTC as in 0x0092DA20).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_BEDSLEEP      // SheatheAllWeapons declaration
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_GOSSIP          // CCreatureAction_SheatheWeapons virtual members (Initialise / PerformAction / Clone)
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_SheatheWeapons.h"
#include "engine/CTCCreatureModeManager.h"

void CCreatureAction_SheatheWeapons::PerformAction()
{
    PCreature->SheatheAllWeapons();
    CTCCreatureModeManager* modes;
    if (PCreature->GetTC(&modes))
        modes->RemoveMode(4);
    {
        CCharString noise("SIMPLE_NOISE");
        PCreature->MakeNoise(noise);
    }
}
