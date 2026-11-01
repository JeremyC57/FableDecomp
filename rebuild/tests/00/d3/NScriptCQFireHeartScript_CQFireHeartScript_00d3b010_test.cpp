// Behaviour test for NScript::CQ_FireHeartScript::~CQ_FireHeartScript (0x00D3B010, compiler-generated): members destroyed
// in reverse declaration order (VillagerEscapeMarker, ProphetComment, the three scripted-thing handles, seh_Prophet, the
// Pattern / FollowerHandles / script-thing vectors, the six bool vectors, PadMoonFX, PadSunFX, Effect, the two maps),
// then ~CScriptBase.
#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
static char g_log[512];
static void Log(char c) { std::size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct StringModel { char Tag; StringModel() : Tag(0) { } ~StringModel() { if (Tag) Log(Tag); } };
struct ScriptThingModel { char Tag; ScriptThingModel() : Tag(0) { } virtual ~ScriptThingModel() { if (Tag) Log(Tag); } };
struct HandleModel { char Tag; HandleModel() : Tag(0) { } virtual ~HandleModel() { if (Tag) Log(Tag); } };
struct PatternDefModel { char Tag; PatternDefModel() : Tag(0) { } ~PatternDefModel() { if (Tag) Log(Tag); } };
struct ScriptBaseModel { virtual ~ScriptBaseModel() { Log('S'); } };
struct FireHeartModel : ScriptBaseModel {
    std::map<int, bool> CutsceneFlags;
    std::map<int, int> csargs;
    ScriptThingModel Effect;
    StringModel PadSunFX, PadMoonFX;
    std::vector<bool> StartingPattern, ProphetsKilled, ProphetsReleased, ShouldProphetSpeak, ShouldVillagerSpeak, CommentGivenThisRound;
    std::vector<ScriptThingModel> Prophets, Villagers, Followers;
    std::vector<HandleModel> FollowerHandles;
    std::vector<PatternDefModel> Pattern;
    std::vector<HandleModel> seh_Prophet;
    HandleModel seh_BriarRose, seh_Scythe, seh_GuildMaster;
    StringModel ProphetComment;
    ScriptThingModel VillagerEscapeMarker;
};
int main()
{
    FireHeartModel* s = new FireHeartModel;
    s->CutsceneFlags[1] = true; s->csargs[2] = 3;
    s->Effect.Tag = 'E'; s->PadSunFX.Tag = 'u'; s->PadMoonFX.Tag = 'o';
    s->StartingPattern.push_back(true);
    s->Prophets.resize(1); s->Prophets[0].Tag = 'P';
    s->Villagers.resize(2); s->Villagers[0].Tag = 'V'; s->Villagers[1].Tag = 'W';
    s->FollowerHandles.resize(1); s->FollowerHandles[0].Tag = 'H';
    s->Pattern.resize(1); s->Pattern[0].Tag = 'T';
    s->seh_Prophet.resize(1); s->seh_Prophet[0].Tag = 'p';
    s->seh_BriarRose.Tag = 'B'; s->seh_Scythe.Tag = 'C'; s->seh_GuildMaster.Tag = 'G';
    s->ProphetComment.Tag = 'c'; s->VillagerEscapeMarker.Tag = 'M';
    g_log[0] = 0;
    ScriptBaseModel* p = s;
    delete p;
    if (std::strcmp(g_log, "McGCBpTHVWPouES") != 0) { std::printf("00d3b010_TEST FAIL %s\n", g_log); return 1; }
    std::printf("00d3b010_TEST PASS\n");
    return 0;
}