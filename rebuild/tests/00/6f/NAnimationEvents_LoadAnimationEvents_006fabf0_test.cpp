// Behaviour test for 0x006FABF0 NAnimationEvents::LoadAnimationEvents(bool): the progress lines appear only when asked,
// and the game then sound managers load <misc dir>game_animation_events.txt / sound_animation_events.txt, in that order
#include <cstdio>
#include <cstring>

namespace model {

char g_log[512];
void log_line(const char* a, const char* b) { std::strcat(g_log, a); std::strcat(g_log, b); std::strcat(g_log, ";"); }

struct CManager {
    const char* Name;
    void Load(const char* fileName) { log_line(Name, fileName); }
};
CManager g_game = { "game:" };
CManager g_sound = { "sound:" };
CManager& GetGameEventManager() { return g_game; }
CManager& GetSoundEventManager() { return g_sound; }

void DisplayProgress(const char* text, float fraction, bool, bool)
{
    if (fraction == -1.0f) log_line("progress:", text);
}
const char* GetMiscDir() { return "data/misc/"; }

void LoadAnimationEvents(bool displayProgress)
{
    char path[128];
    if (displayProgress)
        DisplayProgress("Loading Game Animation Events", -1.0f, false, false);
    std::strcpy(path, GetMiscDir());
    std::strcat(path, "game_animation_events.txt");
    GetGameEventManager().Load(path);
    if (displayProgress)
        DisplayProgress("Loading Sound Animation Events", -1.0f, false, false);
    std::strcpy(path, GetMiscDir());
    std::strcat(path, "sound_animation_events.txt");
    GetSoundEventManager().Load(path);
}

}

int main()
{
    using namespace model;
    int bad = 0;
    g_log[0] = 0;
    LoadAnimationEvents(true);
    if (std::strcmp(g_log, "progress:Loading Game Animation Events;game:data/misc/game_animation_events.txt;"
                           "progress:Loading Sound Animation Events;sound:data/misc/sound_animation_events.txt;") != 0) bad = 1;
    g_log[0] = 0;
    LoadAnimationEvents(false);
    if (std::strcmp(g_log, "game:data/misc/game_animation_events.txt;sound:data/misc/sound_animation_events.txt;") != 0) bad = 2;
    if (bad) { std::printf("006fabf0_TEST FAIL\n"); return 1; }
    std::printf("006fabf0_TEST PASS\n");
    return 0;
}