// CWorldMap::PopulateRegionWithVillagers(CThing& village) -- populate a freshly loaded village region from its CVillagePeopleDef: tally the work places and homes of the village's buildings, create the teachers, shopkeepers, market traders, tavern staff, games masters, servants and apprentices, fill the remaining homes with workers, unemployed adults and children in the def's male / female / child proportions, pick a gay villager, house everyone, pair couples and parents, then add guards, ghosts and the dummy villagers that stand in for creatures until they are needed
// DIFFER(13821v13622) md 2902 / mdf 1434 (v21, 54 grades): call sequence matches retail in every DisplayProgress segment except 2 inline-budget sites (P9 CreateRelevantPersonAndUpdateStats 0x004FF750 inline in ours, called in retail; P11 female vector<CThingAICreature*>::push_back called 0x00517A10 in ours, inline in retail) -- forcing P9 out (inline_depth(0)) frees budget and cascades P12 vector copy / P14 SetHomeBuilding inline (v18 md 3056); rest is c2prio colouring (esi=0 / edi=peopleDef swapped) and frame 0x19C v 0x194
// Next: find a spelling that both outlines P9 and keeps P12/P14 out (e.g. P9 via a non-inline helper TU-order change, or /d2inlT bisect with the P9 pragma present); then re-read c2prio for the esi/edi swap (peopleDef CDefPointer lifetime / declaration point) and the 8-byte frame excess (two dead locals?)
// Leaf: ?PopulateRegionWithVillagers@CWorldMap@@QAEXAAVCThing@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SEXTYPE
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
#define FABLE_RETAIL_CTHINGBUILDING_SIMDEF
#define FABLE_RETAIL_CTHINGCREATUREDEF_SEX
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_GAYVILLAGERS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_POPULATE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLDSEED
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CWORLDMAP_POPULATE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY_CALLS
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

namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b); }   // 0x009E9F40
long __fastcall GFROR13(unsigned long value);                                // 0x00497890
CCharString __fastcall GFIntToCharString(long value);                        // 0x0099F830
CCharString __fastcall operator+(const CCharString& a, const CCharString& b);    // 0x0099F570
extern CCharString g_CCharString_013bab2c;                                   // 0x013BAB2C the gay-candidate dialogue voice

inline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)   // ?GFRandom@@YIKKPAK@Z (inline here)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

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
enum ECreatureType { CREATURE_TYPE_CHILD = 1, CREATURE_TYPE_ADULT = 2 };

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
class CShopDef : public CDefClassBase {                              // Ego_r.pdb members at retail offsets
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CShopDef", -1); }
    unsigned char _pad_0x25[0x94 - 0x25];
    unsigned long PreferredShopkeeper;                               // +0x94 (Ego_r +0xA4 less the four STLport containers)
};
class CSimBuildingDef : public CDefClassBase {                       // Ego_r.pdb members at retail offsets (Ego_r - 4)
public:
    unsigned char _pad_0x25[0x3c - 0x25];
    long Property;                                                   // +0x3c ESimBuildingProperty flags
    long WorkSpaces;                                                 // +0x40
    long LiveSpaces;                                                 // +0x44
};
class CBuyableHouseDef : public CDefClassBase {
public:
    bool IsBuyable;                                                  // +0x25
    bool InitiallyEmpty;                                             // +0x26
};
class CTCBuyableHouse : public CTCBase {
public:
    enum { InterfaceType = 213 };
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)213; }
    CDefPointer<const CBuyableHouseDef> GetPDef() const { return PDef; }
    CDefPointer<const CBuyableHouseDef> PDef;                        // +0x0c
};
class CTCThingOwner : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)33; }
    std::list<CIntelligentPointer<CThing> > ObjectList;              // +0x0c
};
class CTCTalk : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)15; }
    unsigned char _pad_0x0c[0x38 - 0x0c];
    CCharString DialogueVoice;                                       // +0x38
    CCharString GetDialogueVoice() const { return DialogueVoice; }
};
class CTCDummyVillager : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)214; }
    void SetupFromData(unsigned long defType, CThingBuilding* home, CThingBuilding* work, bool guard);   // 0x0072F170
    void SetupFromVillager(CThingAICreature& villager, bool guard);  // 0x0072F2F0
};
class CTCDNone {
public:
    static CThing* __fastcall Create(long defIndex, const C3DVector& pos, bool flag);   // 0x00835A20
};
class CThingAICreatureInit {
public:
    CThingAICreatureInit(CThingPhysical* original, CThingPhysical* previous, bool playerCharacter,
                         const CCharString& scriptName, const CCharString& overridingBrainName, bool drawable)
        : OriginalThing(original), PreviousThing(previous), ScriptName(scriptName),
          OverridingBrainName(overridingBrainName), PlayerCharacter(playerCharacter), Drawable(drawable),
          PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false), HomeBuilding(0), WorkBuilding(0)
    {
    }
    CThingPhysical* OriginalThing;                   // +0x00
    CThingPhysical* PreviousThing;                   // +0x04
    CCharString     ScriptName;                      // +0x08
    CCharString     OverridingBrainName;             // +0x0c
    bool            PlayerCharacter;                 // +0x10
    bool            Drawable;                        // +0x11
    bool            PlaceAtEndOfList;                // +0x12
    bool            EnableCreatureAutoPlacing;       // +0x13
    CThingBuilding* HomeBuilding;                    // +0x14
    CThingBuilding* WorkBuilding;                    // +0x18
};

template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);  // ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z 0x0040DCE0

class CIsHouseLargerThan {
public:
    bool operator()(const CThingBuilding* a, const CThingBuilding* b) const;
};
namespace {
struct RandFunc {
    unsigned long* Seed;
    long operator()(long n);
};
}
namespace std {
template <class RandomAccessIter> void __fastcall random_shuffle(RandomAccessIter first, RandomAccessIter last);   // 0x005131D0
template <class RandomAccessIter, class RandomNumberGenerator>
void __fastcall random_shuffle(RandomAccessIter first, RandomAccessIter last, RandomNumberGenerator& rand);   // 0x00513210 / 0x00513270
template <class InputIter, class T> InputIter __fastcall find(InputIter first, InputIter last, const T& val);   // 0x00511BF0 (__find)
}

inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }
inline CThingBuilding* CThingAICreature::GetWorkBuilding() const { return WorkBuilding.Get(); }

typedef std::map<CThingBuilding*, std::list<CThingAICreature*> > CBuildingCreatureMap;
CThingBuilding* __fastcall GetRelevantBuilding(CBuildingCreatureMap& buildings, ESimBuildingProperty property, bool work, bool live);   // 0x005029A0
void __fastcall AddToWorkBuildingList(CThingAICreature* creature, CBuildingCreatureMap& buildings, CThingBuilding* building);   // 0x005003E0
void __fastcall AddToRelevantBuilding(CThingAICreature* creature, CBuildingCreatureMap& buildings, std::vector<CThingBuilding*>& houses,
                                      const CDefinitionManager* defs, CWorld& world);   // 0x00502B00

// The member template CDefClassBase::GetSubDef<T> (header text; the CWorldMap TU instantiates it: 0x00512FE0 CVillagePeopleDef,
// 0x005130A0 CShopDef).
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

// 0x004FF750: create a villager of the under-represented sex (the same TU; VC7.1 expands it at the work-place sites).
CThingAICreature* __fastcall CreateRelevantPersonAndUpdateStats(float maleWeight, float femaleWeight, long maleDefIndex,
    long femaleDefIndex, long& noMales, long& noFemales, const C3DVector& pos, const CThingAICreatureInit& init,
    CTCVillage* village, std::vector<CThingAICreature*>& males, std::vector<CThingAICreature*>& females, bool listCreatures)
{
    CThingAICreature* creature = 0;
    if (noMales * femaleWeight < noFemales * maleWeight)
    {
        if (maleDefIndex)
        {
            creature = CThingAICreature::Create(maleDefIndex, pos, 4, init);
            if (listCreatures) males.push_back(creature);
            ++noMales;
        }
        else if (femaleDefIndex)
        {
            creature = CThingAICreature::Create(femaleDefIndex, pos, 4, init);
            if (listCreatures) females.push_back(creature);
            ++noFemales;
        }
    }
    else
    {
        if (femaleDefIndex)
        {
            creature = CThingAICreature::Create(femaleDefIndex, pos, 4, init);
            if (listCreatures) females.push_back(creature);
            ++noFemales;
        }
        else if (maleDefIndex)
        {
            creature = CThingAICreature::Create(maleDefIndex, pos, 4, init);
            if (listCreatures) males.push_back(creature);
            ++noMales;
        }
    }
    if (creature)
    {
        creature->SetAsAllowedToFollowHero(true);
        creature->FollowHeroThroughTeleporters = true;
        village->AddThingToVillage(*creature);
        if (!listCreatures)
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

        long noGhosts = peopleDef->NumGhosts;
        long noGuards = peopleDef->NumGuards;
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
        for (std::list<CIntelligentPointer<CThingAICreature> >::iterator it = villageTC->VillagePeople.begin();
             it != villageTC->VillagePeople.end(); )
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
#pragma inline_depth(0)
                    workBuildings[work].push_back(worker);
#pragma inline_depth()
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
#pragma inline_depth(0)
                    homeBuildings[home].push_back(resident);
#pragma inline_depth()
                }
            }
            else
            {
                if (villager->GetHomeBuilding())
                    (*it)->SetHomeBuilding(0);
                if (villager->GetWorkBuilding())
                    villager->SetWorkBuilding(0);
                villager->IsUnderScriptSupervision();
            }
            ++it;
        }

        // The village's buildings: free homes and work places by kind.
        CThingBuilding* school = 0;
        for (std::list<CIntelligentPointer<CThingBuilding> >::iterator b = villageTC->VillageBuildings.begin();
             b != villageTC->VillageBuildings.end(); ++b)
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
                            for (std::list<CIntelligentPointer<CThing> >::iterator g = owner->ObjectList.begin(); g != owner->ObjectList.end(); ++g)
                            {
                                CThing* game = g->Get();
                                if (IS_THING_ALIVE(game) && game->HasTC((ETCInterfaceType)207))
                                {
                                    ++gamesMasterSpaces;
                                    --freeSpaces;
                                }
                            }
                        }
                        barmanSpaces = 1;
                        barmaidSpaces = freeSpaces - 1;
                    }
                    else if (property & SIM_BUILDING_GAMES)
                    {
                        CTCThingOwner* owner;
                        if (building->GetTC((ETCInterfaceType)33, &owner))
                        {
                            for (std::list<CIntelligentPointer<CThing> >::iterator g = owner->ObjectList.begin(); g != owner->ObjectList.end(); ++g)
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
            long maleDefIndex = peopleDef->MaleShopKeeper;
            long femaleDefIndex = peopleDef->FemaleShopKeeper;
            CDefPointer<const CShopDef> shopDef;
            if (init.WorkBuilding->PeekBaseDef().GetSubDef(&shopDef) && shopDef->PreferredShopkeeper)
            {
                CDefPointer<const CThingCreatureDef> keeperDef;
                villageTC->ParentThing->PeekDefinitionManager().GetDef(shopDef->PreferredShopkeeper, &keeperDef);
                if (keeperDef->Sex == SEX_MALE)
                {
                    maleDefIndex = shopDef->PreferredShopkeeper;
                    femaleDefIndex = 0;
                }
                else
                {
                    femaleDefIndex = shopDef->PreferredShopkeeper;
                    maleDefIndex = 0;
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
            long maleDefIndex;
            long femaleDefIndex;
            if (GFRandom(0xffffffff, World.GetWorldSeed()) & 1)
            {
                maleDefIndex = peopleDef->MaleEvilApprentice;
                femaleDefIndex = peopleDef->FemaleEvilApprentice;
            }
            else
            {
                maleDefIndex = peopleDef->MaleGoodApprentice;
                femaleDefIndex = peopleDef->FemaleGoodApprentice;
            }
            CThingAICreature* creature = CreateRelevantPersonAndUpdateStats(maleWeight, femaleWeight, maleDefIndex,
                femaleDefIndex, noMales, noFemales, village.GetPos(), init, villageTC, males, females, true);
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
                    for (h = houses.begin(); h != houses.end(); ++h)
                        if (*h == work)
                            break;
                    if (h != houses.end())
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
        std::sort(houses.begin(), houses.end(), CIsHouseLargerThan());
        for (std::vector<CThingBuilding*>::iterator h = houses.begin(); h != houses.end(); ++h)
        {
            NProgressDisplay::DisplayProgress("PopulateRegionWithVillagers 14", -1.0f, false, false);
            long freeSpaces = 0;
            if (!(*h)->PeekPSimDefinition().IsNull())
                freeSpaces = (*h)->PeekPSimDefinition()->LiveSpaces;
            freeSpaces -= (long)homeBuildings[*h].size();
            while (freeSpaces && (females.size() || males.size() || children.size()))
            {
                if (females.size())
                {
                    females.back()->SetHomeBuilding(*h);
                    homeBuildings[*h].push_back(females.back());
                    females.pop_back();
                    --freeSpaces;
                }
                if (!freeSpaces)
                    break;
                if (males.size())
                {
                    males.back()->SetHomeBuilding(*h);
                    homeBuildings[*h].push_back(males.back());
                    males.pop_back();
                    --freeSpaces;
                }
                if (!freeSpaces)
                    break;
                if (children.size())
                {
                    children.back()->SetHomeBuilding(*h);
                    homeBuildings[*h].push_back(children.back());
                    children.pop_back();
                    --freeSpaces;
                }
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
            for (std::list<CThingAICreature*>::iterator c = home->second.begin(); c != home->second.end(); ++c)
            {
                CThingAICreature* creature = *c;
                if (!creature || creature->GetCreatureType() != CREATURE_TYPE_ADULT)
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
                for (std::list<CThingAICreature*>::iterator c = home->second.begin(); c != home->second.end(); ++c)
                {
                    CThingAICreature* creature = *c;
                    if (creature && creature->GetCreatureType() == CREATURE_TYPE_CHILD &&
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
