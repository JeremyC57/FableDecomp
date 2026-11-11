// CThingPhysicalSwitch::UntriggerMe() -- clear the triggered flag and, when the def names a trigger sound and the switch has a sound player, play it attached
// Leaf: ?UntriggerMe@CThingPhysicalSwitch@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083DF20, 247 bytes (+ int3 padding), plain `ret`. Ego_r.pdb / FableWin.pdb
//   ?UntriggerMe@CThingPhysicalSwitch@@IAEXXZ (protected).
// Callees (PDB spellings): CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDef<CThingPhysicalSwitchDef> 0x0083E0D0 (the
//   first PeekDefinition expanded), CThingPhysicalSwitch::PeekDefinition 0x0083D900 (the second one called), CVectorMap::LowerBound
//   0x0040F020 (the inline HasTC + GetTC(TCI_SOUND_PLAYER 0x10)), CTCSoundPlayer::PlayAttachedSample 0x0066B280, the def's virtual release.
// Layout: CThing DefGlobalIndex +0x8C, bitset +0x20, map +0x44; CThingPhysicalSwitch Triggered +0xD0; CThingPhysicalSwitchDef
//   TriggerSound +0x90; CSoundInit (inline ctor) 0x20 bytes.
// Name fix (agentNF12 2026-11-11): moved off the local CThing / CThingPhysicalSwitch / CDefPointer / CDefinitionManager / CTCSoundPlayer /
// CSoundPair / CSoundInit stand-ins (and same-TU stand-in bodies for GetDef, LowerBound and PlayAttachedSample) onto the shared headers.
// 0x004C7990 binds CThing::PeekDefinitionManager (was ?GetDefinitionManager@CThingPhysicalSwitch@@QAEPAVCDefinitionManager@@XZ),
// 0x0083E0D0 the const GetDef<CThingPhysicalSwitchDef> template (was ?GetDef_0083e0d0@CDefinitionManager@@...), 0x0066B280
// PlayAttachedSample(const CSoundPair&, const CSoundInit&, bool, bool, bool) returning unsigned long (was (.., bool, long, long) void);
// the def member is TriggerSound (was UntriggerSound); the row's own symbol is the PDB protected IAEXXZ (was QAEXXZ).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
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
#include "engine/CTCSoundPlayer.h"
#include "engine/CSoundPair.h"
#include "engine/CSoundInit.h"

void CThingPhysicalSwitch::UntriggerMe()
{
    if (Triggered) {
        Triggered = false;
        if (PeekDefinition().TriggerSound && HasTC(TCI_SOUND_PLAYER)) {
            CTCSoundPlayer& player = static_cast<CTCSoundPlayer&>(GetTC(TCI_SOUND_PLAYER));
            {
                long sound = PeekDefinition().TriggerSound;
                CSoundPair pair(sound, sound);
                player.PlayAttachedSample(pair, CSoundInit(), true, false, false);
            }
        }
    }
}
