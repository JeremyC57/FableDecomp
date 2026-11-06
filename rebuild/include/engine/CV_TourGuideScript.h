#ifndef FABLE_ENGINE_CV_TOURGUIDESCRIPT_H
#define FABLE_ENGINE_CV_TOURGUIDESCRIPT_H
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#ifdef FABLE_RETAIL_CV_TOURGUIDESCRIPT_GUIDE
#include "engine/CEntityScriptBase.h"
class CScriptGameResourceObjectScriptedThingBase;
#endif
// Retail opt-in (Codex pass12): original PDB full fields; retail base is14 bytes smaller than Ego_r.
// Native ctor00ee41f0 and Alloc00ee6c00 prove size178, vtable012f46b8, Init slot3=00ee42a0.
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TourGuideScript:public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_TourGuideScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    class CTourGuideGuide;
    class CTourGuideFollower;
    enum TourGuideLocations {
        TG_LOCATION_START=0,
        TG_LOCATION_TRAINING=1,
        TG_LOCATION_SERVANT=2,
        TG_LOCATION_STATUE=3,
        TG_LOCATION_BANQUET=4,
        TG_LOCATION_MAP=5,
        TG_LOCATION_LIBRARY=6,
        TG_LOCATION_BAR=7,
        TG_LOCATION_STAIRS_OUTSIDE=8,
        TG_LOCATION_BALCONY=9,
        TG_LOCATION_DORM1=10,
        TG_LOCATION_DORM2=11,
        TG_LOCATION_STAIRS_INSIDE=12,
        TG_LOCATION_SHOP=13,
        TG_LOCATION_CLOISTERS=14,
        TG_LOCATION_TOMB=15,
        TG_LOCATION_MAZE=16,
        TG_LOCATION_BRIDGE=17,
        TG_LOCATIONS_MAX=18
    };
    struct WaypointData {
        CCharString locMarker;
        CCharString locTextOverheard;
        CCharString locTextRequested;
    };
    WaypointData WaypointInfo[TG_LOCATIONS_MAX]; // +0x48
    bool TourGuideKilled; // +0x120
    bool AllFollowersDead; // +0x121
    bool QuestFinished; // +0x122
    bool TourFinished; // +0x123
    bool QuestActivated; // +0x124
    bool Initialise; // +0x125
    bool SpawnedQuestFinishThread; // +0x126
    CCharString RandomGuideResponse[5]; // +0x128
    CCharString RandomFollowerResponseM[5]; // +0x13c
    CCharString RandomFollowerResponseF[5]; // +0x150
    long WaypointCounter; // +0x164
    CScriptThing NextTourWaypoint; // +0x168
    bool GuideSpokenToHeroThisWaypoint; // +0x174
    bool OverheardTourGuideThisWaypoint; // +0x175
#ifdef FABLE_RETAIL_CV_TOURGUIDESCRIPT_GUIDE
    // Retail opt-in (chunk 807): the script's own threads and helper (Ego_r.pdb / FableWin.pdb name set
    // ?<Name>@CV_TourGuideScript@NScript@@QAE...; CTourGuideGuide::Main 0x00EE57B0 calls MoveToNextWaypoint and spawns
    // WatchForNoFollowers under "ParentClass.WatchForNoFollowers").
    void MoveToNextWaypoint(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&);   // 0x00EE6850
    void WatchForNoFollowers();                                          // 0x00EE6A40
    void WatchForClosingTime();                                          // 0x00EE4A00
    void WatchForGuideKilled();                                          // 0x00EE4A70
#endif
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_TourGuideScript)==0x178);
#ifdef FABLE_RETAIL_CV_TOURGUIDESCRIPT_GUIDE
// Retail opt-in (chunk 807): the guide entity script. Ego_r.pdb CTourGuideGuide (struct_layouts_egor.tsv, 36 bytes; entity offsets
// equal retail's, CEntityScriptBase is 0x14 in both): ParentClass +0x14, PMasterData +0x18, saveWaypointIdx +0x1c,
// lastRandomSpeechIdx +0x20 (the landed Init 0x00EE4C60 sets both to -1). Entity vtable 0x012F511C (landed Alloc 0x00EE4BB0):
// slot 0 scalar deleting dtor 0x00EE4CD0, 1 Main 0x00EE57B0, 2 Init 0x00EE4C60, 3 GetParentScript 0x00EE4C50; OnPredicateFail
// 0x00EE4CC0. Main / Init / OnPredicateFail are private (PDB EAEXXZ).
namespace NScript {
class CV_TourGuideScript::CTourGuideGuide : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_TourGuideScript&, CScriptDataBase*, const CScriptThing&); // 0x00EE4BB0
    virtual CScriptBase* GetParentScript() const;                        // 0x00EE4C50
private:
    virtual void Main();                                                 // 0x00EE57B0
    virtual void Init();                                                 // 0x00EE4C60
    virtual void OnPredicateFail();                                      // 0x00EE4CC0
protected:
    CV_TourGuideScript* ParentClass;                                     // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    long saveWaypointIdx;                                                // +0x1c
    long lastRandomSpeechIdx;                                            // +0x20
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_TourGuideScript::CTourGuideGuide)==0x24);
#endif
FABLE_STATIC_ASSERT(sizeof(NScript::CV_TourGuideScript::WaypointData)==0xc);
#endif
