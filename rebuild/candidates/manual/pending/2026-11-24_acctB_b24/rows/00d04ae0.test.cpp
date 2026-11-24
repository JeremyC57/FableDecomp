// Behaviour test for 0x00D04AE0 NScript::CQ_BanditCampBossBattleScript::CAssassin1::Main: model plus the body verbatim from the row
// source, callees stubbed. Checks the alive predicate on Me, the opening frame and early exit, the take-over, the hit test (melee / non-heal
// special ability -> assassins under attack), the talk paths (gate 3 open: not needed; first talk: intro, question, yes with / without the
// fee, no; later talks: repeat question), the fee and experience, TalkedToAssassin, the hired / attacked tail (clear information, release,
// idle) and the plain idle tail, and that every exit releases the scripted-thing handle, movie, pause and copies.
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#ifndef _MSC_VER
#define _vsnprintf vsnprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

static char g_log[8192];
static void Log(const char* fmt, ...) { char b[256]; va_list a; va_start(a, fmt); _vsnprintf(b, sizeof(b) - 1, fmt, a); b[sizeof(b) - 1] = 0; va_end(a); strcat(g_log, b); strcat(g_log, ";"); }
static int g_frames = 0, g_iatt = 0, g_termAt = 1 << 30, g_termFrame = 1 << 30;
static int g_live = 0, g_movies = 0, g_pauses = 0, g_handles = 0;
static bool Term() { ++g_iatt; return g_iatt >= g_termAt || g_frames >= g_termFrame; }

class CCharString { public:
    char Text[128];
    CCharString(const char* t, long len = -1) { (void)len; strncpy(Text, t, 127); Text[127] = 0; }
    CCharString(const CCharString& o) { strcpy(Text, o.Text); }
};
enum EHeroAbility { HERO_ABILITY_HEAL_LIFE_SPELL = 14 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE = 0, GROUP_SELECT_RANDOM = 1 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };

static int g_hitAt = -1, g_hitKind = 0;          // 1 melee, 2 heal-life spell, 3 another special ability
static int g_talkAt[3] = { -1, -1, -1 };
class CScriptThing { public:
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    ~CScriptThing() { --g_live; }
    bool MsgIsHitBy(const CCharString& a) const { CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_hitAt && g_hitKind == 1; }
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& a) const { CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_hitAt && g_hitKind >= 2; }
    bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ab, const CCharString& a) const { CHECK(ab == HERO_ABILITY_HEAL_LIFE_SPELL); CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_hitAt && g_hitKind == 2; }
    bool MsgIsTalkedToBy(const CCharString& a) const { CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); for (int i = 0; i < 3; i++) if (g_talkAt[i] == g_frames) return true; return false; }
};
static float g_health = 100.0f;
static long g_gold = 0, g_answer = 1, g_xp = 0;
static int g_startFails = 0, g_speakFrames = 0, g_answerWait = 0;
class CScriptGameResourceObjectScriptedThingBase { public:
    bool Valid; int Busy;
    CScriptGameResourceObjectScriptedThingBase() : Valid(false), Busy(0) { ++g_handles; }
    ~CScriptGameResourceObjectScriptedThingBase() { --g_handles; }
    bool IsValid() const { return Valid; }
    void Release() { Valid = false; Log("release"); }
    CScriptThing GetScriptThing() const { CHECK(Valid); return CScriptThing(7); }
    void Speak(const CScriptThing& to, const char* key, ETextGroupSelectionMethod m, bool a, bool b, bool c) { CHECK(to.Id == 1); CHECK(Valid); CHECK(m == GROUP_SELECT_NONE && !a && b && !c); CHECK(g_movies == 1 && g_pauses == 1); Log("say %s", key); Busy = g_speakFrames; }
    bool IsPerformingScriptTask() { return Busy-- > 0; }
};
class TRetVoid { public: void Return() { } };
class CScriptGameResourceObjectMovieBase { public:
    CScriptGameResourceObjectMovieBase() { ++g_movies; }
    ~CScriptGameResourceObjectMovieBase() { --g_movies; }
};
class CGameScriptInterface { public:
    void NewScriptFrame() { ++g_frames; }
    float GetHealth(const CScriptThing& t) { CHECK(t.Id == 7); return g_health; }
    CScriptThing GetHero() { return CScriptThing(1); }
    bool StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& h, EScriptAIPriority p) { CHECK(t.Id == 7 && p == SCRIPT_AI_PRIORITY_HIGHEST); if (g_startFails > 0) { g_startFails--; return false; } h.Valid = true; Log("start"); return true; }
    void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase&) { CHECK(name.Text[0] == 0); CHECK(g_movies > 0); Log("movie"); }
    void GiveHeroYesNoQuestion(const CCharString& q, const CCharString& y, const CCharString& n, const CCharString& e, bool b) {
        CHECK(strcmp(y.Text, "TEXT_OBJECT_HERO_ANSWER_YES") == 0 && strcmp(n.Text, "TEXT_OBJECT_HERO_ANSWER_NO") == 0 && e.Text[0] == 0 && b); Log("ask %s", q.Text); }
    long MsgIsQuestionAnsweredYesOrNo() { return g_answerWait-- > 0 ? -1 : g_answer; }
    long GetHeroGold() { return g_gold; }
    void GiveHeroGold(long d) { g_gold += d; Log("gold %ld", d); }
    void GiveHeroExperience(long x) { g_xp += x; Log("xp %ld", x); }
    void ClearThingHasInformation(const CScriptThing& t) { CHECK(t.Id == 7); Log("clear info"); }
};
class CWideScreenMagicPauseEntities { public:
    explicit CWideScreenMagicPauseEntities(CGameScriptInterface&) { ++g_pauses; }
    ~CWideScreenMagicPauseEntities() { --g_pauses; }
};
struct CScriptDef { float BAC_AssassinGoldAmount; long EXPERIENCE_GRANT_SMALL; };
static CScriptDef g_def = { 500.0f, 25 };
static CScriptDef* pGlobalScriptDef = &g_def;
namespace NScript {
struct CIsThingAlive { CScriptThing Thing; CIsThingAlive(const CScriptThing& t) : Thing(t) { } };
class CQ_BanditCampBossBattleScript { public:
    bool Gate3Open, AssassinsUnderAttack, AssassinCutsceneTriggered, TalkedToAssassin;
    CQ_BanditCampBossBattleScript() : Gate3Open(false), AssassinsUnderAttack(false), AssassinCutsceneTriggered(false), TalkedToAssassin(false) { }
    class CAssassin1 { public:
        CGameScriptInterface* Game;
        CScriptThing Me;
        CQ_BanditCampBossBattleScript& ParentClass;
        int PredicateId;
        CAssassin1(CQ_BanditCampBossBattleScript& p) : Me(7), ParentClass(p), PredicateId(0) { }
        void SetPredicate(const CIsThingAlive& p) { PredicateId = p.Thing.Id; }
        bool IsActiveThreadTerminating() { return Term(); }
        void Main();
    };
};
}
void NScript::CQ_BanditCampBossBattleScript::CAssassin1::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    bool TalkedTo = false;
    while (!ParentClass.AssassinsUnderAttack && !ParentClass.AssassinCutsceneTriggered) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            ParentClass.AssassinsUnderAttack = true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (ParentClass.Gate3Open) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_NOT_NEEDED", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (!TalkedTo) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_INTRO", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    Game->GiveHeroYesNoQuestion("TEXT_QST_009_ASSASSIN1_INTRO_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer;
                    while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (answer == 1) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        if (Game->GetHeroGold() < pGlobalScriptDef->BAC_AssassinGoldAmount) {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_POOR", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating())
                                        return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                        } else {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating())
                                        return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            ParentClass.AssassinCutsceneTriggered = true;
                            Game->GiveHeroGold((long)-pGlobalScriptDef->BAC_AssassinGoldAmount);
                            Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
                        }
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_NO", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                    }
                    TalkedTo = true;
                    ParentClass.TalkedToAssassin = true;
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->GiveHeroYesNoQuestion("TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer;
                    while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (answer == 1) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        if (Game->GetHeroGold() < pGlobalScriptDef->BAC_AssassinGoldAmount) {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_POOR", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating())
                                        return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                        } else {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating())
                                        return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            ParentClass.AssassinCutsceneTriggered = true;
                            Game->GiveHeroGold((long)-pGlobalScriptDef->BAC_AssassinGoldAmount);
                            Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
                        }
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_NO", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                    }
                }
            }
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    if (ParentClass.AssassinCutsceneTriggered || ParentClass.AssassinsUnderAttack) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->ClearThingHasInformation(Me);
        seh_me.Release();
        while (true) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
    } else {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        while (true) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
    }
}

static void Reset()
{
    g_log[0] = 0; g_frames = 0; g_iatt = 0; g_termAt = 1 << 30; g_termFrame = 1 << 30; g_hitAt = -1; g_hitKind = 0;
    for (int i = 0; i < 3; i++) g_talkAt[i] = -1;
    g_health = 100.0f; g_gold = 0; g_answer = 1; g_xp = 0; g_startFails = 0; g_speakFrames = 0; g_answerWait = 0;
}
static void Run(NScript::CQ_BanditCampBossBattleScript& parent)
{
    CGameScriptInterface game;
    {
        NScript::CQ_BanditCampBossBattleScript::CAssassin1 a(parent);
        a.Game = &game;
        a.Main();
        CHECK(a.PredicateId == 7);
        CHECK(g_live == 1);
        CHECK(g_handles == 0 && g_movies == 0 && g_pauses == 0);
    }
    CHECK(g_live == 0);
}
static bool Has(const char* s) { return strstr(g_log, s) != 0; }

int main()
{
    typedef NScript::CQ_BanditCampBossBattleScript P;
    // 1. terminating at the first check: one frame, nothing else
    { Reset(); P p; g_termAt = 1; Run(p); CHECK(g_frames == 1 && g_log[0] == 0); }
    // 2. idle until terminated (take-over after one failed start)
    { Reset(); P p; g_startFails = 1; g_termFrame = 6; Run(p); CHECK(strcmp(g_log, "start;") == 0); CHECK(g_frames == 6); }
    // 3. hit by the hero: under attack, leave the loop, clear information, release, idle
    { Reset(); P p; g_hitAt = 3; g_hitKind = 1; g_termFrame = 8; Run(p);
      CHECK(strcmp(g_log, "start;clear info;release;") == 0); CHECK(p.AssassinsUnderAttack && !p.AssassinCutsceneTriggered); }
    // 4. heal-life spell: no reaction; another special ability: reaction
    { Reset(); P p; g_hitAt = 3; g_hitKind = 2; g_termFrame = 8; Run(p); CHECK(strcmp(g_log, "start;") == 0 && !p.AssassinsUnderAttack); }
    { Reset(); P p; g_hitAt = 3; g_hitKind = 3; g_termFrame = 8; Run(p); CHECK(p.AssassinsUnderAttack && Has("clear info")); }
    // 5. gate 3 open: not needed (speech waits two frames), no question
    { Reset(); P p; p.Gate3Open = true; g_talkAt[0] = 2; g_speakFrames = 2; g_termFrame = 9; Run(p);
      CHECK(strcmp(g_log, "start;movie;say TEXT_QST_009_ASSASSIN1_NOT_NEEDED;") == 0); CHECK(!p.TalkedToAssassin); }
    // 6. first talk, yes with the fee: intro, question, hire line, fee, experience, hired tail
    { Reset(); P p; g_talkAt[0] = 2; g_gold = 600; g_answerWait = 2; g_termFrame = 12; Run(p);
      CHECK(strcmp(g_log, "start;movie;say TEXT_QST_009_ASSASSIN1_INTRO;ask TEXT_QST_009_ASSASSIN1_INTRO_QUESTION;say TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES;gold -500;xp 25;clear info;release;") == 0);
      CHECK(p.AssassinCutsceneTriggered && p.TalkedToAssassin && g_gold == 100 && g_xp == 25); }
    // 7. first talk, yes but poor; then the repeat question, no
    { Reset(); P p; g_talkAt[0] = 2; g_talkAt[1] = 5; g_gold = 100; g_termFrame = 7; Run(p);
      CHECK(strncmp(g_log, "start;movie;say TEXT_QST_009_ASSASSIN1_INTRO;ask TEXT_QST_009_ASSASSIN1_INTRO_QUESTION;say TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_POOR;movie;ask TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION;say TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_POOR;", 220) == 0);
      CHECK(p.TalkedToAssassin && !p.AssassinCutsceneTriggered && g_gold == 100); }
    // 8. first talk no; later the repeat question yes with the fee: hired
    { Reset(); P p; g_talkAt[0] = 2; g_talkAt[1] = 4; g_answer = 0; g_gold = 500; g_termFrame = 12; Run(p);
      CHECK(Has("say TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_NO;movie;ask TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION;say TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_NO;")); CHECK(!p.AssassinCutsceneTriggered); }
    { Reset(); P p; p.TalkedToAssassin = true; g_talkAt[0] = 2; g_talkAt[1] = 4; g_answer = 0; g_termFrame = 3; Run(p);
      CHECK(Has("ask TEXT_QST_009_ASSASSIN1_INTRO_QUESTION;")); }
    // 9. dead assassin: no lines, question still asked
    { Reset(); P p; g_talkAt[0] = 2; g_health = 0.0f; g_answer = 0; g_termFrame = 5; Run(p);
      CHECK(strcmp(g_log, "start;movie;ask TEXT_QST_009_ASSASSIN1_INTRO_QUESTION;") == 0); CHECK(p.TalkedToAssassin); }
    // 10. terminating while speaking and while waiting for the answer: every object released
    { Reset(); P p; g_talkAt[0] = 2; g_speakFrames = 50; g_termFrame = 6; Run(p); CHECK(Has("say TEXT_QST_009_ASSASSIN1_INTRO") && !Has("ask")); }
    { Reset(); P p; g_talkAt[0] = 2; g_answerWait = 50; g_termFrame = 6; Run(p); CHECK(Has("ask") && !p.TalkedToAssassin); }
    // 11. cutscene triggered by another assassin: leave the loop to the hired tail
    { Reset(); P p; p.AssassinCutsceneTriggered = true; g_termFrame = 4; Run(p); CHECK(strcmp(g_log, "start;clear info;release;") == 0); }
    // 12. exactly the fee on the repeat question: hired (the poor test is strictly less)
    { Reset(); P p; p.TalkedToAssassin = true; g_talkAt[0] = 2; g_gold = 500; g_termFrame = 9; Run(p);
      CHECK(Has("ask TEXT_QST_009_ASSASSIN1_INTRO_QUESTION;say TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES;gold -500;xp 25;")); CHECK(g_gold == 0); }
    { Reset(); P p; g_talkAt[0] = 2; g_talkAt[1] = 4; g_answer = 1; g_gold = 0; g_termFrame = 3; Run(p); }
    { Reset(); P p; g_talkAt[0] = 2; g_talkAt[1] = 4; g_gold = 499; g_termFrame = 6; Run(p);
      CHECK(Has("REPEAT_QUESTION;say TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_POOR;")); }
    if (g_fail) printf("00d04ae0_TEST FAIL %d\n", g_fail); else printf("00d04ae0_TEST PASS\n");
    return g_fail ? 1 : 0;
}
