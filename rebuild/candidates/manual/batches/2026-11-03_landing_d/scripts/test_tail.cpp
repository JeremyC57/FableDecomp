
static void Reset(CV_AmbushScamScript& s, GameCalls* g)
{
    std::memset(&s, 0, sizeof(s)); s.Game = g; g_frame = 0; g_log_n = 0; g_termAt = 1000;
    for (int i = 0; i < 16; ++i) g_regionAt[i] = 0;
}
int main()
{
    GameCalls g; CV_AmbushScamScript s;
    // Good path: Witchwood1 at frame 1 (ignored), Witchwood2 from Witchwood1 at frame 3; morality 3 -> good quest; talk ending.
    Reset(s, &g); g_regionAt[1] = "Witchwood1"; g_regionAt[3] = "Witchwood2"; g_entranceData = "FromWitchwood1"; g_morality = 3;
    s.HeroTalkToTrickster = true; g_regionAt[8] = "Witchwood1";
    s.Main();
    CHECK(FindLog("Bind:AS_Ambusher") == 0 && FindLog("Bind:AS_Trickster") == 1 && FindLog("Bind:AS_Buddy") == 2);
    CHECK(FindLog("PostAdd") == 3 && FindLog("Game.PostAdd") == 4 && FindLog("Gen-Witchwood2") == 5);
    CHECK(FindLog("Activate:V_AmbushScamGood") == 6 && s.MoralitySwitch && s.QuestActivated);
    CHECK(s.TricksterApproachProximity == 7.5f);
    CHECK(FindLog("Spawn:WatchForQuestFinished") == 7 && FindLog("Spawn:WatchForRegionTransition") == 8);
    CHECK(FindLog("Approach") == 9 && FindLog("Ambush") == 10 && s.TricksterApproachComplete && s.AmbushSpringComplete);
    CHECK(FindLog("GoodEnding") == 11 && FindLog("EvilEnding") < 0);
    CHECK(FindLog("Gen+Witchwood2") == 12 && FindLog("Deactivate:V_AmbushScamGood") == 13 && FindLog("Deactivate:ActiveQuest") == 14);
    CHECK(g_log_n == 15 && g_frame == 8);
    // Evil path: quest already active, Witchwood2 at frame 0 breaks at once (no re-activation); the after-ambush attack
    // gives the evil ending; LeftRegion ends the last wait.
    Reset(s, &g); g_regionAt[0] = "Witchwood2"; g_entranceData = ""; g_morality = 1; s.QuestActivated = true;
    s.HeroAttackOnTricksterAfterAmbush = true; s.LeftRegion = true;
    s.Main();
    CHECK(FindLog("Activate:V_AmbushScamGood") < 0 && FindLog("Activate:V_AmbushScamEvil") < 0 && !s.MoralitySwitch);
    CHECK(FindLog("EvilEnding") >= 0 && FindLog("GoodEnding") < 0 && FindLog("Deactivate:V_AmbushScamEvil") >= 0);
    // Not activated yet, evil morality: activates the evil quest.
    Reset(s, &g); g_regionAt[0] = "Witchwood2"; g_entranceData = "FromWitchwood1"; g_morality = 1; s.LeftRegion = true;
    s.Main();
    CHECK(FindLog("Activate:V_AmbushScamEvil") >= 0 && !s.MoralitySwitch && FindLog("Deactivate:V_AmbushScamEvil") >= 0);
    // Wrong entrance: keeps waiting until the thread terminates; nothing activated, generators stay off.
    Reset(s, &g); g_regionAt[0] = "Witchwood2"; g_entranceData = "FromOakvale"; g_termAt = 5;
    s.Main();
    CHECK(!s.QuestActivated && FindLog("Gen-Witchwood2") >= 0 && FindLog("Gen+Witchwood2") < 0 && FindLog("Spawn:WatchForQuestFinished") < 0);
    // Terminating straight away: returns after binding and disabling the generators.
    Reset(s, &g); g_termAt = 0;
    s.Main();
    CHECK(g_log_n == 6 && FindLog("Gen-Witchwood2") == 5);
    if (g_fail) { std::printf("00e2b090_TEST FAIL\n"); return 1; }
    std::printf("00e2b090_TEST PASS\n");
    return 0;
}
