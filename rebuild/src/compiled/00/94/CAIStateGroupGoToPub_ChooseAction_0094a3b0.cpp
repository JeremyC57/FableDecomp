// CAIStateGroup_GoToPub::ChooseAction(bool canDrink) -- pick the creature's next pub activity: sit idle while the table's game is
// being played; outside tavern time with drinking allowed keep drinking a non-empty tankard; thank the barmaid for a full tankard
// or drink one time in four; at a table with company listen / laugh / tell a story / sit idle at random; else drink, fall asleep
// or sit idle.
// Leaf: ?ChooseAction@CAIStateGroup_GoToPub@@IAE_N_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094A3B0, 785 bytes (`ret 4`, __fastcall). Ego_r.pdb / FableWin.pdb ?ChooseAction@CAIStateGroup_GoToPub@@IAE_N_N@Z.
// Callees: CVectorMap::LowerBound 0x0040F020 (inline GetTC 0x23/0xBA/0xCF/0xB8), CTCVillageMember::GetPVillage 0x007EE540,
//   CTCVillage::IsTavernTime 0x00658C0A, CBaseIntelligentPointer::GetPItem 0x00A01B50, Drink 0x00948880, ThankBarmaid 0x00949380,
//   CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890, Laugh 0x00947D30, Listen 0x0094A190, TellStory 0x0094A2B0,
//   FallAsleep 0x0094A0E0, IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340 (4th use, out of line), SitIdle 0x00947CA0.
// Shape notes: VC7.1 cross-jumps identical tails, so the source shape decides which copy survives. The "someone laughing" branch
//   repeats the 50/50 sit-idle / tell-story tail (merged into one copy by the compiler); the talking switch stores into `result`;
//   ThankBarmaid's `if (!ThankBarmaid()) return false;` and the rand%5 `case 0` falling into `default` keep the first Drink copy.
// No engine header for CAIStateGroup_GoToPub / CTCTavernTankard: local declarations as in the landed FreeSeat 0x009486D0.
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCTAVERNTABLE_SEATS
#define FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCVILLAGE_TAVERNTIMES
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
#include "engine/CThingObject.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCTavernTable.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
class CThingBuilding;

class CTCTavernTankard : public CTCBase { public:
    float Fullness;                                 // +0x0c
};

class CAIStateGroup_GoToPub : public CAIStateGroupBase {
public:
    CIntelligentPointer<CThingBuilding> PPub;       // +0x14
    CIntelligentPointer<CThingObject> PBar;         // +0x1c
    CIntelligentPointer<CThingObject> PTable;       // +0x24
    CIntelligentPointer<CThingAICreature> PBarman;  // +0x2c
    CIntelligentPointer<CThing> PTankard;           // +0x34
protected:
    bool SitIdle();                                 // 0x00947CA0
    bool Laugh();                                   // 0x00947D30
    bool Drink();                                   // 0x00948880
    bool ThankBarmaid();                            // 0x00949380
    bool FallAsleep();                              // 0x0094A0E0
    bool Listen();                                  // 0x0094A190
    bool TellStory();                               // 0x0094A2B0
    bool ChooseAction(bool canDrink);
};
bool CAIStateGroup_GoToPub::ChooseAction(bool canDrink)
{
    bool result;
    CTCVillageMember* villageMember;
    GetCreature().GetTC(TCI_VILLAGE_MEMBER, &villageMember);
    CTCVillage* village = villageMember->GetPVillage();
    CTCTavernTankard* tankard = 0;
    if (IS_THING_ALIVE(PTankard))
        PTankard.Get()->GetTC(TCI_TAVERN_TANKARD, &tankard);
    CTCTavernGame* game;
    if (PTable.Get()->GetTC(TCI_TAVERN_GAME, &game) && game->IsBeingPlayed)
        return SitIdle();
    if (!village->IsTavernTime() && canDrink && IS_THING_ALIVE(PTankard)) {
        if (tankard->Fullness == 0.0f)
            return false;
        return Drink();
    }
    if (IS_THING_ALIVE(PTankard) && tankard) {
        if (tankard->Fullness == 1.0f) {
            if (!ThankBarmaid())
                return false;
            return Drink();
        }
        if (GFRandom(4, GetPWorldSeed()) == 0)
            return Drink();
    }
    CTCTavernTable* table;
    PTable.Get()->GetTC(TCI_TAVERN_TABLE, &table);
    if (table->NumPeopleSeated > 1) {
        if (table->SomeoneTalking) {
            switch (GFRandom(4, GetPWorldSeed())) {
            case 0: result = SitIdle(); break;
            case 1: result = Listen(); break;
            case 2: result = Laugh(); break;
            default: result = TellStory(); break;
            }
        }
        else if (table->SomeoneLaughing) {
            if (GFRandom(2, GetPWorldSeed()) == 0)
                return Laugh();
            if (GFRandom(2, GetPWorldSeed()) == 0)
                return SitIdle();
            return TellStory();
        }
        else {
            if (GFRandom(2, GetPWorldSeed()) == 0)
                return SitIdle();
            return TellStory();
        }
        return result;
    }
    switch (GFRandom(5, GetPWorldSeed())) {
    case 1:
        return FallAsleep();
    case 0:
        if (IS_THING_ALIVE(PTankard))
            return Drink();
    default:
        return SitIdle();
    }
}