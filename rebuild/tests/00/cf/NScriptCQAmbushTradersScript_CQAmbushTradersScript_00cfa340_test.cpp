// Behaviour test for NScript::CQ_AmbushTradersScript::~CQ_AmbushTradersScript (0x00CFA340, compiler-generated): members
// destroyed in reverse declaration order (TeleportInfo records, CSFlagMap, the ten speech string arrays, the six
// script-thing vectors, the two timers deregistered), then ~CScriptBase.
#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
static char g_log[512];
static void Log(char c) { std::size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct StringModel { char Tag; StringModel() : Tag('?') { } StringModel(char t) : Tag(t) { } ~StringModel() { if (Tag != '?') Log(Tag); } };
struct TeleportDataModel { StringModel defName, scriptName, teleportTo, teleportMkr; };
struct ScriptThingModel { char Tag; ScriptThingModel() : Tag(0) { } virtual ~ScriptThingModel() { if (Tag) Log(Tag); } };
struct TimerModel { char Tag; ~TimerModel() { Log(Tag); } };
struct ScriptBaseModel { virtual ~ScriptBaseModel() { Log('S'); } };
struct AmbushTradersModel : ScriptBaseModel {
    TimerModel SpeechBanditTimer, SpeechSpyTimer;
    std::vector<ScriptThingModel> AllBandits, AllTraders, AllGuards, BanditsVector, ConvoyMembersVector, SentriesVector;
    StringModel BanditSpeechNormalThere[5], BanditSpeechRandomThere[5], BanditSpeechNormalBack[5], BanditSpeechRandomBack[5];
    StringModel SpySpeechNormal[5], SpySpeechRandom[5], SpySpeechProximityNormal[5], SpySpeechProximityRandom[5];
    StringModel SpySpeechSentryNormal[5], SpySpeechSentryRandom[5];
    std::map<int, bool> CSFlagMap;
    std::vector<TeleportDataModel> TeleportInfo;
};
int main()
{
    AmbushTradersModel* s = new AmbushTradersModel;
    s->SpeechBanditTimer.Tag = 'b'; s->SpeechSpyTimer.Tag = 's';
    s->AllBandits.resize(1); s->AllBandits[0].Tag = 'A';
    s->SentriesVector.resize(2); s->SentriesVector[0].Tag = 'V'; s->SentriesVector[1].Tag = 'W';
    s->BanditSpeechNormalThere[0].Tag = 'n'; s->BanditSpeechNormalThere[4].Tag = 'm';
    s->SpySpeechSentryRandom[1].Tag = 'r';
    s->CSFlagMap[1] = true;
    s->TeleportInfo.resize(1); s->TeleportInfo[0].defName.Tag = 'd'; s->TeleportInfo[0].teleportMkr.Tag = 'k';
    g_log[0] = 0;
    ScriptBaseModel* p = s;
    delete p;
    // TeleportInfo (record members in reverse: k then d), then the string arrays last-declared-first and highest-index-first
    // (r, then m n), then the vectors in reverse with their elements in order (V W ... A), then the timers (s b), then ~CScriptBase.
    if (std::strcmp(g_log, "kdrmnVWAsbS") != 0) { std::printf("00cfa340_TEST FAIL %s\n", g_log); return 1; }
    std::printf("00cfa340_TEST PASS\n");
    return 0;
}