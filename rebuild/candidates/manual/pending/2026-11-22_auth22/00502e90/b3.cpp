// CWorldMap::PopulateRegionWithVillagers(CThing& village) -- populate a freshly loaded village region from its CVillagePeopleDef: tally the work places and homes of the village's buildings, create the teachers, shopkeepers, market traders, tavern staff, games masters, servants and apprentices, fill the remaining homes with workers, unemployed adults and children in the def's male / female / child proportions, pick a gay villager, house everyone, pair couples and parents, then add guards, ghosts and the dummy villagers that stand in for creatures until they are needed
// Grade: DIFFER(13606v13622) -- AUTH22 b3 (2026-11-22), STOCK flags, NO devices: the nine inline-depth pragma regions and the Ego_d-contradicted `bool listCreatures` of the wfE10-15 / p25S draft are gone. Instead 11 dead release ASSERT arms (fable_assert.h FABLE_RETAIL_ASSERT_DEAD_ARM) sit at the debug-only lines of Ego_d's line table before the servant block (helper line 4232; function lines 4487 x2, 4608 / 4613 / 4620, 4696, 4759, 4772 x2, 4801): their IL takes c2's caller size to 8974 at the servant site, so the a+g>9000 step penalty calls CreateRelevantPersonAndUpdateStats at the servant and apprentice sites as retail does (0x00504643 / 0x005047F2) and expands the six earlier ones. Apprentice indices as two ternary arguments (Ego_d line 4889, 184 B, no index locals). Call list by target = retail (476 v 476) except: loop-1 list push_back x2 (retail calls 0x004AC4B0), the female vector push_back (retail expands, 0x00504D90), the teacher / shop Create cross-jump. The arms' conditions are Inferred (positions from Ego_d, conditions unknown). See ESCALATE.md; earlier history in a0.cpp line 2.
// Leaf: ?PopulateRegionWithVillagers@CWorldMap@@QAEXAAVCThing@@@Z
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CSHOPDEF_STATGETTYPENAME_INLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETSEX
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_RELATIONS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_SCRIPTSUPERVISION
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_BULLETTIMEOVERRIDE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGBUILDING_PEEKSIMDEF
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#define FABLE_RETAIL_CTHINGCREATUREDEF_SEX_TYPED
#define FABLE_RETAIL_CTHINGBUILDING_RESIDENCY
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_GAYVILLAGERS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_POPULATE
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SAMPLEBANK_WORLDSEED
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CWORLDMAP_POPULATE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY_CALLS
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY_DISTANCE
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CSIMBUILDINGDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_GETPDEF
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCTALK_GETDIALOGUEVOICE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CTCBase.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCVillage.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSimBuildingDef.h"
#include "engine/CShopDef.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCTalk.h"
#include "engine/CTCDummyVillager.h"

namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b); }   // 0x009E9F40
#include "engine/GFRandom.h"
CCharString __fastcall GFIntToCharString(long value);                        // 0x0099F830
CCharString __fastcall operator+(const CCharString& a, const CCharString& b);    // 0x0099F570
extern CCharString g_CCharString_013bab2c;                                   // 0x013BAB2C the gay-candidate dialogue voice


enum ESimBuildingProperty
{
    SIM_BUILDING_HOME            = 0x001,
    SIM_BUILDING_WORKPLACE       = 0x002,
    SIM_BUILDING_SCHOOL          = 0x004,
    SIM_BUILDING_WORKSHOP        = 0x008,
    SIM_BUILDING_SHOP            = 0x010,
    SIM_BUILDING_TAVERN          = 0x020,
    SIM_BUILDING_MARKET          = 0x100,
    SIM_BUILDING_SERVANTS        = 0x200,
    SIM_BUILDING_APPRENTICES     = 0x400,
    SIM_BUILDING_GAMES           = 0x800,
};
enum ESex { SEX_NONE = 0, SEX_MALE = 1, SEX_FEMALE = 2 };
#include "engine/ECreatureType.h"

class CVillagePeopleDef : public CDefClassBase {                     // Ego_r.pdb members (retail = Ego_r: no container before them)
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CVillagePeopleDef", -1); }
    bool  Enable;                                                    // +0x25
    float DesiredMaleWeight;                                         // +0x28
    float DesiredFemaleWeight;                                       // +0x2c
    float DesiredChildWeight;                                        // +0x30
    float ProbabilityOfHomsexualityPerDialogueEnabledMan;            // +0x34
    long  MaleTeacher, FemaleTeacher;                                // +0x38
    long  MaleShopKeeper, FemaleShopKeeper;                          // +0x40
    long  MaleMarketTrader, FemaleMarketTrader;                      // +0x48
    long  MaleBarman, FemaleBarman;                                  // +0x50
    long  MaleBarmaid, FemaleBarmaid;                                // +0x58
    long  MaleWarehouseWorker, FemaleWarehouseWorker;                // +0x60
    long  MaleServant, FemaleServant;                                // +0x68
    long  MaleGoodApprentice, FemaleGoodApprentice;                  // +0x70
    long  MaleEvilApprentice, FemaleEvilApprentice;                  // +0x78
    long  MaleUnemployed, FemaleUnemployed;                          // +0x80
    long  MaleChild, FemaleChild;                                    // +0x88
    long  GamesMaster;                                               // +0x90
    long  Guard;                                                     // +0x94
    long  MaleGhost, FemaleGhost;                                    // +0x98
    long  NumGuards;                                                 // +0xa0
    long  NumGhosts;                                                 // +0xa4
};
class CBuyableHouseDef : public CDefClassBase {
public:
    bool IsBuyable;                                                  // +0x25
    bool InitiallyEmpty;                                             // +0x26
};
class CTCDNone {
public:
    static CThing* __fastcall Create(long defIndex, const C3DVector& pos, bool flag);   // 0x00835A20
};
#define FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY
#include "engine/CThingAICreatureInit.h"    // shared header (was a local model)

template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);  // ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z 0x0040DCE0

class CIsHouseLargerThan {
public:
    bool operator()(const CThingBuilding* a, const CThingBuilding* b) const;
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);    // 0x004978A0
extern "C" int __cdecl rand(void);                                   // 0x00BFEB16
struct RandFunc {
    unsigned long* Seed;
    long operator()(long n) { return (long)GFFloatRandom((float)n, Seed); }
};
namespace std {
template <class ForwardIter1, class ForwardIter2, class T>
inline void __iter_swap(ForwardIter1 i1, ForwardIter2 i2, T*)
{
    T tmp = *i1;
    *i1 = *i2;
    *i2 = tmp;
}
template <class ForwardIter1, class ForwardIter2>
inline void iter_swap(ForwardIter1 i1, ForwardIter2 i2)
{
    __iter_swap(i1, i2, i1);
}
template <class _Distance> inline _Distance __random_number(_Distance __n) { return rand() % __n; }
template <class RandomAccessIter>
void __fastcall random_shuffle(RandomAccessIter first, RandomAccessIter last)       // 0x005131D0
{
    if (first == last) return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        iter_swap(i, first + __random_number((i - first) + 1));
}
template <class RandomAccessIter, class RandomNumberGenerator>
void __fastcall random_shuffle(RandomAccessIter first, RandomAccessIter last, RandomNumberGenerator& rand)   // 0x00513210 / 0x00513270
{
    if (first == last) return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        iter_swap(i, first + rand((i - first) + 1));
}
template <class InputIter, class T>
InputIter __fastcall __find(InputIter first, InputIter last, const T& val, const input_iterator_tag&)   // 0x00511BF0 (landed std_find_00511bf0.cpp)
{
    while (first != last && !(*first == val))
        ++first;
    return first;
}
template <class InputIter, class T>
inline InputIter __fastcall find(InputIter first, InputIter last, const T& val)
{
    return __find(first, last, val, typename iterator_traits<InputIter>::iterator_category());
}
}

inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }
inline CThingBuilding* CThingAICreature::GetWorkBuilding() const { return WorkBuilding.Get(); }

typedef std::map<CThingBuilding*, std::list<CThingAICreature*> > CBuildingCreatureMap;
// 0x005029A0 (same TU; text = the landed row global_GetRelevantBuilding_005029a0.cpp on the shared headers): the building of the
// wanted kind with room left and the fewest occupants.
CThingBuilding* __fastcall GetRelevantBuilding(
    std::map<CThingBuilding*, std::list<CThingAICreature*> >& buildings, ESimBuildingProperty property, bool work, bool live)
{
    CThingBuilding* best = 0;
    std::map<CThingBuilding*, std::list<CThingAICreature*> >::iterator end = buildings.end();
    for (std::map<CThingBuilding*, std::list<CThingAICreature*> >::iterator it = buildings.begin(); it != end; ++it)
    {
        CDefPointer<const CSimBuildingDef> def = it->first->PeekPSimDefinition();
        if (!def.IsNull() && (def->Property & property))
        {
            if (!(work || live) || it->second.size() < (work ? def->WorkSpaces : def->LiveSpaces))
            {
                if (!best || buildings[it->first].size() < buildings[best].size())
                    best = it->first;
            }
        }
    }
    return best;
}
// 0x005003E0 (same TU; text = the landed row global_AddToWorkBuildingList_005003e0.cpp on the shared headers): register a worker
// with its work building, or drop a full building from the map.
void __fastcall AddToWorkBuildingList(CThingAICreature* creature,
    std::map<CThingBuilding*, std::list<CThingAICreature*> >& buildings, CThingBuilding* building)
{
    std::map<CThingBuilding*, std::list<CThingAICreature*> >::iterator it = buildings.find(building);
    CDefPointer<const CSimBuildingDef> def = it->first->PeekPSimDefinition();
    if (!def.IsNull()) {
        if (it->second.size() + 1 < (unsigned int)def->WorkSpaces)
            it->second.push_back(creature);
        else
            buildings.erase(it);
    }
}
// 0x00502B00 (same TU, defined above the row in retail; text = the landed row global_AddToRelevantBuilding_00502b00.cpp on the shared
// headers): give a villager a home. Its body is visible here so that VC7.1 sees it keeps neither the map nor the vector.
void __fastcall AddToRelevantBuilding(CThingAICreature* creature,
    std::map<CThingBuilding*, std::list<CThingAICreature*> >& occupants, std::vector<CThingBuilding*>& buildings,
    const CDefinitionManager* defs, CWorld& world)
{
    std::vector<CThingBuilding*>::iterator chosen = 0;
    long mostFree = 0;
    bool coin = GFRandom(2, world.GetWorldSeed()) == 0;
    std::vector<CThingBuilding*>::iterator it;
    for (it = buildings.begin(); it != buildings.end(); ++it) {
        CDefPointer<const CSimBuildingDef> def = (*it)->PeekPSimDefinition();
        if (def->LiveSpaces > 1 && occupants[*it].size() == 0) {
            chosen = it;
            break;
        }
    }
    if (chosen == 0) {
        for (it = buildings.begin(); it != buildings.end(); ++it) {
            CDefPointer<const CSimBuildingDef> def = (*it)->PeekPSimDefinition();
            long spaces = def->LiveSpaces - occupants[*it].size();
            if (spaces > 0) {
                if (occupants[*it].size() == 0) {
                    if (def->LiveSpaces > 1 || creature->GetCreatureType() == 1 || coin) {
                        chosen = it;
                        break;
                    }
                }
                long males = 0;
                long females = 0;
                for (std::list<CThingAICreature*>::iterator c = occupants[*it].begin(); c != occupants[*it].end(); ++c) {
                    if ((*c)->GetSex() == SEX_MALE)
                        ++males;
                    else if ((*c)->GetSex() == SEX_FEMALE)
                        ++females;
                }
                if ((females < males && creature->GetSex() == SEX_FEMALE) || (females > males && creature->GetSex() == SEX_MALE)) {
                    chosen = it;
                    break;
                }
                if (spaces > mostFree) {
                    mostFree = spaces;
                    chosen = it;
                }
            }
        }
    }
    if (chosen == 0)
        return;
    creature->SetHomeBuilding(*chosen);
    occupants[*chosen].push_back(creature);
    CDefPointer<const CSimBuildingDef> def = (*chosen)->PeekPSimDefinition();
    if (occupants[*chosen].size() == def->LiveSpaces)
        buildings.erase(chosen);
}


// 0x004FF750: create a villager of the under-represented sex (the same TU; VC7.1 expands it at the work-place sites).
CThingAICreature* __fastcall CreateRelevantPersonAndUpdateStats(float maleWeight, float femaleWeight, long maleDefIndex,
    long femaleDefIndex, long& noMales, long& noFemales, const C3DVector& pos, const CThingAICreatureInit& init,
    CTCVillage* village, std::vector<CThingAICreature*>& males, std::vector<CThingAICreature*>& females, bool find_house)
{
    CThingAICreature* creature = 0;
    FABLE_RETAIL_ASSERT(maleDefIndex || femaleDefIndex);
    if (noMales * femaleWeight < noFemales * maleWeight)
    {
        if (maleDefIndex)
        {
            creature = CThingAICreature::Create(maleDefIndex, pos, 4, init);
            if (find_house) males.push_back(creature);
            ++noMales;
        }
        else if (femaleDefIndex)
        {
            creature = CThingAICreature::Create(femaleDefIndex, pos, 4, init);
            if (find_house) females.push_back(creature);
            ++noFemales;
        }
    }
    else
    {
        if (femaleDefIndex)
        {
            creature = CThingAICreature::Create(femaleDefIndex, pos, 4, init);
            if (find_house) females.push_back(creature);
            ++noFemales;
        }
        else if (maleDefIndex)
        {
            creature = CThingAICreature::Create(maleDefIndex, pos, 4, init);
            if (find_house) males.push_back(creature);
            ++noMales;
        }
    }
    if (creature)
    {
        creature->SetAsAllowedToFollowHero(true);
        creature->FollowHeroThroughTeleporters = true;
        village->AddThingToVillage(*creature);
        if (!find_house)
        {
            CDefPointer<const CVillagePeopleDef> peopleDef;
            if (village->ParentThing->PeekBaseDef().GetSubDef(&peopleDef))
                creature->AutoSetupSexuality(peopleDef->ProbabilityOfHomsexualityPerDialogueEnabledMan);
        }
    }
    return creature;
}

void CWorldMap::PopulateRegionWithVillagers(CThing& village)
{
    CDefPointer<const CVillagePeopleDef> peopleDef;
    if (village.PeekBaseDef().GetSubDef(&peopleDef))
    {
        FABLE_RETAIL_ASSERT(!peopleDef.IsNull());
        FABLE_RETAIL_ASSERT(village.HasTC((ETCInterfaceType)34));
        CTCVillage* villageTC;
        village.GetTC((ETCInterfaceType)34, &villageTC);

        if (World.PopDontPopulateNextLoadedRegion() || !peopleDef->Enable)
            return;

        CBuildingCreatureMap homeBuildings;
        CBuildingCreatureMap workBuildings;

        float maleWeight = peopleDef->DesiredMaleWeight;
        float femaleWeight = peopleDef->DesiredFemaleWeight;
        float childWeight = peopleDef->DesiredChildWeight;
        float totalWeight = childWeight + femaleWeight + maleWeight;
        float normaliser = 1.0f / totalWeight;
        maleWeight *= normaliser;
        femaleWeight *= normaliser;
        childWeight *= normaliser;

        std::vector<CThingAICreature*> males;
        std::vector<CThingAICreature*> females;
        std::vector<CThingAICreature*> children;
        std::vector<CThingBuilding*> houses;
        std::vector<CThingBuilding*> emptyHouses;

        long noGuards = peopleDef->NumGuards;
        long noGhosts = peopleDef->NumGhosts;
        long homeSpaces = 0;
        long schoolSpaces = 0;
        long workshopSpaces = 0;
        long shopSpaces = 0;
        long marketSpaces = 0;
        long barmanSpaces = 0;
        long barmaidSpaces = 0;
        long gamesMasterSpaces = 0;
        long servantSpaces = 0;
        long apprenticeSpaces = 0;
        long noMales = 0;
        long noFemales = 0;
        long noChildren = 0;
        long noBoys = 0;
        long noGirls = 0;

        std::list<CIntelligentPointer<CThingAICreature> > originalVillagers(villageTC->VillagePeople);

        // Existing villagers: drop the guards' buildings, register everyone else with their home and work place.
        std::list<CIntelligentPointer<CThingAICreature> >::iterator end = villageTC->VillagePeople.end();
        for (std::list<CIntelligentPointer<CThingAICreature> >::iterator it = villageTC->VillagePeople.begin();
             it != end; )
        {
            {
                NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 1", -1.0f, false, false);
                CIntelligentPointer<CThingAICreature> villager = *it;
                if (villager->GetProperty() != 3)
                {
                    if ((*it)->GetWorkBuilding())
                    {
                        CThingAICreature* worker = (*it).Get();
                        CThingBuilding* work = (*it)->GetWorkBuilding();
                        // Inline-budget: retail calls list<CThingAICreature*>::push_back 0x004AC4B0 out of line in this loop
                        workBuildings[work].push_back(worker);
                        (*it)->GetWorkBuilding()->PeekPSimDefinition();
                    }
                    if (villager->GetSex() == SEX_MALE)
                    {
                        if (villager->GetHomeBuilding())
                            ++noMales;
                        villager->GetHomeBuilding();
                    }
                    else if (villager->GetSex() == SEX_FEMALE)
                    {
                        if (villager->GetHomeBuilding())
                            ++noFemales;
                        villager->GetHomeBuilding();
                    }
                    else
                    {
                        if (villager->GetHomeBuilding())
                            ++noChildren;
                        (*it)->GetHomeBuilding();
                    }
                    if ((*it)->GetHomeBuilding())
                    {
                        CThingAICreature* resident = (*it).Get();
                        CThingBuilding* home = (*it)->GetHomeBuilding();
                        homeBuildings[home].push_back(resident);
                    }
                }
                else
                {
                    if (villager->GetHomeBuilding())
                        { FABLE_RETAIL_ASSERT(0); (*it)->SetHomeBuilding(0); }
                    if (villager->GetWorkBuilding())
                        { FABLE_RETAIL_ASSERT(0); villager->SetWorkBuilding(0); }
                    if (villager->IsUnderScriptSupervision()) { FABLE_RETAIL_ASSERT(0); }
                }
            }
            ++it;
        }

        // The village's buildings: free homes and work places by kind.
        CThingBuilding* school = 0;
        std::list<CIntelligentPointer<CThingBuilding> >::iterator bend = villageTC->VillageBuildings.end();
        for (std::list<CIntelligentPointer<CThingBuilding> >::iterator b = villageTC->VillageBuildings.begin();
             b != bend; ++b)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 2", -1.0f, false, false);
            CThingBuilding* building = b->Get();
            if (!building || !building->GetIsAvailableForResidency())
                continue;
            CTCBuyableHouse* buyable;
            bool initiallyEmpty = building->GetTC((ETCInterfaceType)213, &buyable) && buyable->GetPDef()->InitiallyEmpty;
            if (initiallyEmpty)
            {
                emptyHouses.push_back(building);
                continue;
            }
            CDefPointer<const CSimBuildingDef> simDef = building->PeekPSimDefinition();
            if (simDef.IsNull())
                continue;
            if (simDef->Property & SIM_BUILDING_HOME)
            {
                long freeSpaces = simDef->LiveSpaces - (long)homeBuildings[building].size();
                if (freeSpaces > 0)
                {
                    houses.push_back(building);
                    homeSpaces += freeSpaces;
                }
                else
                    homeBuildings.erase(building);
            }
            if (simDef->Property & SIM_BUILDING_WORKPLACE)
            {
                long freeSpaces = simDef->WorkSpaces - (long)workBuildings[building].size();
                if (freeSpaces > 0)
                {
                    long property = simDef->Property;
                    if (property & SIM_BUILDING_SCHOOL)
                    {
                        schoolSpaces += freeSpaces;
                        school = building;
                    }
                    else if (property & SIM_BUILDING_WORKSHOP)
                        workshopSpaces += freeSpaces;
                    else if ((property & SIM_BUILDING_SHOP) && !(property & SIM_BUILDING_MARKET))
                        shopSpaces += freeSpaces;
                    else if (property & SIM_BUILDING_MARKET)
                        marketSpaces += freeSpaces;
                    else if (property & SIM_BUILDING_TAVERN)
                    {
                        CTCThingOwner* owner;
                        if (building->GetTC((ETCInterfaceType)33, &owner))
                        {
                            std::list<CIntelligentPointer<CThing> >::iterator gend = owner->GetObjectList().end();
                            for (std::list<CIntelligentPointer<CThing> >::iterator g = owner->GetObjectList().begin(); g != gend; ++g)
                            {
                                CThing* game = g->Get();
                                if (IS_THING_ALIVE(game) && game->HasTC((ETCInterfaceType)207))
                                {
                                    ++gamesMasterSpaces;
                                    --freeSpaces;
                                }
                            }
                        }
                        FABLE_RETAIL_ASSERT(freeSpaces > 0);
                        barmanSpaces = 1;
                        barmaidSpaces = freeSpaces - 1;
                    }
                    else if (property & SIM_BUILDING_GAMES)
                    {
                        CTCThingOwner* owner;
                        if (building->GetTC((ETCInterfaceType)33, &owner))
                        {
                            std::list<CIntelligentPointer<CThing> >::iterator gend = owner->GetObjectList().end();
                            for (std::list<CIntelligentPointer<CThing> >::iterator g = owner->GetObjectList().begin(); g != gend; ++g)
                            {
                                CThing* game = g->Get();
                                if (IS_THING_ALIVE(game) && game->HasTC((ETCInterfaceType)207))
                                    ++gamesMasterSpaces;
                            }
                        }
                    }
                    else if (property & SIM_BUILDING_SERVANTS)
                        servantSpaces += freeSpaces;
                    else if (property & SIM_BUILDING_APPRENTICES)
                        apprenticeSpaces += freeSpaces;
                }
            }
        }

        // Teachers.
        while (schoolSpaces > 0)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 3", -1.0f, false, false);
            CThingAICreatureInit init(0, 0, false, "", "", true);
            init.EnableCreatureAutoPlacing = true;
            init.WorkBuilding = GetRelevantBuilding(workBuildings, SIM_BUILDING_SCHOOL, true, false);
            if (!init.WorkBuilding)
                break;
            CThingAICreature* creature = CreateRelevantPersonAndUpdateStats(maleWeight, femaleWeight, peopleDef->MaleTeacher,
                peopleDef->FemaleTeacher, noMales, noFemales, village.GetPos(), init, villageTC, males, females, true);
            creature->SetAsAllowedToFollowHero(false);
            AddToWorkBuildingList(creature, workBuildings, init.WorkBuilding);
            --schoolSpaces;
            --homeSpaces;
        }

        // Shopkeepers: the shop def's preferred shopkeeper when it names one.
        while (shopSpaces > 0)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 4", -1.0f, false, false);
            CThingAICreatureInit init(0, 0, false, "", "", true);
            init.EnableCreatureAutoPlacing = true;
            init.WorkBuilding = GetRelevantBuilding(workBuildings, SIM_BUILDING_SHOP, true, false);
            if (!init.WorkBuilding)
                break;
            FABLE_RETAIL_ASSERT(peopleDef->MaleShopKeeper || peopleDef->FemaleShopKeeper);
            long maleDefIndex = peopleDef->MaleShopKeeper;
            long femaleDefIndex = peopleDef->FemaleShopKeeper;
            CDefPointer<const CShopDef> shopDef;
            if (init.WorkBuilding->PeekBaseDef().GetSubDef(&shopDef) && shopDef->PreferredShopkeeper)
            {
                CDefPointer<const CThingCreatureDef> keeperDef;
                FABLE_RETAIL_ASSERT(shopDef->PreferredShopkeeper);
                villageTC->ParentThing->PeekDefinitionManager().GetDef(shopDef->PreferredShopkeeper, &keeperDef);
                FABLE_RETAIL_ASSERT(!keeperDef.IsNull());
                if (keeperDef->Sex == SEX_MALE)
                {
                    maleDefIndex = shopDef->PreferredShopkeeper;
                    femaleDefIndex = 0;
                }
                else
                {
                    maleDefIndex = 0;
                    femaleDefIndex = shopDef->PreferredShopkeeper;
                }
            }
            CThingAICreature* creature = CreateRelevantPersonAndUpdateStats(maleWeight, femaleWeight, maleDefIndex,
                femaleDefIndex, noMales, noFemales, village.GetPos(), init, villageTC, males, females, true);
            AddToWorkBuildingList(creature, workBuildings, init.WorkBuilding);
            --shopSpaces;
            --homeSpaces;
        }

        // Market traders.
        while (marketSpaces > 0)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 5", -1.0f, false, false);
            CThingAICreatureInit init(0, 0, false, "", "", true);
            init.EnableCreatureAutoPlacing = true;
            init.WorkBuilding = GetRelevantBuilding(workBuildings, SIM_BUILDING_MARKET, true, false);
            if (!init.WorkBuilding)
                break;
            FABLE_RETAIL_ASSERT(peopleDef->MaleMarketTrader || peopleDef->FemaleMarketTrader);
            CThingAICreature* creature = CreateRelevantPersonAndUpdateStats(maleWeight, femaleWeight, peopleDef->MaleMarketTrader,
                peopleDef->FemaleMarketTrader, noMales, noFemales, village.GetPos(), init, villageTC, males, females, true);
            AddToWorkBuildingList(creature, workBuildings, init.WorkBuilding);
            --marketSpaces;
            --homeSpaces;
        }

        // Barman.
        while (barmanSpaces > 0)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 6", -1.0f, false, false);
            CThingAICreatureInit init(0, 0, false, "", "", true);
            init.EnableCreatureAutoPlacing = true;
            init.WorkBuilding = GetRelevantBuilding(workBuildings, SIM_BUILDING_TAVERN, true, false);
            if (!init.WorkBuilding)
                break;
            CThingAICreature* creature = CreateRelevantPersonAndUpdateStats(maleWeight, femaleWeight, peopleDef->MaleBarman,
                peopleDef->FemaleBarman, noMales, noFemales, village.GetPos(), init, villageTC, males, females, false);
            AddToWorkBuildingList(creature, workBuildings, init.WorkBuilding);
            --barmanSpaces;
        }

        // Barmaids.
        while (barmaidSpaces > 0)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 7", -1.0f, false, false);
            CThingAICreatureInit init(0, 0, false, "", "", true);
            init.EnableCreatureAutoPlacing = true;
            init.WorkBuilding = GetRelevantBuilding(workBuildings, SIM_BUILDING_TAVERN, true, false);
            if (!init.WorkBuilding)
                break;
            CThingAICreature* creature = CreateRelevantPersonAndUpdateStats(maleWeight, femaleWeight, peopleDef->MaleBarmaid,
                peopleDef->FemaleBarmaid, noMales, noFemales, village.GetPos(), init, villageTC, males, females, false);
            AddToWorkBuildingList(creature, workBuildings, init.WorkBuilding);
            --barmaidSpaces;
        }

        // Games masters (a tavern's games first, else a games building).
        while (gamesMasterSpaces > 0)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 8", -1.0f, false, false);
            CThingAICreatureInit init(0, 0, false, "", "", true);
            init.EnableCreatureAutoPlacing = true;
            init.WorkBuilding = GetRelevantBuilding(workBuildings, SIM_BUILDING_TAVERN, false, false);
            if (!init.WorkBuilding)
            {
                init.WorkBuilding = GetRelevantBuilding(workBuildings, SIM_BUILDING_GAMES, false, false);
                if (!init.WorkBuilding)
                    break;
            }
            CThingAICreature* creature = CreateRelevantPersonAndUpdateStats(maleWeight, femaleWeight, peopleDef->GamesMaster,
                0, noMales, noFemales, village.GetPos(), init, villageTC, males, females, false);
            AddToWorkBuildingList(creature, workBuildings, init.WorkBuilding);
            --gamesMasterSpaces;
        }

        // Servants.
        while (servantSpaces > 0)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 9", -1.0f, false, false);
            CThingAICreatureInit init(0, 0, false, "", "", true);
            init.EnableCreatureAutoPlacing = true;
            init.WorkBuilding = GetRelevantBuilding(workBuildings, SIM_BUILDING_SERVANTS, true, false);
            if (!init.WorkBuilding)
                break;
            CThingAICreature* creature = CreateRelevantPersonAndUpdateStats(maleWeight, femaleWeight, peopleDef->MaleServant,
                peopleDef->FemaleServant, noMales, noFemales, village.GetPos(), init, villageTC, males, females, true);
            AddToWorkBuildingList(creature, workBuildings, init.WorkBuilding);
            --servantSpaces;
            --homeSpaces;
        }

        // Apprentices, good or evil at random.
        while (apprenticeSpaces > 0)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 10", -1.0f, false, false);
            CThingAICreatureInit init(0, 0, false, "", "", true);
            init.EnableCreatureAutoPlacing = true;
            init.WorkBuilding = GetRelevantBuilding(workBuildings, SIM_BUILDING_APPRENTICES, true, false);
            if (!init.WorkBuilding)
                break;
            bool evil_apprentice = (GFRandom(0xffffffff, World.GetWorldSeed()) & 1) != 0;
            CThingAICreature* creature = CreateRelevantPersonAndUpdateStats(maleWeight, femaleWeight,
                evil_apprentice ? peopleDef->MaleEvilApprentice : peopleDef->MaleGoodApprentice,
                evil_apprentice ? peopleDef->FemaleEvilApprentice : peopleDef->FemaleGoodApprentice,
                noMales, noFemales, village.GetPos(), init, villageTC, males, females, true);
            AddToWorkBuildingList(creature, workBuildings, init.WorkBuilding);
            --apprenticeSpaces;
            --homeSpaces;
        }

        if (homeSpaces < 0)
            return;

        // Fill the remaining homes: workers or unemployed adults of the sex furthest below its share, else children.
        long population = noChildren + noFemales + noMales + homeSpaces;
        if (homeSpaces)
        {
            float desiredMales = maleWeight * population;
            float desiredFemales = femaleWeight * population;
            float desiredChildren = population * childWeight;
            long remaining = homeSpaces;
            while (remaining--)
            {
                NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 11", -1.0f, false, false);
                float maleShortfall = desiredMales - noMales;
                float femaleShortfall = desiredFemales - noFemales;
                float childShortfall = desiredChildren - noChildren;
                if (maleWeight == 0.0f)
                    maleShortfall = -1000.0f;
                if (femaleWeight == 0.0f)
                    femaleShortfall = -1000.0f;
                if (childWeight == 0.0f)
                    childShortfall = -1000.0f;
                CThingAICreature* creature;
                if (maleShortfall > femaleShortfall && maleShortfall > childShortfall &&
                    (peopleDef->MaleWarehouseWorker || peopleDef->MaleUnemployed))
                {
                    if (workshopSpaces > 0 && peopleDef->MaleWarehouseWorker)
                    {
                        CThingAICreatureInit init(0, 0, false, "", "", true);
                        init.EnableCreatureAutoPlacing = true;
                        init.WorkBuilding = GetRelevantBuilding(workBuildings, SIM_BUILDING_WORKSHOP, true, false);
                        creature = CThingAICreature::Create(peopleDef->MaleWarehouseWorker, village.GetPos(), 4, init);
                        AddToWorkBuildingList(creature, workBuildings, init.WorkBuilding);
                        --workshopSpaces;
                    }
                    else
                    {
                        CThingAICreatureInit init(0, 0, false, "", "", true);
                        init.EnableCreatureAutoPlacing = true;
                        creature = CThingAICreature::Create(peopleDef->MaleUnemployed, village.GetPos(), 4, init);
                    }
                    if (creature)
                        males.push_back(creature);
                    ++noMales;
                }
                else if (femaleShortfall > childShortfall && (peopleDef->FemaleWarehouseWorker || peopleDef->FemaleUnemployed))
                {
                    if (workshopSpaces > 0 && peopleDef->FemaleWarehouseWorker)
                    {
                        CThingAICreatureInit init(0, 0, false, "", "", true);
                        init.EnableCreatureAutoPlacing = true;
                        init.WorkBuilding = GetRelevantBuilding(workBuildings, SIM_BUILDING_WORKSHOP, true, false);
                        creature = CThingAICreature::Create(peopleDef->FemaleWarehouseWorker, village.GetPos(), 4, init);
                        AddToWorkBuildingList(creature, workBuildings, init.WorkBuilding);
                        --workshopSpaces;
                    }
                    else
                    {
                        CThingAICreatureInit init(0, 0, false, "", "", true);
                        init.EnableCreatureAutoPlacing = true;
                        creature = CThingAICreature::Create(peopleDef->FemaleUnemployed, village.GetPos(), 4, init);
                    }
                    if (creature)
                        females.push_back(creature);
                    ++noFemales;
                }
                else if (peopleDef->FemaleChild && peopleDef->MaleChild)
                {
                    CThingAICreatureInit init(0, 0, false, "", "", true);
                    init.EnableCreatureAutoPlacing = true;
                    init.WorkBuilding = school;
                    unsigned long coin = GFRandom(2, CMainGameComponent::Get()->GetWorldSeed());
                    if ((!coin || !noGirls) && noBoys > 0)
                    {
                        creature = CThingAICreature::Create(peopleDef->FemaleChild, village.GetPos(), 4, init);
                        creature->SetScriptName(CCharString("girl") + GFIntToCharString(noGirls));
                        ++noGirls;
                    }
                    else
                    {
                        creature = CThingAICreature::Create(peopleDef->MaleChild, village.GetPos(), 4, init);
                        creature->SetScriptName(CCharString("boy") + GFIntToCharString(noBoys));
                        ++noBoys;
                    }
                    if (creature)
                        children.push_back(creature);
                    ++noChildren;
                }
                else
                    continue;
                if (creature)
                {
                    creature->SetAsAllowedToFollowHero(true);
                    creature->FollowHeroThroughTeleporters = true;
                    villageTC->AddThingToVillage(*creature);
                }
            }
        }

        // Sexuality of the new adults.
        for (std::vector<CThingAICreature*>* adults = &males; ; adults = &females)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 12", -1.0f, false, false);
            for (std::vector<CThingAICreature*>::iterator a = adults->begin(); a != adults->end(); ++a)
            {
                if (*a && !(*a)->IsDead() && !(*a)->IsUnderScriptSupervision())
                    (*a)->AutoSetupSexuality(peopleDef->ProbabilityOfHomsexualityPerDialogueEnabledMan);
            }
            if (adults != &males)
                break;
        }

        // Make sure the village has a gay man with the right dialogue voice when the def allows one.
        if (peopleDef->ProbabilityOfHomsexualityPerDialogueEnabledMan > 0.0f && villageTC->GetNumberOfGayVillagers() <= 0)
        {
            std::vector<CThingAICreature*> candidates(males);
            std::random_shuffle(candidates.begin(), candidates.end());
            for (std::vector<CThingAICreature*>::iterator c = candidates.begin(); c != candidates.end(); ++c)
            {
                CTCTalk* talk;
                bool canTalk = *c && !(*c)->IsDead() && (*c)->PeekTC((ETCInterfaceType)15, (const CTCTalk**)&talk) &&
                               talk->GetDialogueVoice() == g_CCharString_013bab2c;
                if (canTalk && !(*c)->IsUnderScriptSupervision())
                {
                    (*c)->AutoSetupSexuality(1.0f);
                    break;
                }
            }
        }

        // Adults whose work building is also a free home live there.
        for (std::vector<CThingAICreature*>* adults = &males; ; adults = &females)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 13", -1.0f, false, false);
            for (std::vector<CThingAICreature*>::iterator a = adults->begin(); a != adults->end(); )
            {
                CThingBuilding* work = (*a)->GetWorkBuilding();
                if (homeBuildings.find(work) != homeBuildings.end())
                {
                    std::vector<CThingBuilding*>::iterator h;
                    bool found = false;
                    for (h = houses.begin(); h != houses.end(); ++h)
                        if (*h == work)
                        {
                            found = true;
                            break;
                        }
                    if (found)
                    {
                        (*a)->SetHomeBuilding(work);
                        homeBuildings[work].push_back(*a);
                        {
                            CDefPointer<const CSimBuildingDef> simDef = work->PeekPSimDefinition();
                            if ((long)homeBuildings[work].size() == simDef->LiveSpaces)
                                houses.erase(h);
                        }
                        a = adults->erase(a);
                        continue;
                    }
                }
                ++a;
            }
            if (adults != &males)
                break;
        }

        // Shuffle, then fill the largest houses first: a woman, a man, a child at a time.
        RandFunc rand;
        rand.Seed = CMainGameComponent::Get()->GetWorldSeed();
        std::random_shuffle(houses.begin(), houses.end(), rand);
        std::random_shuffle(males.begin(), males.end(), rand);
        std::random_shuffle(females.begin(), females.end(), rand);
        std::random_shuffle(children.begin(), children.end(), rand);
        CIsHouseLargerThan larger;
        std::sort(houses.begin(), houses.end(), larger);
        for (std::vector<CThingBuilding*>::iterator h = houses.begin(); h != houses.end(); ++h)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 14", -1.0f, false, false);
            long freeSpaces = 0;
            if ((*h)->PeekPSimDefinition().Get())
                freeSpaces = (*h)->PeekPSimDefinition()->LiveSpaces;
            freeSpaces -= (long)homeBuildings[*h].size();
            if (freeSpaces)
            while ((females.size() || males.size() || children.size()) && freeSpaces)
            {
                if (females.size())
                {
                    CThingAICreature* female = females.back();
                    female->SetHomeBuilding(*h);
                    homeBuildings[*h].push_back(females.back());
                    females.pop_back();
                    --freeSpaces;
                }
                if (!freeSpaces)
                    break;
                if (males.size())
                {
                    CThingAICreature* male = males.back();
                    male->SetHomeBuilding(*h);
                    homeBuildings[*h].push_back(males.back());
                    males.pop_back();
                    --freeSpaces;
                }
                if (!freeSpaces)
                    break;
                if (children.size())
                {
                    CThingAICreature* child = children.back();
                    child->SetHomeBuilding(*h);
                    homeBuildings[*h].push_back(children.back());
                    children.pop_back();
                    --freeSpaces;
                }
                if (!freeSpaces)
                    break;
            }
        }

        // Whoever is left goes to the most suitable house.
        for (unsigned long i = 0; i < males.size(); ++i)
        {
            if (i >= females.size() || i >= children.size())
                break;
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 15", -1.0f, false, false);
            AddToRelevantBuilding(females[i], homeBuildings, houses, &Component.PeekDefinitionManager(), World);
            if (i < males.size())
                AddToRelevantBuilding(males[i], homeBuildings, houses, &Component.PeekDefinitionManager(), World);
            AddToRelevantBuilding(children[i], homeBuildings, houses, &Component.PeekDefinitionManager(), World);
        }

        // Marry the single adults of each home and make them the parents of its children.
        for (CBuildingCreatureMap::iterator home = homeBuildings.begin(); home != homeBuildings.end(); ++home)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 16", -1.0f, false, false);
            CThingAICreature* singleMale = 0;
            CThingAICreature* singleFemale = 0;
            CThingAICreature* father = 0;
            CThingAICreature* mother = 0;
            std::list<CThingAICreature*>::iterator cend = home->second.end();
            for (std::list<CThingAICreature*>::iterator c = home->second.begin(); c != cend; ++c)
            {
                CThingAICreature* creature = *c;
                if (!creature || creature->GetCreatureType() != HUMAN_ADULT)
                    continue;
                if (creature->GetSex() == SEX_MALE && !creature->GetSpouseCreature())
                {
                    if (singleFemale)
                    {
                        singleFemale->SetSpouseCreature(creature);
                        creature->SetSpouseCreature(singleFemale);
                        mother = singleFemale;
                        father = creature;
                        singleFemale = 0;
                    }
                    else
                        singleMale = creature;
                }
                else if (creature->GetSex() == SEX_FEMALE && !creature->GetSpouseCreature())
                {
                    if (singleMale)
                    {
                        singleMale->SetSpouseCreature(creature);
                        creature->SetSpouseCreature(singleMale);
                        father = singleMale;
                        mother = creature;
                        singleMale = 0;
                    }
                    else
                        singleFemale = creature;
                }
            }
            if (mother || father)
            {
                std::list<CThingAICreature*>::iterator cend = home->second.end();
                for (std::list<CThingAICreature*>::iterator c = home->second.begin(); c != cend; ++c)
                {
                    CThingAICreature* creature = *c;
                    if (creature && creature->GetCreatureType() == HUMAN_CHILD &&
                        !creature->GetFatherCreature() && !creature->GetMotherCreature())
                    {
                        if (father)
                            creature->SetFatherCreature(father);
                        if (mother)
                            creature->SetMotherCreature(mother);
                    }
                }
            }
        }

        // Guards.
        if (peopleDef->Guard && noGuards > 0)
        {
            for (long n = noGuards; n; --n)
            {
                NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 17", -1.0f, false, false);
                CThingAICreatureInit init(0, 0, false, "", "", true);
                init.EnableCreatureAutoPlacing = true;
                CThingAICreature* guard = CThingAICreature::Create(peopleDef->Guard, village.GetPos(), 4, init);
                if (guard && !guard->IsDead())
                {
                    guard->SetAsAllowedToFollowHero(true);
                    guard->FollowHeroThroughTeleporters = true;
                    villageTC->AddThingToVillage(*guard);
                }
            }
        }

        // Ghosts.
        if ((peopleDef->MaleGhost || peopleDef->FemaleGhost) && noGhosts > 0)
        {
            for (long n = noGhosts; n; --n)
            {
                NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 17.5", -1.0f, false, false);
                CThingAICreatureInit init(0, 0, false, "", "", true);
                init.EnableCreatureAutoPlacing = true;
                long ghostDef;
                if (peopleDef->MaleGhost && (!peopleDef->FemaleGhost || !(GFRandom(2, CMainGameComponent::Get()->GetWorldSeed()))))
                    ghostDef = peopleDef->MaleGhost;
                else
                    ghostDef = peopleDef->FemaleGhost;
                CThingAICreature* ghost = CThingAICreature::Create(ghostDef, village.GetPos(), 4, init);
                if (ghost && !ghost->IsDead())
                {
                    ghost->SetAsAllowedToFollowHero(true);
                    ghost->FollowHeroThroughTeleporters = true;
                    villageTC->AddThingToVillage(*ghost);
                }
            }
        }

        // Dummy villagers for the initially empty houses: they stand in until the house is bought.
        for (std::vector<CThingBuilding*>::iterator e = emptyHouses.begin(); e != emptyHouses.end(); ++e)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 18", -1.0f, false, false);
            CThingBuilding* house = *e;
            CDefPointer<const CSimBuildingDef> simDef = house->PeekPSimDefinition();
            if (!simDef.IsNull() && (simDef->Property & SIM_BUILDING_HOME) && simDef->LiveSpaces)
            {
                float desiredMales = maleWeight * population;
                float desiredFemales = femaleWeight * population;
                float desiredChildren = population * childWeight;
                for (long n = simDef->LiveSpaces; n; --n)
                {
                    float maleShortfall = desiredMales - noMales;
                    float femaleShortfall = desiredFemales - noFemales;
                    float childShortfall = desiredChildren - noChildren;
                    if (maleWeight == 0.0f)
                        maleShortfall = -1000.0f;
                    if (femaleWeight == 0.0f)
                        femaleShortfall = -1000.0f;
                    if (childWeight == 0.0f)
                        childShortfall = -1000.0f;
                    CThing* dummy = CTCDNone::Create(World.PeekDefinitionManager().GetDefGlobalIndexFromName("DUMMY_VILLAGER"), village.GetPos(), false);
                    CTCDummyVillager* dummyTC = 0;
                    dummy->GetTC((ETCInterfaceType)214, &dummyTC);
                    if (maleShortfall > femaleShortfall && maleShortfall > childShortfall)
                    {
                        if (workshopSpaces > 0 && peopleDef->MaleWarehouseWorker)
                        {
                            dummyTC->SetupFromData(peopleDef->MaleWarehouseWorker, house,
                                                   GetRelevantBuilding(workBuildings, SIM_BUILDING_WORKSHOP, true, false), false);
                            --workshopSpaces;
                        }
                        else
                            dummyTC->SetupFromData(peopleDef->MaleUnemployed, house, 0, false);
                        ++noMales;
                    }
                    else if (femaleShortfall > childShortfall)
                    {
                        if (workshopSpaces > 0 && peopleDef->FemaleWarehouseWorker)
                        {
                            dummyTC->SetupFromData(peopleDef->FemaleWarehouseWorker, house,
                                                   GetRelevantBuilding(workBuildings, SIM_BUILDING_WORKSHOP, true, false), false);
                            --workshopSpaces;
                        }
                        else
                            dummyTC->SetupFromData(peopleDef->FemaleUnemployed, house, 0, false);
                        ++noFemales;
                    }
                    else
                    {
                        if ((!(GFRandom(2, CMainGameComponent::Get()->GetWorldSeed())) || !noGirls) && noBoys > 0)
                        {
                            dummyTC->SetupFromData(peopleDef->FemaleChild, house, school, false);
                            ++noGirls;
                        }
                        else
                        {
                            dummyTC->SetupFromData(peopleDef->MaleChild, house, school, false);
                            ++noBoys;
                        }
                        ++noChildren;
                    }
                    CTCOwnedEntity* owned = 0;
                    dummy->GetTC((ETCInterfaceType)51, &owned);
                    owned->SetPOwner(house);
                    villageTC->AddThingToVillage(*dummy);
                }
            }
        }

        // Every villager that was here before gets a dummy villager that can recreate it.
        for (std::list<CIntelligentPointer<CThingAICreature> >::iterator v = villageTC->VillagePeople.begin();
             v != villageTC->VillagePeople.end(); ++v)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 19", -1.0f, false, false);
            CThingAICreature* villager = v->Get();
            if (villager && !villager->IsDead() &&
                std::find(originalVillagers.begin(), originalVillagers.end(), villager) == originalVillagers.end())
            {
                CThing* dummy = CTCDNone::Create(World.PeekDefinitionManager().GetDefGlobalIndexFromName("DUMMY_VILLAGER"), village.GetPos(), false);
                CTCDummyVillager* dummyTC = 0;
                dummy->GetTC((ETCInterfaceType)214, &dummyTC);
                dummyTC->SetupFromVillager(*villager, false);
                villageTC->AddThingToVillage(*dummy);
            }
        }
    }
}
