// NScript::CV_TourGuideScript::Init() -- reset tour progress and fill the original waypoint and random-response string tables.
// Leaf: ?Init@CV_TourGuideScript@NScript@@UAEXXZ
#define FABLE_RETAIL_CSCRIPTBASE_PRIVATE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PERSONAL_SCRIPT
#define FABLE_RETAIL_TOUR_GUIDE
#include "engine/CV_TourGuideScript.h"
void NScript::CV_TourGuideScript::Init()
{
    TourGuideKilled = false;
    AllFollowersDead = false;
    QuestFinished = false;
    QuestActivated = false;
    TourFinished = false;
    Initialise = true;
    SpawnedQuestFinishThread = false;
    GuideSpokenToHeroThisWaypoint = false;
    OverheardTourGuideThisWaypoint = false;
    WaypointCounter = 0;
    WaypointInfo[TG_LOCATION_BALCONY].locMarker = "M_TG_LocationBalcony";
    WaypointInfo[TG_LOCATION_BALCONY].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_BALCONY";
    WaypointInfo[TG_LOCATION_BALCONY].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_BALCONY";
    WaypointInfo[TG_LOCATION_BANQUET].locMarker = "M_TG_LocationBanquet";
    WaypointInfo[TG_LOCATION_BANQUET].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_BANQUET";
    WaypointInfo[TG_LOCATION_BANQUET].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_BANQUET";
    WaypointInfo[TG_LOCATION_BAR].locMarker = "M_TG_LocationBar";
    WaypointInfo[TG_LOCATION_BAR].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_BAR";
    WaypointInfo[TG_LOCATION_BAR].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_BAR";
    WaypointInfo[TG_LOCATION_BRIDGE].locMarker = "M_TG_LocationBridge";
    WaypointInfo[TG_LOCATION_BRIDGE].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_BRIDGE";
    WaypointInfo[TG_LOCATION_BRIDGE].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_BRIDGE";
    WaypointInfo[TG_LOCATION_CLOISTERS].locMarker = "M_TG_LocationCloisters";
    WaypointInfo[TG_LOCATION_CLOISTERS].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_CLOISTERS";
    WaypointInfo[TG_LOCATION_CLOISTERS].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_CLOISTERS";
    WaypointInfo[TG_LOCATION_DORM1].locMarker = "M_TG_LocationDorm1";
    WaypointInfo[TG_LOCATION_DORM1].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_DORM1";
    WaypointInfo[TG_LOCATION_DORM1].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_DORM1";
    WaypointInfo[TG_LOCATION_DORM2].locMarker = "M_TG_LocationDorm2";
    WaypointInfo[TG_LOCATION_DORM2].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_DORM2";
    WaypointInfo[TG_LOCATION_DORM2].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_DORM2";
    WaypointInfo[TG_LOCATION_LIBRARY].locMarker = "M_TG_LocationLibrary";
    WaypointInfo[TG_LOCATION_LIBRARY].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_LIBRARY";
    WaypointInfo[TG_LOCATION_LIBRARY].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_LIBRARY";
    WaypointInfo[TG_LOCATION_MAP].locMarker = "M_TG_LocationMap";
    WaypointInfo[TG_LOCATION_MAP].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_MAP";
    WaypointInfo[TG_LOCATION_MAP].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_MAP";
    WaypointInfo[TG_LOCATION_MAZE].locMarker = "M_TG_LocationMaze";
    WaypointInfo[TG_LOCATION_MAZE].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_MAZE";
    WaypointInfo[TG_LOCATION_MAZE].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_MAZE";
    WaypointInfo[TG_LOCATION_SHOP].locMarker = "M_TG_LocationShop";
    WaypointInfo[TG_LOCATION_SHOP].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_SHOP";
    WaypointInfo[TG_LOCATION_SHOP].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_SHOP";
    WaypointInfo[TG_LOCATION_START].locMarker = "M_TG_LocationStart";
    WaypointInfo[TG_LOCATION_START].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_START";
    WaypointInfo[TG_LOCATION_START].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_START";
    WaypointInfo[TG_LOCATION_TOMB].locMarker = "M_TG_LocationTomb";
    WaypointInfo[TG_LOCATION_TOMB].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_TOMB";
    WaypointInfo[TG_LOCATION_TOMB].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_TOMB";
    WaypointInfo[TG_LOCATION_TRAINING].locMarker = "M_TG_LocationTraining";
    WaypointInfo[TG_LOCATION_TRAINING].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_TRAINING";
    WaypointInfo[TG_LOCATION_TRAINING].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_TRAINING";
    WaypointInfo[TG_LOCATION_SERVANT].locMarker = "M_TG_LocationServant";
    WaypointInfo[TG_LOCATION_SERVANT].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_SERVANT";
    WaypointInfo[TG_LOCATION_SERVANT].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_SERVANT";
    WaypointInfo[TG_LOCATION_STAIRS_INSIDE].locMarker = "M_TG_LocationStairsInside";
    WaypointInfo[TG_LOCATION_STAIRS_INSIDE].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_STAIR";
    WaypointInfo[TG_LOCATION_STAIRS_INSIDE].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_STAIR";
    WaypointInfo[TG_LOCATION_STAIRS_OUTSIDE].locMarker = "M_TG_LocationStairsOutside";
    WaypointInfo[TG_LOCATION_STAIRS_OUTSIDE].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_STAIRCASE";
    WaypointInfo[TG_LOCATION_STAIRS_OUTSIDE].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_STAIRCASE";
    WaypointInfo[TG_LOCATION_STATUE].locMarker = "M_TG_LocationStatue";
    WaypointInfo[TG_LOCATION_STATUE].locTextOverheard = "TEXT_QST_066_LOCATION_OVERHEARD_STATUE";
    WaypointInfo[TG_LOCATION_STATUE].locTextRequested = "TEXT_QST_066_LOCATION_REQUESTED_STATUE";
    RandomGuideResponse[0] = "TEXT_QST_066_RANDOM_GUIDE_RESPONSE_0";
    RandomGuideResponse[1] = "TEXT_QST_066_RANDOM_GUIDE_RESPONSE_1";
    RandomGuideResponse[2] = "TEXT_QST_066_RANDOM_GUIDE_RESPONSE_2";
    RandomGuideResponse[3] = "TEXT_QST_066_RANDOM_GUIDE_RESPONSE_3";
    RandomGuideResponse[4] = "TEXT_QST_066_RANDOM_GUIDE_RESPONSE_4";
    RandomFollowerResponseM[0] = "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_10";
    RandomFollowerResponseM[1] = "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_20";
    RandomFollowerResponseM[2] = "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_30";
    RandomFollowerResponseM[3] = "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_40";
    RandomFollowerResponseM[4] = "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_50";
    RandomFollowerResponseF[0] = "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_10";
    RandomFollowerResponseF[1] = "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_20";
    RandomFollowerResponseF[2] = "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_30";
    RandomFollowerResponseF[3] = "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_40";
    RandomFollowerResponseF[4] = "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_50";
}
