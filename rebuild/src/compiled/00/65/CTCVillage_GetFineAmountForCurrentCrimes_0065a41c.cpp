// CTCVillage::GetFineAmountForCurrentCrimes() -- the fine the hero owes this village for the crimes it has on record: 0 when the
//   village's region has no def or its def does not use crimes; otherwise the sum of CrimeFineAmount over PlayersCrimes (a deed type
//   whose effects def says OnlyGetFinedForOneInstance counts once), less AmountOfFineAlreadyPaid. A crime with fine -1 ends the walk
//   (the result is then 0 and AmountOfFineAlreadyPaid becomes 1); a negative total is carried back into AmountOfFineAlreadyPaid.
// Leaf: ?GetFineAmountForCurrentCrimes@CTCVillage@@QAEJXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065A41C, length 415 bytes (`leave`, plain `ret`). Name and signature: Ego_r.pdb and
//   FableWin.pdb ?GetFineAmountForCurrentCrimes@CTCVillage@@QAEJXZ (public, non-const, long). Frame-pointer size TU (CTCVillage.cpp):
//   `#pragma optimize("y", off)` + `#pragma optimize("s", on)` as the landed neighbours 0x0065A28F / 0x0065A5BB / 0x0065C5BB; /GS
//   because of the 10-byte bool array (cookie 0x0139C8A8 at [ebp-4], __security_check_cookie 0x00BFE9F9 before `leave`).
// Callees: CTCBase::GetWorldMap 0x00686D30 (three times); CThing::GetPos 0x004C73D0; CWorldMap::GetMapNumberAt(const C3DVector&)
//   (vtable slot 16, +0x40, 0x004FC0C0); CWorldMap::GetRegionNumberMapIsIn 0x004FC190; CWorldMap::GetRegion 0x004FB490;
//   CWorld::PeekMyPlayer 0x0049D9D0 (on CMainGameComponent::Get() 0x013B86A0 -> world +0x24); CPlayer::GetPControlledCreature
//   0x00487DC0; std::bitset<274>::test 0x00410DE0 and CVectorMap::LowerBound 0x0040F020 (the inline CThing::GetTC, interface 0x51);
//   CTCHeroOpinionDeedLog::GetDeedFromHandle 0x006C58F0; CTCHeroOpinionDeedLog::PeekPDeedEffectsDef 0x006C5630; the STLport tree
//   increment 0x004292C0 (set iterator ++); the defs' slot 1 (CDefPointeeBase::Delete) on a last release.
// Layout: CTCVillage ParentThing +0x04, PlayersCrimes (set<long>) +0x64, AmountOfFineAlreadyPaid +0xA0; CRegion PDef +0x24 (inline
//   GetPRegionDef: null-tested add-ref copy); CRegionDef UseCrimes +0x9D (landed CRegionDef::Transfer 0x004715AB; Ego_r +0xA9);
//   COpinionDeedEffectsDef CrimeFineAmount +0x74, OnlyGetFinedForOneInstance = bit 5 of +0x7C; COpinionDeed DeedType +0x00.
// Shape: the map number and the region number are named locals (written as one nested expression c2 keeps the fine in ebx and
//   spills `this`; retail keeps `this` in ebx and the fine in the frame at [ebp-14h]); the region def is a CDefPointer local that a
//   null def leaves unreleased (`je` straight to `xor eax,eax`) and both other exits release with `dec [edi+4] ; jne`; the effects
//   def is a named local in the loop body, released by its destructor on both the normal path (pointer cleared) and the fine == -1
//   `break` (no clear, and the `fine < 0` test folded away on that path); the deed-type flags are a zeroed bool[10] indexed by
//   DeedType (`xor eax,eax ; stosd ; stosd ; stosw`), the size the neighbour UpdateMirroredCrimeData's Crimes[10] also uses.
// Headers: shared engine headers throughout (the include block of the landed neighbour 0x0065C5BB); new opt-in
//   FABLE_RETAIL_CTCVILLAGE_FINEAMOUNT (engine/CTCVillage.h) declares this method. CRegionDef has no shared header: a two-fact local.
#include "fable_compiler.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_MIRROREDCRIMES
#define FABLE_RETAIL_CTCVILLAGE_FINEAMOUNT
#define FABLE_RETAIL_CREGION_PREGIONDEF
#include <string.h>
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CDefPointer.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/COpinionDeed.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CTCVillage.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

// CRegionDef has no shared header: the two facts this row needs (landed CRegionDef::Transfer 0x004715AB: UseCrimes at retail +0x9D).
class CRegionDef : public CDefPointeeBase { public:
    unsigned char _pad[0x9d - sizeof(CDefPointeeBase)];
    bool UseCrimes;                             // +0x9d
};

long CTCVillage::GetFineAmountForCurrentCrimes()
{
    long map = GetWorldMap().GetMapNumberAt(ParentThing->GetPos());
    long region = GetWorldMap().GetRegionNumberMapIsIn(map);
    CDefPointer<const CRegionDef> def = GetWorldMap().GetRegion(region).GetPRegionDef();
    if (!def.IsNull() && def->UseCrimes) {
        long fine = -AmountOfFineAlreadyPaid;
        CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekWorld().PeekMyPlayer()->GetPControlledCreature();
        if (hero) {
            CTCHeroOpinionDeedLog* deed_log;
            hero->GetTC(&deed_log);
            bool seen[10];
            memset(seen, 0, sizeof(seen));
            std::set<long>::iterator end = PlayersCrimes.end();
            for (std::set<long>::iterator it = PlayersCrimes.begin(); it != end; ++it) {
                const COpinionDeed* deed = deed_log->GetDeedFromHandle(*it);
                if (deed) {
                    CDefPointer<const COpinionDeedEffectsDef> effects = deed_log->PeekPDeedEffectsDef(deed->DeedType);
                    if (effects->CrimeFineAmount == -1) {
                        fine = -1;
                        break;
                    }
                    if (!effects->OnlyGetFinedForOneInstance || !seen[deed->DeedType])
                        fine += effects->CrimeFineAmount;
                    seen[deed->DeedType] = true;
                }
            }
        }
        if (fine < 0) {
            AmountOfFineAlreadyPaid = -fine;
            fine = 0;
        }
        return fine;
    }
    return 0;
}