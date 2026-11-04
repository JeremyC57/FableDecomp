// CTCCreatureOpinionOfHero::ClearPendingDeeds() -- drain the pending deed handles: while the hero exists each (last first) is looked up in the hero's deed log and, when still valid, recorded at the front of its deed type's known list; without a hero the pending list is simply emptied
// Grade: DIFFER(261v261) -- dl=7 (try b, retail headers: std::list<long> deeds overlay CTCCREATUREOPINIONOFHERO_DEEDS + STLP_LIST_POPBACK, GetPControlledCreature, GetTC<T>): only two register choices left -- back() loads the head into ecx and the value into edx (retail eax / ecx), and the KnownDeeds index is in eax and the head in ecx (retail edx / eax); named handle / list reference / type local / for-loop / *--end() forms identical
// Leaf: ?ClearPendingDeeds@CTCCreatureOpinionOfHero@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) 0x00683B00, 261 bytes (plain `ret`). Identity: `this` is the creature's CTCCreatureOpinionOfHero
//   (the callers 0x009600FB / 0x0096010A look up interface 0x4F; PendingDeeds +0x54 and KnownDeeds[92] +0x1C8 as the landed
//   ClearAllKnownDeeds 0x00681A70); the public void Ego_r.pdb method that empties the pending list is
//   ?ClearPendingDeeds@CTCCreatureOpinionOfHero@@QAEXXZ (the catalog's CTCHeroOpinionDeedLog::ApplyPendingDeedLog is wrong).
// Callees: CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
//   CVectorMap<ETCInterfaceType,CTCBase*> LowerBound 0x0040F020 (the inline GetTC<CTCHeroOpinionDeedLog>: interface 81, the miss path
//   reads the never-written out slot), CTCHeroOpinionDeedLog::GetDeedFromHandle(long) const 0x006C58F0 (Ego_r.pdb
//   ?GetDeedFromHandle@CTCHeroOpinionDeedLog@@QBEPBVCOpinionDeed@@J@Z), malloc 0x00BFEA0E / free 0x00BFEA14 (the STLport list nodes).
// Layout: COpinionDeed Type +0, Handle +4 (no engine header: local model, as CTCHeroOpinionDeedLog).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_POPBACK
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CTCCreatureOpinionOfHero.h"

class COpinionDeed {                                    // local model (no engine header)
public:
    EOpinionDeedType Type;                              // +0x00
    long Handle;                                        // +0x04
};
class CTCHeroOpinionDeedLog : public CTCBase {          // local model (no engine header)
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_OPINION_DEED_LOG; }
    const COpinionDeed* GetDeedFromHandle(long handle) const;   // 0x006C58F0
};

void CTCCreatureOpinionOfHero::ClearPendingDeeds()
{
    CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero) {
        CTCHeroOpinionDeedLog* deedLog;
        hero->GetTC(&deedLog);
        while (!PendingDeeds.empty()) {
            const COpinionDeed* deed = deedLog->GetDeedFromHandle(PendingDeeds.back());
            if (deed)
                KnownDeeds[deed->Type].push_front(deed->Handle);
            PendingDeeds.pop_back();
        }
    } else {
        PendingDeeds.clear();
    }
}
