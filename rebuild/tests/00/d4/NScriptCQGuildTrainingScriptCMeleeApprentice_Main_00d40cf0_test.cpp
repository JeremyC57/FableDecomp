// Behaviour test for 0x00d40cf0 NScript::CQ_GuildTrainingScript::CMeleeApprentice::Main: scripted control, sheathe / hero faction /
// unkillable / no boss phases / friends-with-everything, the marker lookup, then per frame: give way while the master data or the
// started melee test wants her, the woods-Will run to MK_GTM_WD_HEROWALK (once) or the scorpion moan when talked to, otherwise the
// quest moan when talked to followed by a run back to the marker, and a run back whenever she is more than 4 from it and idle.
#include <cstdio>
#include <cstring>

class C3DVector { public: float X, Y, Z; };
static C3DVector g_null = { 0, 0, 0 };
class CScriptThing { public: int Id; C3DVector Pos; const C3DVector& GetPos() const { return Id ? Pos : g_null; }
    bool MsgIsTalkedToBy(const char* who) const; void SetFriendsWithEverythingFlag(bool b) const; };
static int g_frame, g_endFrame, g_talkFrame, g_farFrame, g_moves, g_speaks, g_movies, g_pauses, g_starts, g_releases, g_clears;
static int g_sheathe, g_killable, g_boss, g_friends, g_faction;
static bool g_valid, g_performing, g_woods, g_skill, g_will, g_departure, g_woodsDeparture, g_needed, g_testing;
static char g_lastSpeak[64]; static char g_lastLookup[64]; static float g_lastMoveX, g_lastSpeed; static int g_lastMoveType;
bool CScriptThing::MsgIsTalkedToBy(const char* who) const { return Id == 1 && !std::strcmp(who, "SCRIPT_NAME_HERO") && g_frame == g_talkFrame; }
void CScriptThing::SetFriendsWithEverythingFlag(bool b) const { if (b) ++g_friends; }
struct CSeh {
    bool IsValid() const { return g_valid; }
    void Release() { ++g_releases; g_valid = false; }
    void ClearCommands() { ++g_clears; }
    CScriptThing GetScriptThing() const { CScriptThing t; t.Id = 1; t.Pos = g_null; return t; }
    void Speak(const CScriptThing&, const char* key, int, bool, bool, bool) { ++g_speaks; std::strcpy(g_lastSpeak, key); }
    bool IsPerformingScriptTask() const { return g_performing; }
    void MoveToPosition(const C3DVector& p, float speed, int type, bool, bool) { ++g_moves; g_lastMoveX = p.X; g_lastSpeed = speed; g_lastMoveType = type; }
};
class CGame { public:
    CScriptThing Hero;
    void NewScriptFrame() const { ++g_frame; }
    bool StartScriptingEntity(const CScriptThing&, CSeh&, int prio) const { ++g_starts; g_valid = true; return prio == 4; }
    void EntitySheatheWeapons(const CScriptThing&, bool b) const { if (!b) ++g_sheathe; }
    void EntitySetInFaction(const CScriptThing&, const char* f) const { if (!std::strcmp(f, "FACTION_HERO")) ++g_faction; }
    void EntitySetAsKillable(const CScriptThing&, bool a, bool b) const { if (!a && b) ++g_killable; }
    void EntitySetAllowBossPhaseChanges(const CScriptThing&, bool b) const { if (!b) ++g_boss; }
    CScriptThing GetThingWithScriptName(const char* n) const { std::strcpy(g_lastLookup, n); CScriptThing t; t.Id = 2;
        t.Pos.X = !std::strcmp(n, "MeleeApprenticeMarker") ? 7.0f : 9.0f; t.Pos.Y = t.Pos.Z = 0; return t; }
    bool IsQuestActive(const char* q) const {
        if (!std::strcmp(q, "Q_GuildTrainingWoodsWill")) return g_woods;
        if (!std::strcmp(q, "Q_GuildTrainingSkill")) return g_skill;
        if (!std::strcmp(q, "Q_GuildTrainingWill")) return g_will;
        if (!std::strcmp(q, "Q_GuildTrainingDeparture")) return g_departure;
        if (!std::strcmp(q, "Q_GuildTrainingWoodsDeparture")) return g_woodsDeparture;
        return false; }
    float GetHealth(const CScriptThing&) const { return 1.0f; }
    CScriptThing& GetHero() { return Hero; }
    void StartMovieSequence(const char*, int&) const { ++g_movies; }
    void PauseAllNonScriptedEntities(bool b) const { g_pauses += b ? 1 : -1; }
};
static bool IsDistanceBetweenThingsOver(const CScriptThing&, const CScriptThing& b, float d) { return b.Id == 2 && d == 4.0f && g_frame == g_farFrame; }
struct CPause { CGame* G; CPause(CGame& g) : G(&g) { G->PauseAllNonScriptedEntities(true); } ~CPause() { G->PauseAllNonScriptedEntities(false); } };
struct CMaster { bool MeleeApprenticeNeededForCutscene; };
struct CQuest { bool StartedMeleeTesting; };
#define MA_T if (IsActiveThreadTerminating()) return
#define MA_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me, seh_me, 4)) { Game->NewScriptFrame(); MA_T; } \
    MA_T
#define MA_MOAN(key) { \
        int movie = 0; Game->StartMovieSequence("", movie); CPause PauseMyEntitiesPlease(*Game); \
        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) { \
            seh_me.Speak(Game->GetHero(), key, 0, false, true, false); \
            while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); MA_T; } \
            MA_T; \
        } \
    }
struct CMeleeApprentice {
    CGame* Game; CScriptThing Me; CQuest& ParentClass; CMaster* PMasterData; bool WillWoodsChatDone;
    CMeleeApprentice(CGame* g, CQuest& q, CMaster* m) : Game(g), ParentClass(q), PMasterData(m), WillWoodsChatDone(false) { Me.Id = 1; Me.Pos = g_null; }
    bool IsActiveThreadTerminating() const { return g_frame >= g_endFrame; }
    void Main()
    {
        CSeh seh_me;
        MA_START;
        Game->EntitySheatheWeapons(Me, false);
        Game->EntitySetInFaction(Me, "FACTION_HERO");
        Game->EntitySetAsKillable(Me, false, true);
        Game->EntitySetAllowBossPhaseChanges(Me, false);
        Me.SetFriendsWithEverythingFlag(true);
        CScriptThing marker = Game->GetThingWithScriptName("MeleeApprenticeMarker");
        for (;; Game->NewScriptFrame()) {
            MA_T;
            if (PMasterData->MeleeApprenticeNeededForCutscene) {
                MA_T;
                if (seh_me.IsValid()) seh_me.Release();
                while (PMasterData->MeleeApprenticeNeededForCutscene) { Game->NewScriptFrame(); MA_T; }
                MA_T;
                MA_START;
            }
            if (ParentClass.StartedMeleeTesting) {
                MA_T;
                if (seh_me.IsValid()) seh_me.Release();
                while (ParentClass.StartedMeleeTesting) { Game->NewScriptFrame(); MA_T; }
                MA_T;
                MA_START;
                seh_me.ClearCommands();
                Game->EntitySheatheWeapons(Me, false);
            }
            if (Game->IsQuestActive("Q_GuildTrainingWoodsWill")) {
                MA_T;
                if (!WillWoodsChatDone) {
                    MA_T;
                    seh_me.MoveToPosition(Game->GetThingWithScriptName("MK_GTM_WD_HEROWALK").GetPos(), 3.0f, 1, false, true);
                    WillWoodsChatDone = true;
                } else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    MA_T;
                    seh_me.ClearCommands();
                    MA_MOAN("TEXT_QST_028_WHISPER_SCORPION_WOODS");
                    seh_me.MoveToPosition(Game->GetThingWithScriptName("MK_GTM_WD_HEROWALK").GetPos(), 3.0f, 1, false, true);
                }
            } else {
                MA_T;
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    MA_T;
                    seh_me.ClearCommands();
                    if (Game->IsQuestActive("Q_GuildTrainingSkill")) { MA_T; MA_MOAN("TEXT_QST_028_TEEN_WHISPER_SKILL_MOAN"); }
                    else if (Game->IsQuestActive("Q_GuildTrainingWill")) { MA_T; MA_MOAN("TEXT_QST_028_TEEN_WHISPER_WILL_MOAN"); }
                    else if (Game->IsQuestActive("Q_GuildTrainingDeparture")) {
                        MA_T;
                        if (Game->IsQuestActive("Q_GuildTrainingWoodsDeparture")) { MA_T; MA_MOAN("TEXT_QST_028_WHISPER_END_MOAN"); }
                        else { MA_T; MA_MOAN("TEXT_QST_028_WHISPER_MELEE_MOAN"); }
                    }
                    seh_me.MoveToPosition(marker.GetPos(), 3.0f, 1, false, true);
                }
                if (IsDistanceBetweenThingsOver(Me, marker, 4.0f) && !seh_me.IsPerformingScriptTask()) {
                    MA_T;
                    seh_me.MoveToPosition(marker.GetPos(), 3.0f, 1, false, true);
                }
            }
        }
    }
};
static void reset() { g_frame = 0; g_moves = g_speaks = g_movies = g_pauses = g_starts = g_releases = g_clears = 0;
    g_sheathe = g_killable = g_boss = g_friends = g_faction = 0; g_valid = g_performing = false; g_lastSpeak[0] = 0; g_lastMoveX = 0; }

int main()
{
    CGame g; g.Hero.Id = 3; g.Hero.Pos = g_null; CQuest q; q.StartedMeleeTesting = false; CMaster m; m.MeleeApprenticeNeededForCutscene = false;
    int ok = 1;
    // woods Will: one run to the hero-walk marker, then the scorpion moan on the talk frame (and the run again)
    reset(); g_woods = true; g_endFrame = 6; g_talkFrame = 3; g_farFrame = -1;
    { CMeleeApprentice a(&g, q, &m); a.Main(); ok = ok && a.WillWoodsChatDone; }
    ok = ok && g_starts == 1 && g_sheathe == 1 && g_faction == 1 && g_killable == 1 && g_boss == 1 && g_friends == 1
        && g_moves == 2 && g_lastMoveX == 9.0f && g_lastSpeed == 3.0f && g_lastMoveType == 1 && g_speaks == 1
        && !std::strcmp(g_lastSpeak, "TEXT_QST_028_WHISPER_SCORPION_WOODS") && g_movies == 1 && g_pauses == 0 && g_clears == 1 && g_frame == 6;
    // training: the departure moan (woods departure) and the run back to the marker, then a stray run back
    reset(); g_woods = false; g_departure = true; g_woodsDeparture = true; g_endFrame = 8; g_talkFrame = 2; g_farFrame = 5;
    { CMeleeApprentice a(&g, q, &m); a.Main(); }
    ok = ok && g_speaks == 1 && !std::strcmp(g_lastSpeak, "TEXT_QST_028_WHISPER_END_MOAN") && g_moves == 2 && g_lastMoveX == 7.0f && g_pauses == 0;
    // skill quest wins over departure
    reset(); g_skill = true; g_endFrame = 4; g_talkFrame = 1; g_farFrame = -1;
    { CMeleeApprentice a(&g, q, &m); a.Main(); }
    ok = ok && g_speaks == 1 && !std::strcmp(g_lastSpeak, "TEXT_QST_028_TEEN_WHISPER_SKILL_MOAN") && g_moves == 1;
    // terminated before the loop: no marker work
    reset(); g_endFrame = 0;
    { CMeleeApprentice a(&g, q, &m); a.Main(); }
    ok = ok && g_starts == 1 && g_sheathe == 0 && g_moves == 0;
    if (!ok) { std::printf("00d40cf0_TEST FAIL\n"); return 1; }
    std::printf("00d40cf0_TEST PASS\n");
    return 0;
}