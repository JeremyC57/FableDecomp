// Behaviour test for CWillWhisper::Main (0x00D68810): setup calls, then per frame while bandits live: re-faction on a hero
// hit and play the cast animation once per raised whisper flag.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct Whisper {
    bool banditsAlive, whisperAnimate;
    int frames, terminateAt, hitAt, animateAt, banditsDieAt;
    void NewScriptFrame() { ++frames; Log("f"); if (frames == animateAt) whisperAnimate = true; if (frames == banditsDieAt) banditsAlive = false; }
    bool IsActiveThreadTerminating() const { return frames >= terminateAt; }
    bool MsgIsHitBy(const char*) const { return frames == hitAt; }
    bool MsgIsHitByAnySpecialAbilityFrom(const char*) const { return false; }
    bool MsgIsHitBySpecialAbilityFrom(long, const char*) const { return false; }
    void Main()
    {
        Log("pred");
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (IsActiveThreadTerminating()) return;
        Log("setup");
        while (banditsAlive) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (MsgIsHitBy("SCRIPT_NAME_HERO") || (MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return;
                Log("friend");
            }
            if (whisperAnimate == true) {
                if (IsActiveThreadTerminating()) return;
                whisperAnimate = false;
                Log("cast");
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
};

int main()
{
    Whisper w; w.banditsAlive = true; w.whisperAnimate = false; w.frames = 0; w.terminateAt = 99; w.hitAt = 2; w.animateAt = 3; w.banditsDieAt = 5;
    g_log[0] = 0; w.Main();
    if (std::strcmp(g_log, "pred,f,setup,f,friend,f,cast,f,f,") != 0 || w.whisperAnimate) { std::printf("%s 00d68810_TEST FAIL\n", g_log); return 1; }
    Whisper v = w; v.banditsAlive = true; v.frames = 0; v.terminateAt = 3; v.banditsDieAt = 99;
    g_log[0] = 0; v.Main();
    if (std::strcmp(g_log, "pred,f,setup,f,friend,f,") != 0) { std::printf("%s 00d68810_TEST FAIL\n", g_log); return 1; }
    std::printf("00d68810_TEST PASS\n");
    return 0;
}