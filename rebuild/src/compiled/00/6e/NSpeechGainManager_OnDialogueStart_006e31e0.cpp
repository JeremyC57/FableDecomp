// NSpeechGainManager::OnDialogueStart(const CWorld& world, CThing& speaker, unsigned long sound_id, EDialogueLayer layer) -- register a line a live speaker has started playing: insert it into the active dialogue list ahead of the first line on a later layer (or, on its own layer, ahead of a live speaker farther from the hero than the new one by more than the layer's focus-shift threshold), count it on its layer and set its sound's volume to its target gain; always false
// Leaf: ?OnDialogueStart@NSpeechGainManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E31E0, 596 bytes (+ int3 padding to 608), `__fastcall` (ecx = world, edx = speaker), `ret 8`;
//   the one exit is `xor al,al`.
// Identity: Ego_r.pdb / FableWin.pdb ?OnDialogueStart@NSpeechGainManager@@YI_NABVCWorld@@AAVCThing@@KW4EDialogueLayer@1@@Z (the
//   catalog name; the shared header's FABLE_RETAIL_NSPEECHGAINMANAGER_ONDIALOGUESTART declaration). Locals: Ego_d.pdb's record (debug
//   length 2129): world, speaker, sound_id, layer; ptc_sound_player; layer_defs, it; pthing_player; old_to_speaker, new_to_speaker
//   (that nesting is the block structure below). Statements in FableWin.pdb's order (debug 0x023E87D0): the two release-dead ASSERTs
//   (layer < DIALOGUE_LAYER_LAST, IS_THING_ALIVE(&speaker)), `if (layer < DIALOGUE_LAYER_LAST && IS_THING_ALIVE(&speaker))`,
//   GetTC<CTCSoundPlayer> (its failure arm only reports in debug, so release reads the unassigned frame word), IsAttachedSoundPlaying,
//   the layer-defs size ASSERT, the list walk, insert, the layer count, OutputGain, SetSpecificSampleVolume.
// Callees: CVectorMap LowerBound 0x0040F020 (the expanded GetTC, TCI_SOUND_PLAYER 0x10), CTCSoundPlayer::IsAttachedSoundPlaying
//   0x0066AE10, CBaseIntelligentPointer::GetPItem 0x00A01B50, CWorld::PeekMyPlayer 0x0049D9D0, CPlayer::GetPControlledCreature 0x00487DC0,
//   CThing::GetPos 0x004C73D0, CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 / ~ 0x00A01C10 (the CActiveDialogueInfo
//   temporary), list<CActiveDialogueInfo>::_M_create_node 0x006E4CB0 (landed; this TU's out-of-line copy grades RELOCATION_MATCH at that
//   address too), GetTargetGain 0x006E2B70 (landed), CTCSoundPlayer::SetSpecificSampleVolume 0x0066AFF0.
// Globals: g_ActiveDialogue 0x013BABA0, g_LayerSpeakerCounts 0x013BAB94 (NSpeechGainManager.h).
// Shape: FableWin's walk tests `layer == it->Layer && IS_THING_ALIVE(it->ThingSpeaking)` as ONE condition: a dead or null speaker on
//   the same layer falls to the `else if (layer < it->Layer)` arm (FableWin 0x023E8E1B and retail 0x006E329A both jump there); the
//   nested form keeps layer in ebp and the stored offset's squares come out z, y, x (retail: y, z, x). The gain is the chained
//   `SetSpecificSampleVolume(sound_id, it->OutputGain = GetTargetGain(..))` (retail `fst [esp+40h] ; mov eax,[esp+40h] ; fstp [esi+1Ch]`;
//   Ego_d has no gain local). FABLE_RETAIL_STLP_LIST_CREATE_NODE_NOINLINE (overlay opt-in) keeps _M_create_node out of line as retail.
#include <math.h>
#pragma intrinsic(sqrt)
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_ATTACHEDDIALOGUE
#define FABLE_RETAIL_CTCSOUNDPLAYER_SETSPECIFICSAMPLEVOLUME
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST_CREATE_NODE_NOINLINE
#define FABLE_RETAIL_NSPEECHGAINMANAGER_ONDIALOGUESTART
#define FABLE_RETAIL_NSPEECHGAINMANAGER_ACTIVEDIALOGUEINFO
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGlobalDef.h"
#include "engine/ThingAlive.h"
#include "engine/NSpeechGainManager.h"

namespace NSpeechGainManager {

bool __fastcall OnDialogueStart(const CWorld& world, CThing& speaker, unsigned long sound_id, EDialogueLayer layer)
{
    if (layer < DIALOGUE_LAYER_LAST && IS_THING_ALIVE(&speaker)) {
        CTCSoundPlayer* ptc_sound_player;
        speaker.GetTC(&ptc_sound_player);
        if (ptc_sound_player->IsAttachedSoundPlaying(sound_id)) {
            const std::vector<CDialogueLayerDef>& layer_defs = world.PeekDefinitionManager().PeekGlobalDef().DialogueLayers;
            std::list<CActiveDialogueInfo>::iterator it;
            for (it = g_ActiveDialogue.begin(); it != g_ActiveDialogue.end(); ++it) {
                if (layer == it->Layer && IS_THING_ALIVE(it->ThingSpeaking)) {
                    const CThingPlayerCreature* pthing_player = world.PeekMyPlayer()->GetPControlledCreature();
                    if (IS_THING_ALIVE(pthing_player)) {
                        C3DVector old_to_speaker = it->ThingSpeaking->GetPos() - pthing_player->GetPos();
                        C3DVector new_to_speaker = speaker.GetPos() - pthing_player->GetPos();
                        if (new_to_speaker.GetMagnitude() < old_to_speaker.GetMagnitude() - layer_defs[layer].FocusShiftDistanceThreshold)
                            break;
                    }
                }
                else if (layer < it->Layer)
                    break;
            }
            it = g_ActiveDialogue.insert(it, CActiveDialogueInfo(speaker, sound_id, layer, true));
            ++g_LayerSpeakerCounts[layer];
            ptc_sound_player->SetSpecificSampleVolume(sound_id, it->OutputGain = GetTargetGain(*it, layer_defs));
        }
    }
    return false;
}

}