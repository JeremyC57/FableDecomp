// NScript::CQ_RansomVictimChiefsHouseScript::Main() -- original script entry logic.
// Leaf: ?Main@CQ_RansomVictimChiefsHouseScript@NScript@@UAEXXZ
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CQ_RansomVictimChiefsHouseScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBinding.h"

inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const
{
    if (!PImp.Data) return false;
    return PImp.Data->IsAlive();
}

void NScript::CQ_RansomVictimChiefsHouseScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_RansomVictimChiefsHouseScript>("Test", this, &CTest::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    if (!SetChiefsHut) {
        if (IsActiveThreadTerminating()) return;
        while (!Game->IsRegionLoaded("KnotholeGlade")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptThing ChiefsHouse = Game->GetThingWithScriptName("ChiefsHut");
        CScriptThing SecondaryHouse = Game->SetResidency(ChiefsHouse, false);
        if (SecondaryHouse.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            Game->SetResidency(SecondaryHouse, false);
        }
        Game->OverrideAutomaticHouseLocking(ChiefsHouse, true);
        Game->OpenHouseDoors(ChiefsHouse);
        SetChiefsHut = true;
    }
    while (!Game->IsQuestCompleted("Q_RansomVictim")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Game->IsRegionLoaded("KnotholeGlade")) {
            if (IsActiveThreadTerminating()) return;
            CScriptThing ChiefsHouse = Game->GetThingWithScriptName("ChiefsHut");
            Game->OverrideAutomaticHouseLocking(ChiefsHouse, true);
            Game->OpenHouseDoors(ChiefsHouse);
            while (Game->IsRegionLoaded("KnotholeGlade") && !Game->IsQuestCompleted("Q_RansomVictim")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
    }
    if (IsActiveThreadTerminating()) return;
    while (!Game->IsRegionLoaded("KnotholeGlade")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptThing ChiefsHouse = Game->GetThingWithScriptName("ChiefsHut");
    Game->SetResidency(ChiefsHouse, true);
    CScriptThing SecondaryHouse = Game->SetResidency(ChiefsHouse, false);
    if (SecondaryHouse.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Game->SetResidency(SecondaryHouse, false);
    }
}