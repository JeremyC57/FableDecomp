// Behaviour test for 0x00ED8680 NScript::CV_TalentlessBardScript::CTB_Bard::Main: model plus the body verbatim from the row source,
// callees stubbed. Checks the alive predicate on Me, the opening frame and early exit, the two spawned watchers (names, parent, member,
// by-value Me), the hit reaction (melee or a non-heal special ability, only while alive: face, take over, movie, pause, the on-hit line),
// the talk reaction (intro once then a random return line, the song question, fee paid and the verses sung with chords, no-cash and
// rejected lines), terminate exits inside the blocks, and that every exit releases the scripted-thing handle, movie and copies.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#ifndef _MSC_VER
#define _vsnprintf vsnprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

static char g_log[8192];
static void Log(const char* fmt, ...) { char b[256]; va_list a; va_start(a, fmt); _vsnprintf(b, sizeof(b) - 1, fmt, a); b[sizeof(b) - 1] = 0; va_end(a); strcat(g_log, b); strcat(g_log, ";"); }
static int g_frames = 0, g_iatt = 0, g_termAt = 1 << 30, g_termFrame = 1 << 30;
static int g_live = 0;                  // live CScriptThing copies
static int g_movies = 0, g_pauses = 0, g_handles = 0, g_spawned = 0;
static bool Term() { ++g_iatt; return g_iatt >= g_termAt || g_frames >= g_termFrame; }

class CCharString { public:
    char Text[128];
    CCharString() { Text[0] = 0; }
    CCharString(const char* t, long len = -1) { (void)len; strncpy(Text, t, 127); Text[127] = 0; }
    CCharString(const CCharString& o) { strcpy(Text, o.Text); }
    CCharString& operator=(const CCharString& o) { strcpy(Text, o.Text); return *this; }
    void Sprintf(const char* fmt, ...) { va_list a; va_start(a, fmt); _vsnprintf(Text, 127, fmt, a); Text[127] = 0; va_end(a); }
};
CCharString operator+(const CCharString& l, const CCharString& r) { CCharString s(l); strcat(s.Text, r.Text); return s; }

enum EHeroAbility { HERO_ABILITY_HEAL_LIFE_SPELL = 14 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE = 0, GROUP_SELECT_RANDOM = 1, GROUP_SELECT_RANDOM_NO_REPEAT = 2 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };

static int g_hitAt = -1, g_hitKind = 0;  // 1 melee, 2 heal-life spell, 3 another special ability
static int g_talkAt[4] = { -1, -1, -1, -1 };
class CScriptThing { public:
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    ~CScriptThing() { --g_live; }
    bool MsgIsHitBy(const CCharString& a) const { CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_hitAt && g_hitKind == 1; }
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& a) const { CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_hitAt && g_hitKind >= 2; }
    bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ab, const CCharString& a) const { CHECK(ab == HERO_ABILITY_HEAL_LIFE_SPELL); CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_hitAt && g_hitKind == 2; }
    bool MsgIsTalkedToBy(const CCharString& a) const { CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); for (int i = 0; i < 4; i++) if (g_talkAt[i] == g_frames) return true; return false; }
};
static float g_health = 100.0f;
static long g_gold = 0, g_answer = 1;
static int g_startFails = 0, g_speakFrames = 0, g_answerWait = 0;
class CScriptGameResourceObjectScriptedThingBase { public:
    bool Valid; int Busy;
    CScriptGameResourceObjectScriptedThingBase() : Valid(false), Busy(0) { ++g_handles; }
    ~CScriptGameResourceObjectScriptedThingBase() { --g_handles; }
    bool IsValid() const { return Valid; }
    void Release() { CHECK(Valid); Valid = false; Log("release"); }
    CScriptThing GetScriptThing() const { CHECK(Valid); return CScriptThing(7); }
    void Speak(const CScriptThing& to, const char* key, ETextGroupSelectionMethod m, bool a, bool b, bool c) { CHECK(to.Id == 1); CHECK(Valid); CHECK(!a && b && !c); Log("say %s %d", key, (int)m); Busy = g_speakFrames; }
    void Speak(const CScriptThing& to, const CCharString& key, ETextGroupSelectionMethod m, bool a, bool b, bool c) { Speak(to, key.Text, m, a, b, c); }
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
    void EntitySetFacingAngleTowardsThing(const CScriptThing& who, const CScriptThing& to, bool b) { CHECK(who.Id == 1 && to.Id == 7 && !b); Log("face"); }
    bool StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& h, EScriptAIPriority p) { CHECK(t.Id == 7 && p == SCRIPT_AI_PRIORITY_HIGHEST); if (g_startFails > 0) { g_startFails--; return false; } h.Valid = true; Log("start"); return true; }
    void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase&) { CHECK(name.Text[0] == 0); CHECK(g_movies > 0); Log("movie"); }
    void GiveHeroYesNoQuestion(const CCharString& q, const CCharString& y, const CCharString& n, const CCharString& e, bool b) { CHECK(strcmp(q.Text, "TEXT_QST_163_QUESTION_SONG") == 0 && strcmp(y.Text, "TEXT_OBJECT_HERO_ANSWER_YES") == 0 && strcmp(n.Text, "TEXT_OBJECT_HERO_ANSWER_NO") == 0 && e.Text[0] == 0 && b); Log("ask"); }
    long MsgIsQuestionAnsweredYesOrNo() { return g_answerWait-- > 0 ? -1 : g_answer; }
    long GetHeroGold() { return g_gold; }
    void GiveHeroGold(long d) { g_gold += d; Log("gold %ld", d); }
    void PlaySoundOnThing(const CScriptThing& t, const CCharString& s) { CHECK(t.Id == 7); Log("snd %s", s.Text); }
    void Pause(float s) { Log("pause %g", s); }
};
class CWideScreenMagicPauseEntities { public:
    explicit CWideScreenMagicPauseEntities(CGameScriptInterface&) { ++g_pauses; }
    ~CWideScreenMagicPauseEntities() { --g_pauses; }
};
class CScriptDef { public: long BardSingingFee; long BardNumVersesPerVisit; };
static CScriptDef g_def = { 50, 3 };
static CScriptDef* pGlobalScriptDef = &g_def;

class CSpawnedFuncBase { public: virtual ~CSpawnedFuncBase() { } CCharString Name; };
template <class T, class P1> class CSpawnedFunc1 : public CSpawnedFuncBase { public:
    CSpawnedFunc1(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(P1), P1 par1) : Func(func), PClass(pclass), Par1(par1) { Name = name; CHECK(id == 0); }
    void (T::*Func)(P1); T* PClass; P1 Par1;
};
namespace NScript {
class CIsThingAlive { public: CScriptThing Thing; CIsThingAlive(const CScriptThing& t) : Thing(t) { } };
class CV_TalentlessBardScript { public:
    bool BardIntroComplete;
    CSpawnedFuncBase* Spawned[4];
    CV_TalentlessBardScript() : BardIntroComplete(false) { }
    void AddSpawnedFunction(CSpawnedFuncBase* f, const CCharString& region) { CHECK(region.Text[0] == 0); CHECK(g_spawned < 4); Spawned[g_spawned++] = f; }
    void WatchForAttack(CScriptThing) { }
    void WatchForResetWanderCentre(CScriptThing) { }
    class CTB_Bard { public:
        CGameScriptInterface* Game;
        CScriptThing Me;
        CV_TalentlessBardScript* ParentClass;
        int PredicateId;
        int Verse;
        CTB_Bard() : Me(7), PredicateId(0), Verse(0) { }
        void SetPredicate(const CIsThingAlive& p) { PredicateId = p.Thing.Id; }
        bool IsActiveThreadTerminating() { return Term(); }
        CCharString GetRandomSpeech() { char b[32]; sprintf(b, "VERSE%d", ++Verse); return CCharString(b); }
        void Main();
    };
};
}
void NScript::CV_TalentlessBardScript::CTB_Bard::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase sehMe;
    CCharString randomSpeech;
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CV_TalentlessBardScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForAttack"), 0, ParentClass, &CV_TalentlessBardScript::WatchForAttack, Me), CCharString(""));
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CV_TalentlessBardScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForResetWanderCentre"), 0, ParentClass, &CV_TalentlessBardScript::WatchForResetWanderCentre, Me), CCharString(""));
    if (false) {} else for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))
            && Game->GetHealth(Me) > 0.0f) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Game->GetHero(), Me, false);
            if (sehMe.IsValid())
                sehMe.Release();
            while (!Game->StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                sehMe.Speak(Game->GetHero(), "TEXT_QST_163_ON_HIT", GROUP_SELECT_NONE, false, true, false);
                while (sehMe.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (sehMe.IsValid())
                sehMe.Release();
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Game->GetHero(), Me, false);
            if (sehMe.IsValid())
                sehMe.Release();
            while (!Game->StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (!ParentClass->BardIntroComplete) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                    sehMe.Speak(Game->GetHero(), "TEXT_QST_163_INTRO", GROUP_SELECT_NONE, false, true, false);
                    while (sehMe.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                ParentClass->BardIntroComplete = true;
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CCharString return_tag;
                return_tag.Sprintf("TEXT_QST_163_RETURN%d", rand() % 4 + 1);
                if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                    sehMe.Speak(Game->GetHero(), return_tag, GROUP_SELECT_NONE, false, true, false);
                    while (sehMe.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            Game->GiveHeroYesNoQuestion("TEXT_QST_163_QUESTION_SONG", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long YNQAnswer;
            while ((YNQAnswer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (YNQAnswer == 1) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (Game->GetHeroGold() > pGlobalScriptDef->BardSingingFee) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game->GiveHeroGold(-pGlobalScriptDef->BardSingingFee);
                    if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                        sehMe.Speak(Game->GetHero(), "TEXT_QST_163_START", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (sehMe.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (false) {} else for (int verse = 0; verse < pGlobalScriptDef->BardNumVersesPerVisit; verse++) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Game->PlaySoundOnThing(Me, "SND_MANDOLINCHORD_01");
                        Game->Pause(2.0f);
                        randomSpeech = GetRandomSpeech();
                        if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                            sehMe.Speak(Game->GetHero(), randomSpeech, GROUP_SELECT_NONE, false, true, false);
                            while (sehMe.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        Game->Pause(1.0f);
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    CCharString return_tag;
                    return_tag.Sprintf("TEXT_QST_163_NO_CASH%d", rand() % 2 + 1);
                    if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                        sehMe.Speak(Game->GetHero(), return_tag, GROUP_SELECT_NONE, false, true, false);
                        while (sehMe.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                }
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                    sehMe.Speak(Game->GetHero(), "TEXT_QST_163_REJECTED", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (sehMe.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            if (sehMe.IsValid())
                sehMe.Release();
        }
    }
}

static void Reset()
{
    g_log[0] = 0; g_frames = 0; g_iatt = 0; g_termAt = 1 << 30; g_termFrame = 1 << 30; g_hitAt = -1; g_hitKind = 0;
    for (int i = 0; i < 4; i++) g_talkAt[i] = -1;
    g_health = 100.0f; g_gold = 0; g_answer = 1; g_startFails = 0; g_speakFrames = 0; g_answerWait = 0; g_spawned = 0;
}
static void Run(NScript::CV_TalentlessBardScript& parent, bool keep = false)
{
    CGameScriptInterface game;
    {
        NScript::CV_TalentlessBardScript::CTB_Bard bard;
        bard.Game = &game; bard.ParentClass = &parent;
        bard.Main();
        CHECK(bard.PredicateId == 7);
        CHECK(g_live == 1 + g_spawned);           // Me plus the spawned watchers' by-value copies
        CHECK(g_handles == 0 && g_movies == 0 && g_pauses == 0);
    }
    if (keep) return;
    for (int i = 0; i < g_spawned; i++) delete parent.Spawned[i];
    CHECK(g_live == 0);
}
static bool Has(const char* s) { return strstr(g_log, s) != 0; }

int main()
{
    // 1. terminating at the first check: one frame, nothing spawned
    { Reset(); NScript::CV_TalentlessBardScript p; g_termAt = 1; Run(p); CHECK(g_frames == 1 && g_spawned == 0 && g_log[0] == 0); }
    // 2. idle bard: both watchers spawned on Me, one frame per loop pass
    { Reset(); NScript::CV_TalentlessBardScript p; g_termFrame = 5; Run(p, true);
      CHECK(g_spawned == 2); CHECK(g_log[0] == 0); CHECK(g_frames == 5);
      typedef CSpawnedFunc1<NScript::CV_TalentlessBardScript, CScriptThing> F;
      F* a = (F*)p.Spawned[0]; F* b = (F*)p.Spawned[1];
      CHECK(strcmp(a->Name.Text, "ParentClass.WatchForAttack") == 0 && a->Func == &NScript::CV_TalentlessBardScript::WatchForAttack && a->PClass == &p && a->Par1.Id == 7);
      CHECK(strcmp(b->Name.Text, "ParentClass.WatchForResetWanderCentre") == 0 && b->Func == &NScript::CV_TalentlessBardScript::WatchForResetWanderCentre && b->Par1.Id == 7);
      delete a; delete b; CHECK(g_live == 0); }
    // 3. melee hit while alive: face, take over (after one failed start), on-hit line, release
    { Reset(); NScript::CV_TalentlessBardScript p; g_hitAt = 2; g_hitKind = 1; g_startFails = 1; g_speakFrames = 2; g_termFrame = 8; Run(p);
      CHECK(strcmp(g_log, "face;start;movie;say TEXT_QST_163_ON_HIT 0;release;") == 0); }
    // 4. heal-life spell: no reaction; another special ability: reaction; dead bard: no reaction
    { Reset(); NScript::CV_TalentlessBardScript p; g_hitAt = 2; g_hitKind = 2; g_termFrame = 6; Run(p); CHECK(g_log[0] == 0); }
    { Reset(); NScript::CV_TalentlessBardScript p; g_hitAt = 2; g_hitKind = 3; g_termFrame = 6; Run(p); CHECK(Has("say TEXT_QST_163_ON_HIT 0")); }
    { Reset(); NScript::CV_TalentlessBardScript p; g_hitAt = 2; g_hitKind = 1; g_health = 0.0f; g_termFrame = 6; Run(p); CHECK(g_log[0] == 0); }
    // 5. first talk, yes with the fee: intro, ask, pay, start line, three verses with chord and pauses
    { Reset(); NScript::CV_TalentlessBardScript p; g_talkAt[0] = 2; g_gold = 60; g_answerWait = 2; g_termFrame = 40; Run(p);
      CHECK(strcmp(g_log, "face;start;movie;say TEXT_QST_163_INTRO 0;ask;gold -50;say TEXT_QST_163_START 2;"
                          "snd SND_MANDOLINCHORD_01;pause 2;say VERSE1 0;pause 1;snd SND_MANDOLINCHORD_01;pause 2;say VERSE2 0;pause 1;"
                          "snd SND_MANDOLINCHORD_01;pause 2;say VERSE3 0;pause 1;release;") == 0);
      CHECK(p.BardIntroComplete && g_gold == 10); }
    // 6. later talk, answer no: random return line, rejected line
    { Reset(); NScript::CV_TalentlessBardScript p; p.BardIntroComplete = true; g_talkAt[0] = 3; g_answer = 0; g_termFrame = 20;
      srand(11); char want[96]; sprintf(want, "say TEXT_QST_163_RETURN%d 0;ask;say TEXT_QST_163_REJECTED 2;release;", rand() % 4 + 1);
      srand(11); Run(p); CHECK(strstr(g_log, want) != 0); }
    // 7. yes without the fee: no-cash line, gold untouched
    { Reset(); NScript::CV_TalentlessBardScript p; p.BardIntroComplete = true; g_talkAt[0] = 3; g_gold = 50; g_termFrame = 20;
      srand(3); int r1 = rand() % 4 + 1; int r2 = rand() % 2 + 1; char want[96];
      sprintf(want, "say TEXT_QST_163_RETURN%d 0;ask;say TEXT_QST_163_NO_CASH%d 0;release;", r1, r2);
      srand(3); Run(p); CHECK(strstr(g_log, want) != 0 && g_gold == 50); }
    // 8. terminating inside the talk block (while the intro is spoken) and inside the hit block (waiting to take over)
    { Reset(); NScript::CV_TalentlessBardScript p; g_talkAt[0] = 2; g_speakFrames = 50; g_termFrame = 6; Run(p);
      CHECK(Has("say TEXT_QST_163_INTRO 0") && !Has("ask") && !Has("release") && !p.BardIntroComplete); }
    { Reset(); NScript::CV_TalentlessBardScript p; g_hitAt = 2; g_hitKind = 1; g_startFails = 100; g_termFrame = 6; Run(p);
      CHECK(strcmp(g_log, "face;") == 0); }
    // 9. hit and talk in the same frame: both blocks run in order
    { Reset(); NScript::CV_TalentlessBardScript p; p.BardIntroComplete = true; g_hitAt = 2; g_hitKind = 1; g_talkAt[0] = 2; g_answer = 0; g_termFrame = 30; Run(p);
      CHECK(strncmp(g_log, "face;start;movie;say TEXT_QST_163_ON_HIT 0;release;face;start;movie;say TEXT_QST_163_RETURN", 82) == 0); }
    if (g_fail) printf("00ed8680_TEST FAIL %d\n", g_fail); else printf("00ed8680_TEST PASS\n");
    return g_fail ? 1 : 0;
}