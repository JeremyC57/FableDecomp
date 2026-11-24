// CTCCombat::PlayCombatStrikeSounds() -- when the creature has a sound player and an entity sound def, is a hero (has the experience component) and its hero-combat def enables strike sounds, play the experience's on-hit sound from the sound def's map through the sound player
// Leaf: ?PlayCombatStrikeSounds@CTCCombat@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00733B70, 380 bytes, thiscall, `ret`.
// Name: Ego_r.pdb ?PlayCombatStrikeSounds@CTCCombat@@QAEXXZ (0x0074CA00) + FableWin.pdb (0x024FFB00); the catalog label
//   "CTCCreature_PlayDeathFlourish" was descriptive (NAME_FIXES.tsv).
// Source: FableWin.pdb debug body 0x024FFB00 (tc_combat.obj) with Ego_d.pdb's locals (thing, ptc_sound, psound_def, ptc_hero_exp;
//   block: sound_symbol_name, psound_pair): thing.GetTC(&ptc_sound) && thing.PeekBaseDef().GetSubDef(&psound_def) &&
//   GetParentThing().PeekTC(&ptc_hero_exp) && GetPHeroCombatDef()->MeleeCombatPlayStrikeSounds, then GetOnHitSound bound to a const
//   reference, GetPSoundPair on the def's SoundMap and PlayAttachedSample(*psound_pair, CSoundInit(), true, false, false).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEROCOMBATDEF
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CHEROCOMBATDEF_STRIKESOUNDS
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_STRIKESOUNDS
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_ONHITSOUND
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_PLAYATTACHEDSAMPLE
#define FABLE_RETAIL_CENTITYSOUNDDEF_PASS22_SOUNDMAP
#define FABLE_RETAIL_CSOUNDMAP
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR_TYPE_LAST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefString.h"
#include "engine/CEntitySoundDef.h"
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CENTITYSOUNDDEF_SIZE_BODY   // NF357: GetSubDef<CEntitySoundDef> pick-any copy = size-mode owner 0x0065F604 (scaffolding, not retail)
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CHeroCombatDef.h"
#include "engine/CSoundMap.h"
#include "engine/CSoundInit.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCHeroExperience.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CTCCombat.h"

void CTCCombat::PlayCombatStrikeSounds()
{
    CThing& thing = GetParentThing();
    CTCSoundPlayer* ptc_sound;
    CDefPointer<const CEntitySoundDef> psound_def(NULL_DEF);
    const CTCHeroExperience* ptc_hero_exp = 0;
    if (thing.GetTC(&ptc_sound) && thing.PeekBaseDef().GetSubDef(&psound_def) && GetParentThing().PeekTC(&ptc_hero_exp) &&
        GetPHeroCombatDef()->MeleeCombatPlayStrikeSounds) {
        const CCharString& sound_symbol_name = ptc_hero_exp->GetOnHitSound();
        const CSoundPair* psound_pair = psound_def->SoundMap.GetPSoundPair(sound_symbol_name);
        if (psound_pair)
            ptc_sound->PlayAttachedSample(*psound_pair, CSoundInit(), true, false, false);
    }
}