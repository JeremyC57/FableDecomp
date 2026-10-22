// Behaviour test for CAIStateGroup_MinionGuardGreetPlayer::Update (0x00929820): model copy, scripted stubs.
#include <cstdio>
#include <cstring>

struct CVillage { int NextGreetFrame; };
struct CThing {
    unsigned char m_flags91;
    bool hasStats;
    int stats114;
    CVillage* village;
    unsigned int greetGroup;
};
static CThing* g_player;
static CThing* g_creature;
static int g_frame;
static float g_opinion;
static int g_speakResult;
static unsigned int g_spoken;
static int g_terminated;
static char g_lastText[64];

static unsigned int TagFromText(const char* text)
{
    if (std::strcmp(text, "TEXT_AI_GUARD_GREET_NEUTRAL") == 0) return 101;
    if (std::strcmp(text, "TEXT_AI_GUARD_GREET_FRIENDLY") == 0) return 102;
    if (std::strcmp(text, "TEXT_AI_GUARD_GREET_WARY") == 0) return 103;
    return 0;
}
static int Speak(unsigned int speech)
{
    g_spoken = speech;
    return g_speakResult;
}

static void Update()
{
    CThing* player = g_player;
    if (player && !(player->m_flags91 & 1)) {
        CVillage* village = g_creature->village;
        float next = (float)(g_frame + 120);
        village->NextGreetFrame = (int)next;
        unsigned int speech;
        if (player->hasStats && player->stats114 == 0) {
            const char* text;
            float opinion = g_opinion;
            text = "TEXT_AI_GUARD_GREET_NEUTRAL";
            if (opinion > 0.3f)
                text = "TEXT_AI_GUARD_GREET_FRIENDLY";
            if (opinion < -0.3f)
                text = "TEXT_AI_GUARD_GREET_WARY";
            std::strcpy(g_lastText, text);
            speech = TagFromText(text);
        } else {
            speech = g_creature->greetGroup;
        }
        float when;
        if (speech && Speak(speech) == 2)
            when = (float)(g_frame + 1200);
        else
            when = (float)(g_frame + 120);
        g_creature->village->NextGreetFrame = (int)when;
    }
    ++g_terminated;
}

int main()
{
    int ok = 1;
    CVillage village = { 0 };
    CThing hero = { 0, true, 0, 0, 0 };
    CThing guard = { 0, false, 0, &village, 55 };
    g_player = &hero; g_creature = &guard; g_frame = 1000;
    // neutral opinion, spoken: next greet 1200 frames on
    g_opinion = 0.0f; g_speakResult = 2;
    Update();
    if (g_spoken != 101 || village.NextGreetFrame != 2200 || g_terminated != 1) ok = 0;
    // friendly / wary thresholds; not spoken -> 120 frames on
    g_opinion = 0.5f; g_speakResult = 1; g_spoken = 0;
    Update();
    if (g_spoken != 102 || village.NextGreetFrame != 1120) ok = 0;
    g_opinion = -0.5f; g_speakResult = 2;
    Update();
    if (g_spoken != 103 || village.NextGreetFrame != 2200) ok = 0;
    g_opinion = 0.3f;
    Update();
    if (g_spoken != 101) ok = 0;
    // the hero-stats field set: the creature opinion's greet group
    hero.stats114 = 4;
    Update();
    if (g_spoken != 55 || village.NextGreetFrame != 2200) ok = 0;
    // no greet group: nothing spoken, 120 frames on
    guard.greetGroup = 0; g_spoken = 7;
    Update();
    if (g_spoken != 7 || village.NextGreetFrame != 1120) ok = 0;
    // dead or absent player: only Terminate
    village.NextGreetFrame = 9; hero.m_flags91 = 1;
    Update();
    g_player = 0;
    Update();
    if (village.NextGreetFrame != 9 || g_terminated != 8) ok = 0;
    if (!ok) { std::printf("00929820_TEST FAIL\n"); return 1; }
    std::printf("00929820_TEST PASS\n");
    return 0;
}
