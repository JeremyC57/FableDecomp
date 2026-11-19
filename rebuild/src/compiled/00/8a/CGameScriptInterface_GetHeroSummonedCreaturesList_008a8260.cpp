// CGameScriptInterface::GetHeroSummonedCreaturesList(std::vector<CScriptThing>& creatures) const -- append the script things of the hero's live summon-spell minion (TC 145) and live ghost-sword minions (TC 154); true when one was added
// Grade: RELOCATION_MATCH 466v466 (hand3c 2026-11-19). The earlier `phero && !phero->IsDead()` spelling merged the early exit into the
//   main epilogue (456v466); the shared engine/ThingAlive.h IS_THING_ALIVE (inline, two returns) gives retail's private early-exit
//   epilogue 0x008A8428 (`pop esi ; mov al,bl ; pop ebx`), as in the landed sibling CGameScriptThing::MsgGetThingsKilled 0x008D3DD0.
// Shape: IS_THING_ALIVE for the hero and for both minion tests (FableWin.pdb has the instantiations
//   ??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z 0x018FFDA0 and
//   ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingAICreature@@@@@@YI_NABV?$CIntelligentPointer@VCThingAICreature@@@@@Z 0x01B5F900).
//   Only the hero test decides the bytes (the minion tests compile the same either way); the debug size picks the all-IS_THING_ALIVE
//   form: VC7.1 /Od /EHsc gives 418 bytes for it against Ego_d / FableWin's 417 (hero-only 478, minion-only 455, none 488).
//   Locals and names: Ego_d.pdb (thing_list, added, phero; ptc_summon_spell, ptc_ghost_sword; minion; minions, i).
// Retail: 0x008A8260 (466 bytes, `ret 4`), FableWin.pdb / Ego_r.pdb ?GetHeroSummonedCreaturesList@CGameScriptInterface@@UBE_NAAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
// Leaf: ?GetHeroSummonedCreaturesList@CGameScriptInterface@@UBE_NAAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCSUMMONSPELL
#define FABLE_RETAIL_CTCSUMMONSPELL_MINION
#define FABLE_RETAIL_CTCGHOSTSWORD
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCSummonSpell.h"
#include "engine/CTCGhostSword.h"
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::GetHeroSummonedCreaturesList(std::vector<CScriptThing, std::allocator<CScriptThing> >& thing_list) const
{
    bool added = false;
    CThingPlayerCreature* phero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (IS_THING_ALIVE(phero)) {
        const CTCSummonSpell* ptc_summon_spell;
        const CTCGhostSword* ptc_ghost_sword;
        if (phero->PeekTC(TCI_SUMMON_SPELL, &ptc_summon_spell)) {
            CIntelligentPointer<CThingAICreature> minion = ptc_summon_spell->GetSummonedCreature();
            if (IS_THING_ALIVE(minion)) {
                thing_list.push_back(minion->GetScriptThing());
                added = true;
            }
        }
        if (phero->PeekTC(TCI_GHOST_SWORD, &ptc_ghost_sword)) {
            std::vector<CIntelligentPointer<CThingAICreature> > minions = ptc_ghost_sword->GetSummonedCreatures();
            for (long i = 0; i < (long)minions.size(); ++i) {
                if (IS_THING_ALIVE(minions[i])) {
                    thing_list.push_back(minions[i]->GetScriptThing());
                    added = true;
                }
            }
        }
    }
    return added;
}