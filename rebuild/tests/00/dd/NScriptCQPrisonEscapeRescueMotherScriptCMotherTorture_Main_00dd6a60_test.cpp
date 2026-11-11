// Behaviour test for NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture::Main (0x00DD6A60): the hero's mother in the prison
// torture chamber. Brain state 1: every 10 frames (and when talked to, as a speech) she nags GET_WEAPONS until the hero has his weapons,
// then USE_LEVER until her cell door is open; the lever used without weapons gets the two NOSTUFF lines and objective 03, with weapons
// it sets brain state 2, the auto-save flag and saves. Then (torture chamber loaded) the door opens, GOT_WEAPONS, the kraken quest is
// activated, will is usable again. Brain state 2: MotherFollowing, objective 06, HURRY, follow the hero with a health bar; until she
// dodges the kraken: more than 3 running combat hits say CANT_HELP every frame, a hero hit says HIT_BY_HERO, and when the 15-frame comment timer is out an enemy hit says HIT_BY_ENEMY, more than
// 3 running hits with rand()%100==0 FIGHTING_COMPLIMENT, flourish enabled with rand()%200==0 USE_FLOURISH, rand()%400==0 NOT_FAR_NOW.
// While she dodges the kraken rand()%200==0 says FLEEING_KRAKEN; the same comment loop then runs until the Graveyard region loads,
// where the hero is teleported to the region entrance (when it exists) and the script idles. A frame-stepped model.
#include <cstdio>
#include <cstring>
static char g_log[8192];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Mother {
    long BrainState; bool HeroHasFoundWeapons, MotherCellDoorOpen, MotherFollowing, AutoSaveCheckPoint, MotherDodgingKraken;
    int Frame, TerminateAt, TalkAt, LeverAt[2], WeaponsAt, HeroHitAt, EnemyHitAt, KrakenFrom, KrakenTo, GraveyardAt, RunningHits, RandZeroAt[4], NRandZero;
    bool ChamberLoaded, Flourish, EntranceAlive;
    bool T() const { return Frame >= TerminateAt; }
    void NSF(long* timer) { ++Frame; if (timer && *timer > 0) --*timer; if (Frame == WeaponsAt) HeroHasFoundWeapons = true; MotherDodgingKraken = Frame >= KrakenFrom && Frame < KrakenTo; }
    bool RandZero() const { for (int i = 0; i < NRandZero; ++i) if (RandZeroAt[i] == Frame) return true; return false; }   // rand() % n == 0 on these frames
    void Comment(long& comment_timer)
    {
        if (Frame == HeroHitAt) { Log("TEXT_QST_055_MOTHER_HIT_BY_HERO"); comment_timer = 15; }
        else if (comment_timer == 0) {
            if (Frame == EnemyHitAt) { Log("TEXT_QST_055_MOTHER_HIT_BY_ENEMY"); comment_timer = 15; }
            else if (RunningHits > 3 && RandZero()) { Log("TEXT_QST_055_MOTHER_FIGHTING_COMPLIMENT"); comment_timer = 15; }
            else if (Flourish && RandZero()) { Log("TEXT_QST_055_MOTHER_USE_FLOURISH"); comment_timer = 15; }
            else if (RandZero()) { Log("TEXT_QST_055_MOTHER_NOT_FAR_NOW"); comment_timer = 15; }
        }
    }
    void Main()
    {
        NSF(0); if (T()) return;
        if (BrainState == 1) Log("StartScripting");
        long nag_timer = 10;
        while (BrainState == 1) {
            NSF(&nag_timer); if (T()) return;
            if (nag_timer == 0) {
                if (!HeroHasFoundWeapons) { nag_timer = 10; Log("nag:TEXT_QST_055_MOTHER_GET_WEAPONS"); }
                else if (!MotherCellDoorOpen) { nag_timer = 10; Log("nag:TEXT_QST_055_MOTHER_USE_LEVER"); }
            }
            if (Frame == TalkAt) {
                if (!HeroHasFoundWeapons) { nag_timer = 10; Log("say:TEXT_QST_055_MOTHER_GET_WEAPONS"); }
                else if (!MotherCellDoorOpen) { nag_timer = 10; Log("say:TEXT_QST_055_MOTHER_USE_LEVER"); }
            }
            if (Frame == LeverAt[0] || Frame == LeverAt[1]) {
                if (HeroHasFoundWeapons) { BrainState = 2; AutoSaveCheckPoint = true; Log("AutoSave"); }
                else { Log("TEXT_QST_055_PRISON_MOTHER_NOSTUFF_01"); Log("TEXT_QST_055_PRISON_MOTHER_NOSTUFF_02"); Log("OBJECTIVE_03"); }
            }
        }
        if (T()) return;
        if (ChamberLoaded) { MotherCellDoorOpen = true; Log("OpenDoor"); Log("TEXT_QST_055_MOTHER_GOT_WEAPONS"); Log("Q_TentacleKrakenBossFight"); }
        if (BrainState == 2) {
            MotherFollowing = true; Log("OBJECTIVE_06"); Log("TEXT_QST_055_MOTHER_HURRY"); Log("Follow"); Log("HealthBar");
            long comment_timer = 0;
            while (BrainState == 2 && !MotherDodgingKraken) {
                NSF(&comment_timer); if (T()) return;
                if (RunningHits > 3) Log("TEXT_QST_055_MOTHER_CANT_HELP");
                Comment(comment_timer);
            }
        }
        while (MotherDodgingKraken) {
            NSF(0); if (T()) return;
            if (RandZero()) Log("TEXT_QST_055_MOTHER_FLEEING_KRAKEN");
        }
        long comment_timer = 0;
        while (Frame < GraveyardAt) {
            NSF(&comment_timer); if (T()) return;
            Comment(comment_timer);
        }
        if (EntranceAlive) Log("TeleportHero");
        Log("Idle");
    }
};
static Mother Make()
{
    Mother m; std::memset(&m, 0, sizeof m);
    m.BrainState = 1; m.TerminateAt = 1000; m.TalkAt = -1; m.LeverAt[0] = m.LeverAt[1] = -1; m.WeaponsAt = -1; m.HeroHitAt = -1; m.EnemyHitAt = -1;
    m.KrakenFrom = m.KrakenTo = -1; m.GraveyardAt = 60;
    return m;
}
int main()
{
    bool ok = true;
    {   // the whole escape: lever without weapons, nag, weapons found, lever again, follow with comments, the kraken, the graveyard
        Mother m = Make(); m.LeverAt[0] = 3; m.TalkAt = 5; m.WeaponsAt = 14; m.LeverAt[1] = 22; m.ChamberLoaded = true;
        m.HeroHitAt = 25; m.EnemyHitAt = 41; m.KrakenFrom = 45; m.KrakenTo = 50; m.RandZeroAt[0] = 47; m.RandZeroAt[1] = 58; m.NRandZero = 2; m.EntranceAlive = true;
        m.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;TEXT_QST_055_PRISON_MOTHER_NOSTUFF_01;TEXT_QST_055_PRISON_MOTHER_NOSTUFF_02;OBJECTIVE_03;"
            "say:TEXT_QST_055_MOTHER_GET_WEAPONS;nag:TEXT_QST_055_MOTHER_USE_LEVER;AutoSave;OpenDoor;TEXT_QST_055_MOTHER_GOT_WEAPONS;"
            "Q_TentacleKrakenBossFight;OBJECTIVE_06;TEXT_QST_055_MOTHER_HURRY;Follow;HealthBar;TEXT_QST_055_MOTHER_HIT_BY_HERO;"
            "TEXT_QST_055_MOTHER_HIT_BY_ENEMY;TEXT_QST_055_MOTHER_FLEEING_KRAKEN;TEXT_QST_055_MOTHER_NOT_FAR_NOW;TeleportHero;Idle;")
            && m.BrainState == 2 && m.MotherFollowing && m.MotherCellDoorOpen && m.AutoSaveCheckPoint;
    }
    g_log[0] = 0;
    {   // loaded in brain state 2 outside the chamber; compliment needs more than 3 running hits; thread terminated while following
        Mother m = Make(); m.BrainState = 2; m.RunningHits = 4; m.RandZeroAt[0] = 6; m.NRandZero = 1; m.Flourish = true; m.TerminateAt = 8;
        m.Main();
        ok = ok && !std::strcmp(g_log, "OBJECTIVE_06;TEXT_QST_055_MOTHER_HURRY;Follow;HealthBar;TEXT_QST_055_MOTHER_CANT_HELP;TEXT_QST_055_MOTHER_CANT_HELP;"
            "TEXT_QST_055_MOTHER_CANT_HELP;TEXT_QST_055_MOTHER_CANT_HELP;TEXT_QST_055_MOTHER_CANT_HELP;TEXT_QST_055_MOTHER_FIGHTING_COMPLIMENT;"
            "TEXT_QST_055_MOTHER_CANT_HELP;") && m.Frame == 8 && !m.MotherCellDoorOpen;
    }
    std::printf(ok ? "00dd6a60_TEST PASS\n" : "00dd6a60_TEST FAIL\n");
    return ok ? 0 : 1;
}