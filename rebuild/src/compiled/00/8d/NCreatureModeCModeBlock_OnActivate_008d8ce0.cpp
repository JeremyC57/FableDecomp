// NCreatureMode::CModeBlock::OnActivate() -- base activation, remember the creature, suspend its animation while it is in state 1, play its "START_BLOCKING" sound pair, then run one FrameUpdate
// Leaf: ?OnActivate@CModeBlock@NCreatureMode@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8CE0, 231 bytes (`ret`; vtable 0x01267C3C slot 5).
// Callees: CTCCreatureModeManager::CModeBase::OnActivate 0x0073AAC0 (qualified base call); CThingAICreature::SuspendAI
// (bool, long) 0x00833A10; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface, type 0x10 = bit 16
// of the dword at +0x20); CCharString(const char*, long) 0x0099EBF0 ("START_BLOCKING"), ~CCharString 0x0099EAE0; the creature's
// virtual slot 89 (+0x164, the sound pair for a name, hidden-pointer return); the sound component's play call 0x0066B280
// (manifest "CAnimateThing_PlayRandomSound"); this mode's own virtual slot 3 (+0x0c, FrameUpdate 0x008D8EA0).
// Layout (PDB CModeBlock 0x20): CModeBase 0x1c (ParentThing +0x04), PCreature +0x1c. Creature: state byte +0x10.
// Shape: the landed CCreatureAction_BlockBase::Initialise 0x00855430 sound block (string temporary first, the play-parameter
// block assigned in its ctor body) inside the GetInterface test, after the suspend test.
// Naming (NF128, 2026-11-20; q6454): the local CModeBase / CThingAnimatable / CThingCreatureBase (89 placeholder virtuals) / CTCSoundPlayer /
//   CThingAICreature / CSoundPair stand-ins replaced by the shared headers: CModeBlock (no shared header; the row already modelled it) now
//   derives from the shared nested CTCCreatureModeManager::CModeBase (VIRTUALS), GetSoundPair is the CThingCreatureBase slot of
//   engine/CThingCreatureBase.h under FABLE_RETAIL_CTHING_ORIGINAL_VTABLE (the 0x00665860 define set), SuspendAI from engine/CThingAICreature.h
//   (DRAINED), PlayAttachedSample from engine/CTCSoundPlayer.h. RELOCATION_MATCH in one form.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CTCSOUNDPLAYER_PLAYATTACHEDSAMPLE
#define FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR_TYPE_LAST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CSoundPair.h"
#include "engine/CSoundInit.h"                    // Ego_r.pdb CSoundInit (0x20), the start parameters; inline ctor expanded
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCCreatureModeManager.h"

namespace NCreatureMode {

class CModeBlock : public CTCCreatureModeManager::CModeBase {    // vtable 0x01267C3C (no shared header; RTTI CModeBlock : CModeBase)
public:
    virtual void OnActivate();
    CThingCreatureBase* PCreature;         // +0x1c
};

void CModeBlock::OnActivate()
{
    CTCCreatureModeManager::CModeBase::OnActivate();
    PCreature = (CThingCreatureBase*)&ParentThing;
    if (PCreature->GetType() == 1)
        ((CThingAICreature*)PCreature)->SuspendAI(true, 0);
    CTCSoundPlayer* player;
    if (ParentThing.GetTC(TCI_SOUND_PLAYER, &player)) {
        player->PlayAttachedSample(PCreature->GetSoundPair("START_BLOCKING"), CSoundInit(), true, false, false);
    }
    FrameUpdate();
}

}
