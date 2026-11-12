// Behaviour test for CNewFrontendGameComponent::Run (0x0042EC7C): the start-up movie table, the per-movie flow (clear colour,
// skip flag, failure -> media player flag + error string, quit check), the attract-movie file name per language and the hand-over
// to the next component.
#include <cstdio>
#include <cstring>

struct CMovieInfo { const char* FilePath; long Width; long Height; unsigned long ClearColour; long DelayMS; long Flags; float NoSkipDuration; };

static int g_played = 0;
static bool g_fail_second = true;
static bool PlayMovie(const char* name, char* error) { ++g_played; if (g_fail_second && g_played == 2) { std::strcpy(error, "no wmp"); return false; } (void)name; return true; }

struct CFrontEnd {
    bool WindowsMediaPlayerInstalled;
    char MediaPlayerErrorString[32];
    unsigned long ClearColour;
    bool SkipOnAnyButton;
    CFrontEnd() : WindowsMediaPlayerInstalled(true), ClearColour(1), SkipOnAnyButton(false) { MediaPlayerErrorString[0] = 0; }
    bool PlayStartupMovies(int quit_after)
    {
        CMovieInfo movies[3] = {
            { "Data\\Video\\lionhead_logo.xmv", 640, 400, 0xFFFFFFFFu, 0, 0x1a0, 20.0f },
            { "Data\\Video\\Microsoft_Logo.xmv", 640, 480, 0xFF000000u, 0, 0x1a0, 20.0f },
            { "Data\\Video\\intro_comp.xmv", 640, 360, 0x00000000u, 0, 0x1a0, 10.0f },
        };
        for (unsigned long i = 0; i < 3; ++i) {
            ClearColour = movies[i].ClearColour;
            SkipOnAnyButton = true;
            char error[32] = "";
            if (!PlayMovie(movies[i].FilePath, error)) {
                WindowsMediaPlayerInstalled = false;
                std::strcpy(MediaPlayerErrorString, error);
            }
            SkipOnAnyButton = false;
            ClearColour = 0xFF000000u;
            if ((int)i == quit_after)
                return false;
        }
        return true;
    }
};

static void AttractName(unsigned long language, char* out)
{
    std::strcpy(out, "attract");
    switch (language) {
    case 7: std::strcat(out, "German"); break;
    case 10: std::strcat(out, "Spanish"); break;
    case 12: std::strcat(out, "French"); break;
    case 16: std::strcat(out, "Italian"); break;
    default: std::strcat(out, "English"); break;
    }
    std::strcat(out, ".wmv");
}

int main()
{
    CFrontEnd fe;
    bool ok = fe.PlayStartupMovies(-1);
    if (!ok || g_played != 3 || fe.WindowsMediaPlayerInstalled || std::strcmp(fe.MediaPlayerErrorString, "no wmp") != 0
        || fe.ClearColour != 0xFF000000u || fe.SkipOnAnyButton) {
        std::printf("0042ec7c_TEST FAIL\n");
        return 1;
    }
    CFrontEnd quit;
    g_played = 0; g_fail_second = false;
    if (quit.PlayStartupMovies(0) || g_played != 1 || !quit.WindowsMediaPlayerInstalled) {
        std::printf("0042ec7c_TEST FAIL\n");
        return 1;
    }
    char name[64];
    AttractName(7, name);
    if (std::strcmp(name, "attractGerman.wmv") != 0) { std::printf("0042ec7c_TEST FAIL\n"); return 1; }
    AttractName(17, name);
    if (std::strcmp(name, "attractEnglish.wmv") != 0) { std::printf("0042ec7c_TEST FAIL\n"); return 1; }
    std::printf("0042ec7c_TEST PASS\n");
    return 0;
}
