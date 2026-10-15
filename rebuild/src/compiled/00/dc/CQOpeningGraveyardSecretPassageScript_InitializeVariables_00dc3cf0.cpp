// CQ_OpeningGraveyardSecretPassageScript::InitializeVariables() -- name the four ceremonial hero objects and clear the quest flags, the friendly-kill count and one flag in the shared script data
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC3CF0, length 113 bytes.
// Callees: CCharString::operator=(const char*) @ 0x0099EFE0 (x4: "OBJECT_CEREMONIAL_SWORD" @ 0x012DA464, "..._SHIELD"
// @ 0x012DA448, "..._ARMOUR" @ 0x012DA42C, "..._HELMET" @ 0x012DA410).
// Layout (retail = Ego_r PDB offsets -0x14): the script data pointer +0x44, finished_quest +0x48, DoorChange +0x49,
// ZombieCamPlayed +0x4a, skeleton_visited +0x4b, doors_open +0x4c, GravekeeperIntroduced +0x4d, GravekeeperCutsceneStart
// +0x4e, UndeadCutscenePlayed +0x4f, UndeadCutsceneStarted +0x50, SkeletonStuff[4] +0x51 (untouched), MissionSucceeded
// +0x55, MissionFailed +0x56, DoorClosed +0x57, hero_object[4] +0x58, LieutenantAround +0x68, FriendliesKilled +0x6c,
// VisitorsAttacked +0x70; the data's flag byte at +0x11c is not named by the PDB.
// Shape: the four string assignments first, then one zero register for every flag in source order (+0x4a first).
class CCharString {
public:
    void* PStringData;
    CCharString& operator=(const char* text);      // 0x0099EFE0
};
struct CQ_OpeningGraveyardSecretPassageData {      // the shared quest data the script holds at +0x44
    unsigned char _pad[0x11c];
    bool          m_flag11c;                       // +0x11c
};
struct CQ_OpeningGraveyardSecretPassageScript {
    unsigned char _pad_0x00[0x44];
    CQ_OpeningGraveyardSecretPassageData* Data;    // +0x44
    bool          finished_quest;                  // +0x48
    bool          DoorChange;                      // +0x49
    bool          ZombieCamPlayed;                 // +0x4a
    bool          skeleton_visited;                // +0x4b
    bool          doors_open;                      // +0x4c
    bool          GravekeeperIntroduced;           // +0x4d
    bool          GravekeeperCutsceneStart;        // +0x4e
    bool          UndeadCutscenePlayed;            // +0x4f
    bool          UndeadCutsceneStarted;           // +0x50
    bool          SkeletonStuff[4];                // +0x51
    bool          MissionSucceeded;                // +0x55
    bool          MissionFailed;                   // +0x56
    bool          DoorClosed;                      // +0x57
    CCharString   hero_object[4];                  // +0x58
    bool          LieutenantAround;                // +0x68
    long          FriendliesKilled;                // +0x6c
    bool          VisitorsAttacked;                // +0x70
    void InitializeVariables();
};
void CQ_OpeningGraveyardSecretPassageScript::InitializeVariables()
{
    hero_object[0] = "OBJECT_CEREMONIAL_SWORD";
    hero_object[1] = "OBJECT_CEREMONIAL_SHIELD";
    hero_object[2] = "OBJECT_CEREMONIAL_ARMOUR";
    hero_object[3] = "OBJECT_CEREMONIAL_HELMET";
    ZombieCamPlayed = false;
    finished_quest = false;
    skeleton_visited = false;
    doors_open = false;
    DoorChange = false;
    GravekeeperIntroduced = false;
    GravekeeperCutsceneStart = false;
    UndeadCutscenePlayed = false;
    UndeadCutsceneStarted = false;
    MissionSucceeded = false;
    MissionFailed = false;
    DoorClosed = false;
    LieutenantAround = false;
    FriendliesKilled = 0;
    VisitorsAttacked = false;
    Data->m_flag11c = false;
}