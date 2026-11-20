// CCreatureAction_SitDown::PerformAction() -- push the action's sitting mode onto the creature's mode manager (interface type 0x31)
// Leaf: ?PerformAction@CCreatureAction_SitDown@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- HasTC + the ternary GetTC model into an UNINITIALISED local (the miss arm reads the key slot): both arms load the mode manager straight into ecx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00848FD0, length 105 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface
// lookup for type 0x31 = bit 17 of the bitset dword at +0x24), CTCCreatureModeManager::AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>)
// @ 0x0073ACC0 (bool result dropped).
// Shape: the mode manager is an UNINITIALISED out-parameter used unconditionally (the miss path loads the never-written frame word into ecx and
// joins the hit path). The counted pointer argument is passed BY VALUE: its inline copy constructor builds it in the argument area
// (`sub esp,8 ; mov edx,esp`), copies the object word, then the count block word with the null test around the store and the add-ref
// `inc dword ptr [eax]` on the block's reference count.
// Layout: engine/CCreatureAction_SitDown.h retail branch (FABLE_RETAIL_CCREATUREACTION_SITDOWN): PSitMode (CCountedPointer<CModeSitBase>)
// at +0xa8/+0xac on the retail 0xa8-byte CCreatureActionBase.
// Naming (NF138, 2026-11-20; q6408): the local CTCCreatureModeManager / CCountedPointer_CModeBase / SCountBlock / CCreatureActionBase /
//   CCreatureAction_SitDown models replaced by the shared headers (CTCCreatureModeManager.h ADDMODEPOINTER with the nested CModeBase,
//   CCountedPointer.h, NCreatureMode_CModeSitFloor.h, CCreatureActionBase.h, CCreatureAction_SitDown.h); 0x0073ACC0 now binds the Ego_r.pdb
//   ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z (was ...QAE_NUCCountedPointer_CModeBase@@@Z).
#include "fable_compiler.h"
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTION_SITDOWN
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode_CModeSitFloor.h"
#include "engine/CCreatureAction_SitDown.h"

void CCreatureAction_SitDown::PerformAction()
{
    CTCCreatureModeManager* pModes;
    CThing* creature = PCreature.Get();
    if (creature->HasTC(TCI_ENTITY_MODE_MANAGER))
        pModes = ((CTCCreatureModeManager*)&creature->GetTC(TCI_ENTITY_MODE_MANAGER));
    pModes->AddMode(PSitMode);
}
