// CombatDialogueManager::PlayBattleCry(CThingCreatureBase& creature) -- when the creature may speak: look up its creature def's battle-cry
// sound in the global sample bank and, if it has one and a sound player, play it attached; record the frame of the last battle cry
// Leaf: ?PlayBattleCry@CombatDialogueManager@@QAE_NAAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AF850, `ret 4`. Callees: CombatDialogueManager::IsSpeechAllowed 0x008AF5F0,
//   CDefClassBase::GetSubDef<CCreatureDef> 0x00485815 (on the creature's base def +0x70), CDefStringTable::GetString 0x009D49B0 on
//   GDefStringTable 0x013CA828 (the implicit CDefString -> CCharString conversion of BattleCrySound +0xb0), CMainGameComponent::PMainGameComponent
//   0x013B86A0 (+0x10 sample bank), CASoundBank::GetSoundIndexFromSymbol 0x00A01920, ~CCharString 0x0099EAE0, CVectorMap::LowerBound 0x0040F020
//   (inline GetTC<CTCSoundPlayer>, 0x10 after the HasTC test), the inline CSoundInit / CSoundPair, CTCSoundPlayer::PlayAttachedSample 0x0066B280,
//   CWorld::GetFrame 0x0049D870; the def handle's release (no null test: the def is dereferenced first).
// NF67 (round 7): ported onto the shared headers. The ExternalRetry_008af850 namespace (local CCreatureDef / DefGuard / CCharString /
//   CDefStringTable / SymbolMap / CASoundBank / CMainGameComponent / CVectorMap / CThingCreatureBase models and their same-TU stand-in
//   bodies) is gone: the def lookup is the header's GETSUBDEF_BODY template, the string and the sample bank are the real classes.
//   0x013B86A0 now binds ?PMainGameComponent@CMainGameComponent@@1PAV1@A (nf47:land1011fv).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_PLAYATTACHEDSAMPLE
#define FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR_TYPE_LAST
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CASOUNDBANK_SOUNDINDEX
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SAMPLEBANK_WORLDSEED
#define FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#define FABLE_RETAIL_CCREATUREDEF_SCREENINVENTORIES
#define FABLE_RETAIL_CCREATUREDEF_RESPAWNWAIT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_COMBATDIALOGUEMANAGER
#define FABLE_RETAIL_COMBATDIALOGUEMANAGER_SPEECH
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CSoundInit.h"
#include "engine/CSoundPair.h"
#include "engine/CASoundBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CombatDialogueManager.h"

bool CombatDialogueManager::PlayBattleCry(CThingCreatureBase& creature)
{
    if (IsSpeechAllowed(creature)) {
        CDefPointer<const CCreatureDef> p_creature_def;
        creature.PeekBaseDef().GetSubDef(&p_creature_def);
        long sound_index = CMainGameComponent::Get()->GetSampleBank().GetSoundIndexFromSymbol(p_creature_def->BattleCrySound);
        if (sound_index && creature.HasTC(TCI_SOUND_PLAYER)) {
            CTCSoundPlayer* sound_player;
            creature.GetTC(&sound_player);
            if (sound_player->PlayAttachedSample(CSoundPair(sound_index), CSoundInit(), true, false, false)) {
                LastBattleCryFrame = CWorld::GetFrame();
                return true;
            }
        }
    }
    return false;
}
