// CTCActionUseBuyHouse::ReceivedConfirmation(CTCBuyableHouse& house) -- the hero answered yes to the pending house question (HouseQuestion): buy the house (pay its price, own it, count one more shop or house on the hero's stats) or sell it back (get the price, disown it, stop renting it out, count one less); toggle renting it out; or pay for the next dress level and apply it
// Leaf: ?ReceivedConfirmation@CTCActionUseBuyHouse@@AAEXAAVCTCBuyableHouse@@@Z
// Flags: none
// Retail 0x0072EB40, 463 bytes (`ret 4`, no frame pointer, no SEH). Identity: Ego_r.pdb (0x00747160) and FableWin.pdb
//   ?ReceivedConfirmation@CTCActionUseBuyHouse@@AAEXAAVCTCBuyableHouse@@@Z (private, reference parameter); the only caller is
//   GlobalUpdate 0x0072EE30 (call at 0x0072EF00), the next-but-one function of the same retail source file.
// Callees: CTCBase::GetPlayerManager 0x00686D40 (protected member, `this` still in ecx), CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetPControlledCreature 0x00487DC0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inline
//   CThing::GetTC<CTCHeroStats>, key TCI_HERO_STATS = 4), CTCBuyableHouse::GetPrice 0x006C0120, GetPriceForNextDressLevel 0x006BF920,
//   IncrementDressLevel 0x006C04B0, SetRented 0x006C1150, SetOwnedByPlayer 0x006C17B0, CTCHeroStats::AddMoney(long, bool) 0x0057B327.
// Layout: CTCActionUseBuyHouse (no shared header; Ego_r.pdb, 32 bytes, RTTI base CTCActionUseBase 0x14): PDef +0x14,
//   WaitingForConfirmation +0x18, HouseQuestion +0x1C. engine/CTCBuyableHouse.h (OwnedByPlayer +0x11 / Rented +0x12 through
//   FableWin.pdb's inline GetOwnedByPlayer / GetRented), engine/CTCBase.h (GetParentThing, +0x04), engine/CThing.h (UsingComponents
//   +0x20, Components +0x44), engine/CTCHeroStats.h (NumberOfShopsOwned +0xE0, NumberOfHousesOwned +0xE4).
// Shape: a `dec eax` chain switch on HouseQuestion (1 buy/sell, 2 rent, 3 dress) with the dress arm laid out first; each arm fetches
//   the hero again (three inline GetTC expansions, the key copy in the one `push ecx` local or the dead argument slot). The `push 1`
//   ahead of each GetPrice call is AddMoney's bool (arguments are evaluated right to left), not a GetPrice argument. HasTC(TCI_SHOP)
//   on the house's thing is __forceinline (FABLE_RETAIL_CTHING_HASTC_FORCEINLINE): retail tests the bitset word in place
//   (`test dword ptr [esi+28h],80000h`); the plain inline leaves `shr ; and` (the old 467v463 residue). The value 3 of EHouseQuestion
//   has no enumerator name in either PDB: HOUSE_QUESTION_DRESS is descriptive (the landed siblings name 0..2).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_RECEIVEDCONFIRMATION
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_PROPERTYOWNED
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CDefPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCActionUseBase.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCBuyableHouse.h"

class CBuyHouseDef;
enum EHouseQuestion { HOUSE_QUESTION_NONE = 0, HOUSE_QUESTION_BUY = 1, HOUSE_QUESTION_RENT = 2, HOUSE_QUESTION_DRESS = 3 };
class CTCActionUseBuyHouse : public CTCActionUseBase {       // RTTI .?AVCTCActionUseBuyHouse@@ : CTCActionUseBase (0x14)
public:
    CDefPointer<const CBuyHouseDef> PDef;                    // +0x14
    bool           WaitingForConfirmation;                   // +0x18
    EHouseQuestion HouseQuestion;                            // +0x1c
private:
    void ReceivedConfirmation(CTCBuyableHouse& house);
};

void CTCActionUseBuyHouse::ReceivedConfirmation(CTCBuyableHouse& house)
{
    switch (HouseQuestion) {
    case HOUSE_QUESTION_BUY:
        if (!house.GetOwnedByPlayer()) {
            CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCHeroStats* stats;
            if (hero && hero->GetTC(TCI_HERO_STATS, &stats)) {
                stats->AddMoney(-house.GetPrice(), true);
                house.SetOwnedByPlayer(true);
                if (house.GetParentThing().HasTC(TCI_SHOP))
                    ++stats->NumberOfShopsOwned;
                else
                    ++stats->NumberOfHousesOwned;
            }
        } else {
            CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCHeroStats* stats;
            if (hero && hero->GetTC(TCI_HERO_STATS, &stats)) {
                stats->AddMoney(house.GetPrice(), true);
                house.SetOwnedByPlayer(false);
                if (house.GetRented())
                    house.SetRented(false);
                if (house.GetParentThing().HasTC(TCI_SHOP))
                    --stats->NumberOfShopsOwned;
                else
                    --stats->NumberOfHousesOwned;
            }
        }
        break;
    case HOUSE_QUESTION_RENT:
        house.SetRented(!house.GetRented());
        break;
    case HOUSE_QUESTION_DRESS: {
        CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCHeroStats* stats;
        if (hero && hero->GetTC(TCI_HERO_STATS, &stats)) {
            stats->AddMoney(-house.GetPriceForNextDressLevel(), true);
            house.IncrementDressLevel();
        }
        break;
    }
    }
}