// CTCTavernGame::RegisterGameMaster(CThingAICreature* master) -- PGameMaster (+0xD0) = CIntelligentPointer<CThingAICreature>(master), through a stack temporary
// Leaf: ?RegisterGameMaster@CTCTavernGame@@QAEXPAVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094D430, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?RegisterGameMaster@CTCTavernGame@@QAEXPAVCThingAICreature@@@Z; Ego_r CTCTavernGame PGameMaster is
//   CIntelligentPointer<CThingAICreature> (retail +0xD0, engine/CTCTavernGame.h); the landed caller 0x0094D020 binds this exact spelling.
//   The other binder 0x0094CB40 spells it ?SetGameMaster@CTCTavernGameCoinGolf@@ (no PDB carries that name: a binder spelling to fix).
//   Was the stand-in CClass_0094d430::AssignTarget_0094d430.
// Body: the (T*) constructor of a temporary CIntelligentPointer<CThingAICreature> (0x00A01B10, vptr, SetPItem 0x00A01B90), the inline
//   operator= (GetPItem 0x00A01B50, SetPItem on the member), ~CBaseIntelligentPointer 0x00A01C10.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_REGISTERGAMEMASTER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCTavernGame.h"

void CTCTavernGame::RegisterGameMaster(CThingAICreature* master)
{
    PGameMaster = CIntelligentPointer<CThingAICreature>(master);
}
