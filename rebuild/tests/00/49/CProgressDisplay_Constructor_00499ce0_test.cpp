// Behaviour test for 0x00499CE0 CProgressDisplay::CProgressDisplay.
#include <cstdio>
#include <cstring>
#include <new>
static const double g_ProgressTimeZero = 0.0;
template <class T> struct CVector { T* s; T* f; T* e; CVector() : s(0), f(0), e(0) {} };
struct Display {
    CVector<int> ProgressTextures, GuildSealTextures, LoadingScreenTextures; long LoadingScreenIndex;
    CVector<int> TextTags, TextTagsPC; long TextTagIndex;
    long GuildSealFrameIndex, StageCount; double LastUpdateTime, LastScreenUpdate, LastProgressUpdate;
    bool DebugDisplay, Initialised, FullScreen, BlackScreen, DisplayText, DisplayResetProgressTipText,
         ForRelaunch, ForFrontEnd, ForFileInstaller, FileInstallerFinished;
    Display() : LoadingScreenIndex(0), TextTagIndex(0)
    {
        LastUpdateTime = g_ProgressTimeZero; LastScreenUpdate = g_ProgressTimeZero;
        FullScreen = true; DisplayText = true; LastProgressUpdate = g_ProgressTimeZero;
        GuildSealFrameIndex = 0; StageCount = 0;
        DebugDisplay = false; Initialised = false; BlackScreen = false; DisplayResetProgressTipText = false;
        ForRelaunch = false; ForFrontEnd = false; ForFileInstaller = false; FileInstallerFinished = false;
    }
};
int main()
{
    unsigned char raw[sizeof(Display)]; std::memset(raw, 0xAB, sizeof raw);
    Display* d = new (raw) Display;
    if (d->ProgressTextures.s || d->TextTagsPC.e || d->LoadingScreenIndex || d->TextTagIndex || d->StageCount ||
        d->GuildSealFrameIndex || d->LastUpdateTime != 0.0 || d->LastProgressUpdate != 0.0 ||
        !d->FullScreen || !d->DisplayText || d->DebugDisplay || d->Initialised || d->BlackScreen || d->FileInstallerFinished) {
        std::printf("00499ce0_TEST FAIL\n"); return 1;
    }
    std::printf("00499ce0_TEST PASS\n");
    return 0;
}
