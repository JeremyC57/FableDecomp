// CGameScriptInterface::AddCrimeCommitted(const CScriptThing& victim, ECrime crime, bool informWitnesses, const CScriptThing* criminal, const CScriptThing* witness, EOpinionPostDeedType postType) const -- record a crime against the live hero: post the deed to the hero's opinion deed log (criminal, witness: the given live AI creature, else the victim village's first villager when witnesses are to be informed) and, when asked, inform every live guard of the victim's village that does not yet know of the deed
// Leaf: ?AddCrimeCommitted@CGameScriptInterface@@UBEXABVCScriptThing@@W4ECrime@@_NPBV2@3W4EOpinionPostDeedType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089FEB0, 559 bytes (+ int3 padding), thiscall `ret 18h`.
// Callees (PDB spellings): CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CScriptThing slots 75 IsAlive / 11 GetPThing,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020, list<CIntelligentPointer<CThingAICreature> >::size 0x0065F17B (STLport distance form),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0, CTCHeroOpinionDeedLog::PostDeed 0x006C6990,
//   CTCCreatureOpinionOfHero::KnowsOfDeed 0x00681AE0 / InformOfDeed(long, bool) 0x00683590 (overlay FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM_HANDLE).
//   Graded with FABLE_HDR_OVERLAY=$A/hdr. Local-model prior (2026-10-22 hand288) parked on a register tie; the shared headers match.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM_HANDLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CTCVillage.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"

void CGameScriptInterface::AddCrimeCommitted(const CScriptThing& victim, ECrime crime, bool informWitnesses,
                                             const CScriptThing* criminal, const CScriptThing* witness, EOpinionPostDeedType postType) const
{
    CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->IsDead())
        return;
    if (!victim.IsAlive())
        return;
    CThing* criminalThing = 0;
    if (criminal)
        criminalThing = criminal->GetPThing();
    CThingAICreature* witnessCreature = 0;
    if (witness && witness->IsAlive() && (witness->GetPThing()->GetTypeFlags() & 0x10))
        witnessCreature = static_cast<CThingAICreature*>(witness->GetPThing());
    long deed = -1;
    if (informWitnesses && !witnessCreature) {
        CTCVillage* village;
        if (victim.GetPThing()->GetTC(TCI_VILLAGE, &village) && village->VillagePeople.size())
            witnessCreature = village->VillagePeople.front().Get();
    }
    CTCHeroOpinionDeedLog* deedLog;
    if (hero->GetTC(TCI_HERO_OPINION_DEED_LOG, &deedLog))
        deed = deedLog->PostDeed((EOpinionDeedType)crime, hero->GetPos(), criminalThing, witnessCreature, postType, true, false, 0, false, 0);
    if (informWitnesses && deed != -1 && victim.IsAlive()) {
        CTCVillage* village;
        if (victim.GetPThing()->GetTC(TCI_VILLAGE, &village)) {
            std::list<CIntelligentPointer<CThingAICreature> >& people = village->VillagePeople;
            for (std::list<CIntelligentPointer<CThingAICreature> >::iterator it = people.begin(), end = people.end(); it != end; ++it) {
                CThingAICreature* member = it->Get();
                CTCCreatureOpinionOfHero* opinion;
                if (member && !member->IsDead() && member->HasTC(TCI_GUARD) && member->GetTC(TCI_CREATURE_OPINION_OF_HERO, &opinion)) {
                    if (!opinion->KnowsOfDeed(deed, (EOpinionDeedType)crime, true))
                        opinion->InformOfDeed(deed, false);
                }
            }
        }
    }
}