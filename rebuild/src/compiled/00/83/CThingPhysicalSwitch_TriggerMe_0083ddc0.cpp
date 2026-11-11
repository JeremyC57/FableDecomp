// CThingPhysicalSwitch::TriggerMe(CThingCreatureBase& triggerer) -- trigger the switch once: mark it triggered, queue a CEventTriggered for the triggerer on the entity-events component, then play the def's TriggerSound through the sound player when it has one
// Leaf: ?TriggerMe@CThingPhysicalSwitch@@IAEXAAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083DDC0, 344 bytes (`ret 4`). Ego_r.pdb / FableWin.pdb
//   ?TriggerMe@CThingPhysicalSwitch@@IAEXAAVCThingCreatureBase@@@Z (protected).
// Callees (PDB spellings): CVectorMap::LowerBound 0x0040F020 (the inline HasTC + GetTC of TCI_ENTITY_EVENTS 0x42 and TCI_SOUND_PLAYER
//   0x10), operator new 0x00BFEA1A, NEntityEvents::CEventTriggered::CEventTriggered(CThing&) 0x0083CF20, CTCEntityEvents::AddEvent
//   0x008113C0, CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDef<CThingPhysicalSwitchDef> 0x0083E0D0 (the first
//   PeekDefinition expanded), CThingPhysicalSwitch::PeekDefinition 0x0083D900 (the second one called), CTCSoundPlayer::PlayAttachedSample
//   0x0066B280, the def's virtual release.
// Layout: CThing DefGlobalIndex +0x8C, bitset +0x20, map +0x44; CThingPhysicalSwitch Triggered +0xD0; CThingPhysicalSwitchDef
//   TriggerSound +0x90; CSoundInit (inline ctor) 0x20 bytes.
// Shape: as UntriggerMe 0x0083DF20: the sound pair is a named local in its own block (it shares the frame word of the def handle and
//   the lookup key), the CSoundInit a temporary.
// Name fix (agentNF12 2026-11-11): moved off the local CThing / CThingPhysicalSwitch / CDefPointer / CDefinitionManager /
// CTCEntityEvents / CEventTriggered / CTCSoundPlayer / CSoundPair / CSoundInit stand-ins (and a same-TU stand-in GetDef body) onto the
// shared headers (new engine/NEntityEvents_CEventTriggered.h). 0x004C7990 binds CThing::PeekDefinitionManager (was
// ?GetDefinitionManager@CThing@@QBEPAVCDefinitionManager@@XZ), 0x0083E0D0 the const GetDef<CThingPhysicalSwitchDef> template (was
// ?GetDef@CDefinitionManager@@QAE_NJAAV?$CDefPointer@VCThingPhysicalSwitchDef@@@@@Z), 0x0083CF20 ??0CEventTriggered@NEntityEvents@@QAE@AAVCThing@@@Z
// (was the pointer form), 0x0066B280 PlayAttachedSample (.., bool, bool, bool) returning unsigned long; the row's own symbol is the PDB
// protected IAE taking CThingCreatureBase& (was QAE taking CThing*).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGPHYSICALSWITCH_PEEKDEFINITION
#define FABLE_RETAIL_CTHINGPHYSICALSWITCH_PEEKDEFINITION_SAMETU
#define FABLE_RETAIL_CTHINGPHYSICALSWITCH_TRIGGER
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_PLAYATTACHEDSAMPLE
#define FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR_TYPE_LAST
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPhysicalSwitchDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingPhysicalSwitch.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventTriggered.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CSoundPair.h"
#include "engine/CSoundInit.h"


void CThingPhysicalSwitch::TriggerMe(CThingCreatureBase& triggerer)
{
    if (Triggered)
        return;
    Triggered = true;
    if (HasTC(TCI_ENTITY_EVENTS)) {
        CTCEntityEvents& events = static_cast<CTCEntityEvents&>(GetTC(TCI_ENTITY_EVENTS));
        events.AddEvent(new NEntityEvents::CEventTriggered(triggerer));
    }
    if (PeekDefinition().TriggerSound && HasTC(TCI_SOUND_PLAYER)) {
        CTCSoundPlayer& player = static_cast<CTCSoundPlayer&>(GetTC(TCI_SOUND_PLAYER));
        {
            long sound = PeekDefinition().TriggerSound;
            CSoundPair pair(sound, sound);
            player.PlayAttachedSample(pair, CSoundInit(), true, false, false);
        }
    }
}
