// Behaviour test for CNewFrontendGameComponent::Run (0x0042EC7C), modelled on src/0042ec7c.cpp: the start-up movie table and the
// per-movie flow (clear colour, skip flag, failure -> media player flag + error string, quit check returns false), the front-end
// bank sequence (opened only with runtime level files), the init sequence (menu-font glyph preload), the main loop (quit on
// Update() == 2 or Quit, attract movie after 120 s with the language suffix), and the three exits (developer front end -> true,
// create game -> leave-frontend banks + main game -> true, otherwise false), each retiring the component.
#include <cstdio>
#include <cstring>

struct CMovieInfo { const char* FilePath; long Width; long Height; unsigned long ClearColour; long DelayMS; long Flags; float NoSkipDuration; };

static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

static int g_played = 0;
static int g_fail_movie = -1;
static bool PlayMovie(const char* name, char* error)
{
    ++g_played;
    Log(name);
    if (g_played - 1 == g_fail_movie) { std::strcpy(error, "no wmp"); return false; }
    return true;
}

struct CBankFileManager {
    void OpenRetailBank(const char* dir, const char* platform, const char* file, bool startup)
    {
        char s[128];
        std::sprintf(s, "open %s%s%s %d", dir, platform, file, startup ? 1 : 0);
        Log(s);
    }
    void CloseRetailBank(const char* dir, const char* platform, const char* file)
    {
        char s[128];
        std::sprintf(s, "close %s%s%s", dir, platform, file);
        Log(s);
    }
};
static CBankFileManager GBankFileManager;
static bool g_bRuntimeLevelFiles = true;
static bool g_bFrontEndFlag_01375448 = true;
static bool g_bFrontEndFlag_137544A = true;
static void* g_pRetiredGameComponent = 0;

struct CFrontEnd {
    bool WindowsMediaPlayerInstalled;
    char MediaPlayerErrorString[32];
    unsigned long ClearColour;
    bool SkipOnAnyButton;
    bool Running, Quit, CreateDevFrontEnd, CreateGame;
    int updates_before_quit;
    int frames;
    float elapsed_per_frame, attract_elapsed;
    unsigned long language;
    char preloaded[64];
    int attract_movies;
    CFrontEnd() : WindowsMediaPlayerInstalled(true), ClearColour(1), SkipOnAnyButton(false), Running(false), Quit(false),
                  CreateDevFrontEnd(false), CreateGame(false), updates_before_quit(3), frames(0), elapsed_per_frame(50.0f),
                  attract_elapsed(0.0f), language(7), attract_movies(0)
    { MediaPlayerErrorString[0] = 0; preloaded[0] = 0; }

    int SystemUpdate() { return updates_before_quit-- <= 0 ? 2 : 0; }
    void Preload(const char* glyphs) { std::strcpy(preloaded, glyphs); }

    void PlayAttractMovie()
    {
        char movie[64];
        std::strcpy(movie, "attract");
        switch (language) {
        case 4:  std::strcat(movie, "English"); break;
        case 7:  std::strcat(movie, "German"); break;
        case 10: std::strcat(movie, "Spanish"); break;
        case 12: std::strcat(movie, "French"); break;
        case 16: std::strcat(movie, "Italian"); break;
        case 17: std::strcat(movie, "English"); break;
        case 18: std::strcat(movie, "English"); break;
        default: std::strcat(movie, "English"); break;
        }
        std::strcat(movie, ".wmv");
        SkipOnAnyButton = true;
        char error[32] = "";
        PlayMovie(movie, error);
        SkipOnAnyButton = false;
        attract_elapsed = 0.0f;
        ++attract_movies;
    }

    // returns 0 = false, 1 = true (developer front end), 2 = true (main game)
    int Run()
    {
        CMovieInfo movies[3] = {
            { "Data\\Video\\lionhead_logo.xmv", 640, 400, 0xFFFFFFFFu, 0, 0x1a0, 20.0f },
            { "Data\\Video\\Microsoft_Logo.xmv", 640, 480, 0xFF000000u, 0, 0x1a0, 20.0f },
            { "Data\\Video\\intro_comp.xmv", 640, 360, 0x00000000u, 0, 0x1a0, 10.0f },
        };
        if (g_bFrontEndFlag_01375448 && g_bFrontEndFlag_137544A) {
            for (unsigned long i = 0; i < 3; ++i) {
                const CMovieInfo& movie = movies[i];
                ClearColour = movie.ClearColour;
                SkipOnAnyButton = true;
                char error_string[32] = "";
                if (!PlayMovie(movie.FilePath, error_string)) {
                    WindowsMediaPlayerInstalled = false;
                    std::strcpy(MediaPlayerErrorString, error_string);
                }
                SkipOnAnyButton = false;
                ClearColour = 0xFF000000u;
                if (SystemUpdate() == 2)
                    return 0;
            }
        }
        if (g_bRuntimeLevelFiles) {
            const char* platform = "pc\\";
            GBankFileManager.OpenRetailBank("graphics\\", platform, "frontend.big", true);
            GBankFileManager.OpenRetailBank("dialogue\\", "", "text.big", false);
            GBankFileManager.OpenRetailBank("shaders\\", platform, "shaders.big", false);
        }
        Running = true;
        {
            const char glyphs[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz()";
            Preload(glyphs);
        }
        while (SystemUpdate() != 2 && !Quit) {
            ++frames;
            attract_elapsed += elapsed_per_frame;
            if (attract_elapsed >= 120.0f)
                PlayAttractMovie();
        }
        Quit = true;
        Running = false;
        int result;
        if (CreateDevFrontEnd) {
            Log("dev");
            result = 1;
        } else if (CreateGame) {
            g_bFrontEndFlag_01375448 = false;
            if (g_bRuntimeLevelFiles) {
                const char* platform = "pc\\";
                GBankFileManager.CloseRetailBank("graphics\\", platform, "frontend.big");
                GBankFileManager.OpenRetailBank("graphics\\", "", "graphics.big", true);
                GBankFileManager.OpenRetailBank("graphics\\", platform, "textures.big", true);
                GBankFileManager.OpenRetailBank("dialogue\\", "", "dialogue.big", false);
                GBankFileManager.OpenRetailBank("misc\\", platform, "effects.big", false);
            }
            Log("game");
            g_pRetiredGameComponent = this;
            return 2;
        } else {
            result = 0;
        }
        g_pRetiredGameComponent = this;
        return result;
    }
};

static int Fail() { std::printf("0042ec7c_TEST FAIL\n"); return 1; }

int main()
{
    // 1. all movies, the second fails; main loop runs three frames (one attract movie at 150 s); create game.
    g_log[0] = 0; g_played = 0; g_fail_movie = 1;
    CFrontEnd fe;
    fe.CreateGame = true; fe.updates_before_quit = 6;   // three movie checks, then three frames
    int r = fe.Run();
    if (r != 2 || fe.WindowsMediaPlayerInstalled || std::strcmp(fe.MediaPlayerErrorString, "no wmp") != 0 || fe.ClearColour != 0xFF000000u
        || fe.SkipOnAnyButton || fe.Running || !fe.Quit || g_bFrontEndFlag_01375448 || g_pRetiredGameComponent != &fe)
        return Fail();
    if (std::strcmp(fe.preloaded, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz()") != 0 || fe.attract_movies != 1)
        return Fail();
    const char* expect1 =
        "Data\\Video\\lionhead_logo.xmv;Data\\Video\\Microsoft_Logo.xmv;Data\\Video\\intro_comp.xmv;"
        "open graphics\\pc\\frontend.big 1;open dialogue\\text.big 0;open shaders\\pc\\shaders.big 0;"
        "attractGerman.wmv;"
        "close graphics\\pc\\frontend.big;open graphics\\graphics.big 1;open graphics\\pc\\textures.big 1;"
        "open dialogue\\dialogue.big 0;open misc\\pc\\effects.big 0;game;";
    if (std::strcmp(g_log, expect1) != 0) { std::printf("%s\n", g_log); return Fail(); }

    // 2. movies skipped (flag cleared by run 1), no runtime level files, developer front end.
    g_log[0] = 0; g_played = 0; g_fail_movie = -1; g_bRuntimeLevelFiles = false; g_pRetiredGameComponent = 0;
    CFrontEnd dev;
    dev.CreateDevFrontEnd = true; dev.updates_before_quit = 1; dev.language = 17;
    if (dev.Run() != 1 || g_played != 0 || std::strcmp(g_log, "dev;") != 0 || g_pRetiredGameComponent != &dev || !dev.WindowsMediaPlayerInstalled)
        return Fail();

    // 3. quit during the first movie: returns false before any bank is opened and before the component retires.
    g_log[0] = 0; g_played = 0; g_bFrontEndFlag_01375448 = true; g_bRuntimeLevelFiles = true; g_pRetiredGameComponent = 0;
    CFrontEnd quit;
    quit.updates_before_quit = 0;
    if (quit.Run() != 0 || g_played != 1 || std::strcmp(g_log, "Data\\Video\\lionhead_logo.xmv;") != 0 || g_pRetiredGameComponent != 0
        || quit.Running)
        return Fail();

    // 4. neither exit flag: front end quits through the else arm (false), the component still retires; attract in English (lang 4).
    g_log[0] = 0; g_played = 0; g_bFrontEndFlag_01375448 = false;
    CFrontEnd plain;
    plain.language = 4; plain.elapsed_per_frame = 130.0f;   // movies skipped: three frames, an attract movie each
    if (plain.Run() != 0 || g_pRetiredGameComponent != &plain || plain.attract_movies != 3
        || std::strstr(g_log, "attractEnglish.wmv") == 0 || std::strstr(g_log, "close") != 0)
        return Fail();

    std::printf("0042ec7c_TEST PASS\n");
    return 0;
}