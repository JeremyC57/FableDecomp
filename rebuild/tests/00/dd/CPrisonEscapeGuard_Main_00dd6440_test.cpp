#include <cstdio>
#include <string>
#include <vector>
// behaviour test: CPrisonEscapeGuard::Main -- when the alert timer is 0 and the guard sees the hero (or is hit by him, except
// by special ability 14) it sounds the alarm once (ALARM animation, whistle, then the quest's bandit alarm) and shouts one of
// the two "seen criminal" lines, restarting the 30-second timer; termination ends the watch
static int g_frames = 0, g_endAt = 6, g_seenAt = 2, g_timerValue = 0;
static std::vector<std::string> g_log;
struct CScriptThing {
    bool hitBy, hitAny, hitAbility14;
    bool MsgIsHitBy(const char*) const { return hitBy; }
    bool MsgIsHitByAnySpecialAbilityFrom(const char*) const { return hitAny; }
    bool MsgIsHitBySpecialAbilityFrom(int ability, const char*) const { return ability == 14 && hitAbility14; }
};
struct CHandle {
    int busy;
    CHandle() : busy(0) {}
    void PlayAnimation(const char* a) { g_log.push_back(a); busy = 2; }
    bool IsPerformingScriptTask() const { return busy > 0; }
};
struct CGameScriptInterface {
    CScriptThing hero;
    CHandle* active;
    void NewScriptFrame() { ++g_frames; if (g_timerValue > 0) --g_timerValue; if (active && active->busy) --active->busy; }
    bool StartScriptingEntity(const CScriptThing&, CHandle& h) { active = &h; return true; }
    CScriptThing& GetHero() { return hero; }
    bool CanThingBe_Seen_ByOtherThing(const CScriptThing&, const CScriptThing&) const { return g_frames >= g_seenAt; }
    void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) const {}
    void Pause(float) const {}
    void PlaySoundOnThing(const CScriptThing&, const char* s) const { g_log.push_back(s); }
    void Play2DSound(const char* s) const { g_log.push_back(s); }
    long AddNewConversation(const CScriptThing&, bool, bool) { return 3; }
    void AddPersonToConversation(long, const CScriptThing&) {}
    void AddLineToConversation(long, const char* text, bool, const CScriptThing&, const CScriptThing&) { g_log.push_back(text); }
};
struct CQ_PrisonEscapeRescueMotherScript { bool AlarmSounded; };
struct CPrisonEscapeGuard {
    CGameScriptInterface* Game; CScriptThing Me; CQ_PrisonEscapeRescueMotherScript* ParentClass; long line;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void Main();
};
void CPrisonEscapeGuard::Main()
{
    while (!IsActiveThreadTerminating()) {
        if (g_timerValue == 0
            && (Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(), Me) || Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO")))) {
            if (!ParentClass->AlarmSounded) {
                CHandle guard;
                Game->StartScriptingEntity(Me, guard);
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                guard.PlayAnimation("ALARM");
                Game->Pause(2.5f);
                Game->PlaySoundOnThing(Me, "SND_RACEWHISTLE");
                while (guard.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                Game->active = 0;
                if (!ParentClass->AlarmSounded) { ParentClass->AlarmSounded = true; Game->Play2DSound("SND_BANDIT_ALARM"); }
            }
            g_timerValue = 30;
            long c = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(c, Game->GetHero());
            Game->AddLineToConversation(c, line == 1 ? "TEXT_AI_GUARD_SEEN_CRIMINAL" : "TEXT_AI_GUARD_SEEN_CRIMINAL_2", false, Me, Game->GetHero());
        }
        Game->NewScriptFrame();
    }
}
int main()
{
    CGameScriptInterface game; game.active = 0;
    CQ_PrisonEscapeRescueMotherScript quest; quest.AlarmSounded = false;
    CPrisonEscapeGuard g; g.Game = &game; g.ParentClass = &quest; g.line = 2;
    g.Me.hitBy = false; g.Me.hitAny = true; g.Me.hitAbility14 = true;   // hit only by ability 14: not an alert
    g.Main();
    bool ok = quest.AlarmSounded && g_log.size() == 4 && g_log[0] == "ALARM" && g_log[1] == "SND_RACEWHISTLE"
        && g_log[2] == "SND_BANDIT_ALARM" && g_log[3] == "TEXT_AI_GUARD_SEEN_CRIMINAL_2" && g_frames == 6;
    g_log.clear(); g_frames = 0; g_seenAt = 1000; g_timerValue = 0; g.line = 1; g.Me.hitAbility14 = false;   // a real hit
    g.Main();
    ok = ok && g_log.size() == 1 && g_log[0] == "TEXT_AI_GUARD_SEEN_CRIMINAL";   // alarm already sounded; timer blocks repeats
    if (!ok) { std::printf("00dd6440_TEST FAIL\n"); return 1; }
    std::printf("00dd6440_TEST PASS\n");
    return 0;
}