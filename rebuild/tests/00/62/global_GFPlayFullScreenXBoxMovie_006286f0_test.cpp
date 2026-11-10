// behaviour test for 0x006286F0 GFPlayFullScreenXBoxMovie: the row's control flow transcribed over mock managers.
// Checks: bullet-time cancel only for a live hero holding the component; the 0x10 sound fade and its restore (volumes from
// the profile, the new-profile copies when UseNewProfileValues is set); the 0x02 pause flags cleared and set again; the
// .xmv -> .wmv path; the text-bank fallback (flag 0x08); an Init failure formats 0x%08X, closes and deletes the player and
// returns false without restoring the sound state; EC_COMPLETE, a skip key (Escape / Space / Return / Alt+F4) and the
// system's quit code (2) each end playback, with the texture and the player released and true returned.
#include <stdio.h>
#include <string.h>

static char g_log[4096];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static int g_fail = 0;
static void Check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }

struct Profile { bool UseNew; bool Sub, SubNew; float Sound, Music, Dialogue, SoundNew, MusicNew, DialogueNew;
    bool IsSubtitlingEnabled() const { return UseNew ? SubNew : Sub; }
    float GetSoundVolume() const { return UseNew ? SoundNew : Sound; }
    float GetMusicVolume() const { return UseNew ? MusicNew : Music; }
    float GetDialogueVolume() const { return UseNew ? DialogueNew : Dialogue; } };
static Profile g_profile;

struct SoundManager { float vol[256]; bool e25, e26;
    void SetGainFader(unsigned long g, float v, float fade) { char b[64]; sprintf(b, "vol%lx=%g/%g", g, v, fade); Log(b); vol[g] = v; }
    void SetEnabled25(bool b) { e25 = b; Log(b ? "e25=1" : "e25=0"); }
    void SetEnabled26(bool b) { e26 = b; Log(b ? "e26=1" : "e26=0"); } };
static SoundManager g_sound; static SoundManager* PSoundManager = 0;

struct BulletTime { void ImmediateCancel() { Log("bt-cancel"); } };
struct Hero { bool dead; BulletTime* bt; };
static Hero* g_hero = 0; static bool g_haveGame = false; static int g_mainTextBank = 0;

static int g_initResult = 0; static int g_completeAfter = 99; static int g_keyAt = -1; static int g_key = 0; static bool g_alt = false;
static int g_quitAfter = 99; static int g_frames = 0;
static int g_videos = 0, g_textures = 0; static int* g_lastBank = 0;

struct Video { int* bank; bool subs; float volume;
    Video() : bank(0), subs(false), volume(-1) { ++g_videos; }
    ~Video() { --g_videos; Log("video-dtor"); }
    long Init(const char* path) { Log(path); g_lastBank = bank; return g_initResult; }
    void Close() { Log("close"); }
    void Play() { Log("play"); }
    void Stop() { Log("stop"); }
    bool Event() { return g_frames >= g_completeAfter; } };
struct Texture { Texture() { ++g_textures; } ~Texture() { --g_textures; Log("tex-dtor"); } };

static bool Update() { return g_frames >= g_quitAfter; }
static bool ScanKey() {   // EventScanStart / EventScanNext over this frame's single key event
    if (g_frames != g_keyAt) return false;
    if (g_key == 1 || g_key == 0x39 || g_key == 0x1c || (g_key == 0x3e && g_alt)) return true;
    return false; }

static bool PlayMovie(const char* file, unsigned long flags, int* text_bank, char* perror)
{
    if (g_haveGame) {
        Hero* hero = g_hero;
        if (hero && !hero->dead) {
            if (hero->bt) hero->bt->ImmediateCancel();
        }
    }
    if ((flags & 0x10) && PSoundManager) {
        PSoundManager->SetGainFader(8, g_profile.GetDialogueVolume(), 0.0f);
        PSoundManager->SetGainFader(0x10, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x40, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x80, 0.0f, 500.0f);
    }
    if (flags & 2) { PSoundManager->SetEnabled25(false); PSoundManager->SetEnabled26(false); }
    char path[256]; strcpy(path, file);
    char* ext = strstr(path, ".xmv"); if (ext) memcpy(ext, ".wmv", 4);
    Video* video = new Video;
    if (!text_bank && (flags & 8)) text_bank = &g_mainTextBank;
    if (text_bank) video->bank = text_bank;
    long hr = video->Init(path);
    if (hr >= 0) {
        video->subs = g_profile.IsSubtitlingEnabled();
        video->volume = g_profile.GetMusicVolume();
        video->Play();
        Texture* texture = new Texture;
        if (!Update()) {
            do {
                if (video->Event()) break;
                if (ScanKey()) { video->Stop(); goto finished; }
                ++g_frames; Log("frame");
            } while (!Update());
        }
finished:
        delete texture;
        video->Close();
        delete video;
        if ((flags & 0x10) && PSoundManager) {
            PSoundManager->SetGainFader(8, 0.0f, 0.0f);
            PSoundManager->SetGainFader(0x10, g_profile.GetSoundVolume(), 0.0f);
            PSoundManager->SetGainFader(0x40, g_profile.GetDialogueVolume(), 0.0f);
            PSoundManager->SetGainFader(0x80, g_profile.GetSoundVolume(), 0.0f);
        }
        if (flags & 2) { PSoundManager->SetEnabled25(true); PSoundManager->SetEnabled26(true); }
        return true;
    }
    if (perror) sprintf(perror, "0x%08X", (unsigned)hr);
    video->Close();
    delete video;
    return false;
}

static void Reset() { g_log[0] = 0; g_frames = 0; g_completeAfter = 99; g_keyAt = -1; g_key = 0; g_alt = false; g_quitAfter = 99;
    g_initResult = 0; g_hero = 0; g_haveGame = false; PSoundManager = &g_sound; g_sound.e25 = g_sound.e26 = true; }

int main()
{
    g_profile.UseNew = false; g_profile.Sub = true; g_profile.SubNew = false;
    g_profile.Sound = 0.5f; g_profile.Music = 0.25f; g_profile.Dialogue = 0.75f;
    g_profile.SoundNew = 0.1f; g_profile.MusicNew = 0.2f; g_profile.DialogueNew = 0.3f;
    char err[32];
    // 1. Init failure: message, close, delete, false, no restore
    Reset(); g_initResult = (int)0x80004005; err[0] = 0;
    Check(!PlayMovie("Data\\Video\\a.xmv", 0x12, 0, err), "init failure returns false");
    Check(strcmp(err, "0x80004005") == 0, "error text 0x%08X");
    Check(strstr(g_log, "Data\\Video\\a.wmv;close;video-dtor") != 0, "wmv path, close, delete");
    Check(strstr(g_log, "e25=1") == 0 && strstr(g_log, "vol8=0/0") == 0, "no restore after a failure");
    Check(g_videos == 0 && g_textures == 0, "nothing leaked on failure");
    // 2. EC_COMPLETE after two frames, with fade + pause flags
    Reset(); g_completeAfter = 2;
    Check(PlayMovie("m.xmv", 0x12, 0, 0), "complete returns true");
    Check(strstr(g_log, "vol8=0.75/0;vol10=0/500;vol40=0/500;vol80=0/500;e25=0;e26=0;m.wmv;play;frame;frame;tex-dtor;close;video-dtor;vol8=0/0;vol10=0.5/0;vol40=0.75/0;vol80=0.5/0;e25=1;e26=1;") != 0, "fade/pause sequence");
    Check(g_videos == 0 && g_textures == 0, "released after completion");
    // 3. skip keys
    int keys[4] = { 1, 0x39, 0x1c, 0x3e };
    for (int k = 0; k < 4; ++k) { Reset(); g_keyAt = 1; g_key = keys[k]; g_alt = true;
        Check(PlayMovie("m.xmv", 0, 0, 0), "skip returns true");
        Check(strstr(g_log, "frame;stop;tex-dtor;close;video-dtor;") != 0, "skip stops after one frame"); }
    Reset(); g_keyAt = 0; g_key = 0x3e; g_alt = false; g_quitAfter = 3;
    PlayMovie("m.xmv", 0, 0, 0); Check(strstr(g_log, "stop") == 0 && g_frames == 3, "F4 without Alt does not skip");
    Reset(); g_keyAt = 0; g_key = 0x10; g_quitAfter = 2;
    PlayMovie("m.xmv", 0, 0, 0); Check(strstr(g_log, "stop") == 0 && g_frames == 2, "other key does not skip");
    // 4. system quit before the first frame
    Reset(); g_quitAfter = 0;
    Check(PlayMovie("m.xmv", 0, 0, 0) && g_frames == 0 && strstr(g_log, "play;tex-dtor;close;video-dtor;") != 0, "quit before first frame");
    // 5. bullet time: live hero with the component cancels, dead hero does not
    BulletTime bt; Hero h; h.dead = false; h.bt = &bt;
    Reset(); g_haveGame = true; g_hero = &h; g_quitAfter = 0; PlayMovie("m.xmv", 0, 0, 0); Check(strncmp(g_log, "bt-cancel;", 10) == 0, "bullet time cancelled");
    h.dead = true; Reset(); g_haveGame = true; g_hero = &h; g_quitAfter = 0; PlayMovie("m.xmv", 0, 0, 0); Check(strstr(g_log, "bt-cancel") == 0, "dead hero untouched");
    // 6. text bank: argument wins, flag 8 falls back to the main game's bank, else none
    {   int own = 0; Reset(); g_quitAfter = 0; PlayMovie("m.xmv", 8, &own, 0); Check(g_lastBank == &own, "own text bank kept");
        Reset(); g_quitAfter = 0; PlayMovie("m.xmv", 8, 0, 0); Check(g_lastBank == &g_mainTextBank, "flag 8 falls back to the main bank");
        Reset(); g_quitAfter = 0; PlayMovie("m.xmv", 0, 0, 0); Check(g_lastBank == 0, "no bank without flag 8"); }
    // 7. new-profile values
    g_profile.UseNew = true; Reset(); g_completeAfter = 0;
    PlayMovie("m.xmv", 0x10, 0, 0); Check(strstr(g_log, "vol8=0.3/0;") != 0 && strstr(g_log, "vol10=0.1/0;vol40=0.3/0;vol80=0.1/0;") != 0, "new-profile volumes");
    // 8. no sound manager: the fade is skipped
    g_profile.UseNew = false; Reset(); PSoundManager = 0; g_completeAfter = 0;
    PlayMovie("m.xmv", 0x10, 0, 0); Check(strstr(g_log, "vol") == 0, "no fade without a sound manager");
    if (!g_fail) printf("006286f0_TEST PASS\n");
    return g_fail;
}