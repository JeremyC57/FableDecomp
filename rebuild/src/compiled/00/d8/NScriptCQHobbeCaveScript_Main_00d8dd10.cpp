// NScript::CQ_HobbeCaveScript::Main() -- the Hobbe Cave quest main: set the first objective (Granny's House, or the cave tunnel once the pre-cave save is done), bind the fifteen entity scripts, post them, spawn the four quest threads, activate the barrier and Granny sub-quests, wait for Granny's House (or the save), show the quest start screen once, then wait in the cave entrance tunnel for the hero to carry the hexagon door key (auto-saving once in Greatwood Caves when he has it), and set the third objective
// Leaf: ?Main@CQ_HobbeCaveScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D8DD10, 3194 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00D8E989.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012D5540 that the "Q_HobbeCave" Alloc 0x00D99DC0 stores; Ego_r.pdb
//   name set ?Main@CQ_HobbeCaveScript@NScript@@UAEXXZ. Every bound Alloc is a landed CQ_HobbeCaveScript::C*::Alloc row (binding-name
//   literal = class name less its `C`); the threads are this class's RescuedBandit 0x00D91740, WatchForTermination 0x00D8E990,
//   WatchForHavingKey 0x00D8EE50 and WatchForGrannysRegion 0x00D8F040 (Ego_r.pdb QAEXXZ methods named by their thread-name literals).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CQ_HobbeCaveScript> vtable 0x012D567C), CScriptBase::AddSpawnedFunction
//   0x00CB7E50; CGameScriptInterfaceBase slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities, 70 GetHero,
//   184 IsObjectInThingsPossession, 276 ActivateQuest, 296 SetQuestCardObjective, 309 KickOffQuestStartScreen, 655 GetActiveQuestName,
//   703 AutoSaveCheckPoint.
// Shape: the landed quest-Main binding model (CQ_PrisonEscapeRescueMotherScript::Main 0x00DD2CE0) with WAIT_UNTIL loops (loop, then a
//   trailing terminate check); the key wait is a for(;;) with a break.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HobbeCaveScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"

namespace NScript {
#define FABLE_HC_ENTITY(Name) \
    class CQ_HobbeCaveScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HobbeCaveScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_HC_ENTITY(CHobbeSpawn)                 // Alloc 0x00D8F310
FABLE_HC_ENTITY(CHobbeTorrent)               // Alloc 0x00D8F3F0
FABLE_HC_ENTITY(CHobbeRoaming)               // Alloc 0x00D8F4F0
FABLE_HC_ENTITY(CHobbeLeader)                // Alloc 0x00D8F950
FABLE_HC_ENTITY(CHobbeLarder)                // Alloc 0x00D8F5E0
FABLE_HC_ENTITY(CGranny)                     // Alloc 0x00D8FC70
FABLE_HC_ENTITY(CNymph)                      // Alloc 0x00D8FE30
FABLE_HC_ENTITY(CJames)                      // Alloc 0x00D8FFA0
FABLE_HC_ENTITY(CJamesCommentMarker)         // Alloc 0x00D906D0
FABLE_HC_ENTITY(CJamesCommentMarkerAdv)      // Alloc 0x00D90A50
FABLE_HC_ENTITY(CBanditCommentMarker)        // Alloc 0x00D901B0
FABLE_HC_ENTITY(CBanditSacrifice)            // Alloc 0x00D90E90
FABLE_HC_ENTITY(CBanditInRoomSample)         // Alloc 0x00D90FF0
FABLE_HC_ENTITY(CCheckTheSilverKeyChest)     // Alloc 0x00D91250
FABLE_HC_ENTITY(CInvisibleCage)              // Alloc 0x00D91470
#undef FABLE_HC_ENTITY
}

void NScript::CQ_HobbeCaveScript::Main()
{
    if (!SavedBeforeCaves) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HOBBE_CAVE_OBJECTIVE_01", "GrannysHouse", "Greatwood");
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HOBBE_CAVE_OBJECTIVE_02", "HobbeCaveEntranceTunnel", "Greatwood");
    }
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("HobbeSpawn", this, &CHobbeSpawn::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("HobbeTorrent", this, &CHobbeTorrent::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("HobbeRoaming", this, &CHobbeRoaming::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("HobbeLeader", this, &CHobbeLeader::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("HobbeLarder", this, &CHobbeLarder::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("Granny", this, &CGranny::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("Nymph", this, &CNymph::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("James", this, &CJames::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("JamesCommentMarker", this, &CJamesCommentMarker::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("JamesCommentMarkerAdv", this, &CJamesCommentMarkerAdv::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("BanditCommentMarker", this, &CBanditCommentMarker::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("BanditSacrifice", this, &CBanditSacrifice::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("BanditInRoomSample", this, &CBanditInRoomSample::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("CheckTheSilverKeyChest", this, &CCheckTheSilverKeyChest::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeCaveScript>("InvisibleCage", this, &CInvisibleCage::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_HobbeCaveScript>("RescuedBandit", 0, this, &CQ_HobbeCaveScript::RescuedBandit), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_HobbeCaveScript>("WatchForTermination", 0, this, &CQ_HobbeCaveScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_HobbeCaveScript>("WatchForHavingKey", 0, this, &CQ_HobbeCaveScript::WatchForHavingKey), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_HobbeCaveScript>("WatchForGrannysRegion", 0, this, &CQ_HobbeCaveScript::WatchForGrannysRegion), CCharString(""));
    Game->ActivateQuest("Q_HobbeCaveBarrier");
    Game->ActivateQuest("Q_HobbeCaveGranny");
    while (!SavedBeforeCaves && !Game->IsRegionLoaded("GrannysHouse")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!QuestStartScreened) {
        if (IsActiveThreadTerminating())
            return;
        Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
        QuestStartScreened = true;
    }
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (Game->IsRegionLoaded("HobbeCaveEntranceTunnel")) {
            if (IsActiveThreadTerminating())
                return;
            if (Game->IsObjectInThingsPossession("OBJECT_HEXAGON_DOOR_KEY", Game->GetHero()))
                break;
        }
        if (!SavedBeforeCaves && Game->IsRegionLoaded("GreatwoodCaves")) {
            if (IsActiveThreadTerminating())
                return;
            if (Game->IsObjectInThingsPossession("OBJECT_HEXAGON_DOOR_KEY", Game->GetHero())) {
                if (IsActiveThreadTerminating())
                    return;
                SavedBeforeCaves = true;
                Game->AutoSaveCheckPoint();
            }
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HOBBE_CAVE_OBJECTIVE_03", "HobbeCaveNymphFocal", "Greatwood");
    ReachedHobbeCave = true;
    if (IsActiveThreadTerminating())
        return;
}