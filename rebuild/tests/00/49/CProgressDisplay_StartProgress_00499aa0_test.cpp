// Behaviour test for CProgressDisplay::StartProgress 0x00499AA0: a copy of the model with recording stubs for the callees.
#include <cstdio>
#include <cstring>

static bool   g_bProgressDisplayEnabled;
static bool   g_bProgressDisplaySuppressed;
static bool   g_bAutoSaveProgressShown;
static double g_AutoSaveProgressEndTime;
static bool   g_ProgressDebugDisplay;
static bool   g_bCompileFrontendDefinitions;
static double g_now;
static int    g_wrapOption, g_wrapValue, g_wrapCalls;
static int    g_fontLoads;
static char   g_fontName[32];
static int    g_fontRefs;        // live references to the loaded font

static double GFGetTime() { return g_now; }
static void WordWrap_SetOption(unsigned int option, unsigned int value) { g_wrapOption = (int)option; g_wrapValue = (int)value; ++g_wrapCalls; }

struct Editor {
    bool active;
    virtual bool IsEditorActive() const { return active; }
};
static Editor* g_pMain;

struct FontRef {                 // counted pointer stand-in: counts live references
    int* count;
    FontRef() : count(0) { }
    FontRef(int* c) : count(c) { if (count) ++*count; }
    FontRef(const FontRef& o) : count(o.count) { if (count) ++*count; }
    ~FontRef() { if (count) --*count; count = 0; }
    FontRef& operator=(const FontRef& o)
    {
        if (count != o.count) {
            if (count) --*count;
            count = o.count;
            if (count) ++*count;
        }
        return *this;
    }
};
static FontRef GetFont(const char* name)
{
    ++g_fontLoads;
    std::strcpy(g_fontName, name);
    return FontRef(&g_fontRefs);
}

struct Display {
    long   LoadingScreenIndex;
    long   TextTagIndex;
    char   TextTag[16];
    char   LastTextDisplayed[16];
    long   GuildSealFrameIndex;
    long   StageCount;
    double LastUpdateTime, LastScreenUpdate, LastProgressUpdate;
    bool   DebugDisplay, Initialised, FullScreen, BlackScreen, DisplayText, DisplayResetProgressTipText;
    bool   ForRelaunch, ForFrontEnd, ForFileInstaller, FileInstallerFinished;
    static FontRef PFont;
    int    texCalls; bool texRelaunch, texFrontEnd; bool texSawInitialised;
    int    renderCalls; char renderText[64]; float renderProgress; bool renderA, renderB;

    virtual void InitialiseTextures(bool forRelaunch, bool forFrontEnd)
    {
        ++texCalls; texRelaunch = forRelaunch; texFrontEnd = forFrontEnd; texSawInitialised = Initialised;
    }
    void RenderProgress(const char* text, float progress, bool a, bool b)
    {
        ++renderCalls; std::strcpy(renderText, text); renderProgress = progress; renderA = a; renderB = b;
    }
    void StartProgress(bool fullScreen, bool forRelaunch, bool forFrontEnd, bool blackScreen, bool forFileInstaller, bool renderFlag)
    {
        if (!g_bProgressDisplayEnabled)
            return;
        if (!g_bProgressDisplaySuppressed) {
            if (Initialised)
                return;
            if (g_bAutoSaveProgressShown)
                g_AutoSaveProgressEndTime = GFGetTime() + 3.0;
        }
        LastUpdateTime = 0.0;
        LoadingScreenIndex = 0;
        LastScreenUpdate = 0.0;
        TextTagIndex = -1;
        LastProgressUpdate = 0.0;
        std::strcpy(TextTag, "");
        std::strcpy(LastTextDisplayed, "");
        GuildSealFrameIndex = 0;
        StageCount = 0;
        DebugDisplay = g_ProgressDebugDisplay;
        if ((g_pMain && g_pMain->IsEditorActive()) || g_bCompileFrontendDefinitions)
            DebugDisplay = true;
        Initialised = true;
        FullScreen = fullScreen;
        BlackScreen = blackScreen;
        DisplayText = true;
        DisplayResetProgressTipText = false;
        ForRelaunch = forRelaunch;
        ForFrontEnd = forFrontEnd;
        ForFileInstaller = forFileInstaller;
        FileInstallerFinished = false;
        InitialiseTextures(forRelaunch, forFrontEnd);
        WordWrap_SetOption(0, 1);
        PFont = GetFont("ENG_ARIAL_16");
        if (!DebugDisplay)
            RenderProgress("Initialising progress display", -1.0f, false, renderFlag);
    }
};
FontRef Display::PFont;

static void Dirty(Display& d)
{
    d.LoadingScreenIndex = 7; d.TextTagIndex = 5; std::strcpy(d.TextTag, "TAG"); std::strcpy(d.LastTextDisplayed, "LAST");
    d.GuildSealFrameIndex = 9; d.StageCount = 44; d.LastUpdateTime = 1.5; d.LastScreenUpdate = 2.5; d.LastProgressUpdate = 3.5;
    d.DebugDisplay = true; d.Initialised = false; d.FullScreen = false; d.BlackScreen = false; d.DisplayText = false;
    d.DisplayResetProgressTipText = true; d.ForRelaunch = false; d.ForFrontEnd = false; d.ForFileInstaller = false;
    d.FileInstallerFinished = true; d.texCalls = 0; d.texRelaunch = d.texFrontEnd = d.texSawInitialised = false;
    d.renderCalls = 0; d.renderText[0] = 0; d.renderProgress = 0.0f; d.renderA = true; d.renderB = false;
}
static void ResetGlobals()
{
    g_bProgressDisplayEnabled = true; g_bProgressDisplaySuppressed = false; g_bAutoSaveProgressShown = false;
    g_AutoSaveProgressEndTime = -1.0; g_ProgressDebugDisplay = false; g_bCompileFrontendDefinitions = false; g_now = 10.0;
    g_wrapOption = g_wrapValue = -1; g_wrapCalls = 0; g_fontLoads = 0; g_fontName[0] = 0; g_pMain = 0;
}

int main()
{
    int bad = 0;
    Display d;
    Editor ed;

    // 1. disabled: nothing happens
    ResetGlobals(); Dirty(d); g_bProgressDisplayEnabled = false;
    d.StartProgress(true, true, true, true, true, true);
    if (d.Initialised || d.StageCount != 44 || d.texCalls || g_wrapCalls || g_fontLoads || d.renderCalls) bad |= 1;

    // 2. already initialised and not suppressed: nothing happens, not even the auto-save end time
    ResetGlobals(); Dirty(d); d.Initialised = true; g_bAutoSaveProgressShown = true;
    d.StartProgress(true, true, true, true, true, true);
    if (d.StageCount != 44 || d.texCalls || g_fontLoads || d.renderCalls || g_AutoSaveProgressEndTime != -1.0) bad |= 2;

    // 3. normal start: state reset, flags recorded, textures, word wrap, font, first frame
    ResetGlobals(); Dirty(d); g_bAutoSaveProgressShown = true;
    d.StartProgress(true, false, true, true, false, true);
    if (g_AutoSaveProgressEndTime != 13.0) bad |= 4;
    if (d.LastUpdateTime != 0.0 || d.LastScreenUpdate != 0.0 || d.LastProgressUpdate != 0.0 || d.LoadingScreenIndex != 0 ||
        d.TextTagIndex != -1 || d.TextTag[0] || d.LastTextDisplayed[0] || d.GuildSealFrameIndex != 0 || d.StageCount != 0) bad |= 8;
    if (d.DebugDisplay || !d.Initialised || !d.FullScreen || !d.BlackScreen || !d.DisplayText || d.DisplayResetProgressTipText ||
        d.ForRelaunch || !d.ForFrontEnd || d.ForFileInstaller || d.FileInstallerFinished) bad |= 16;
    if (d.texCalls != 1 || d.texRelaunch || !d.texFrontEnd || !d.texSawInitialised) bad |= 32;
    if (g_wrapCalls != 1 || g_wrapOption != 0 || g_wrapValue != 1) bad |= 64;
    if (g_fontLoads != 1 || std::strcmp(g_fontName, "ENG_ARIAL_16") != 0 || g_fontRefs != 1) bad |= 128;   // only PFont holds it
    if (d.renderCalls != 1 || std::strcmp(d.renderText, "Initialising progress display") != 0 || d.renderProgress != -1.0f ||
        d.renderA || !d.renderB) bad |= 256;

    // 4. suppressed display restarts even when initialised; no auto-save end time; debug display skips the first frame
    ResetGlobals(); Dirty(d); d.Initialised = true; g_bProgressDisplaySuppressed = true; g_bAutoSaveProgressShown = true;
    g_ProgressDebugDisplay = true;
    d.StartProgress(false, true, false, false, true, false);
    if (g_AutoSaveProgressEndTime != -1.0 || d.StageCount != 0 || !d.DebugDisplay || d.renderCalls != 0 || d.texCalls != 1 ||
        !d.texRelaunch || d.texFrontEnd || d.FullScreen || d.BlackScreen || !d.ForRelaunch || d.ForFrontEnd || !d.ForFileInstaller ||
        g_fontLoads != 1 || g_fontRefs != 1) bad |= 512;

    // 5. editor active forces the debug display; an inactive editor does not
    ResetGlobals(); Dirty(d); ed.active = true; g_pMain = &ed;
    d.StartProgress(true, false, false, false, false, false);
    if (!d.DebugDisplay || d.renderCalls != 0) bad |= 1024;
    ResetGlobals(); Dirty(d); ed.active = false; g_pMain = &ed;
    d.StartProgress(true, false, false, false, false, false);
    if (d.DebugDisplay || d.renderCalls != 1 || d.renderB) bad |= 2048;

    // 6. compiling the front-end definitions forces the debug display
    ResetGlobals(); Dirty(d); g_bCompileFrontendDefinitions = true;
    d.StartProgress(true, false, false, false, false, true);
    if (!d.DebugDisplay || d.renderCalls != 0) bad |= 4096;

    if (bad) { std::printf("00499aa0_TEST FAIL (%d)\n", bad); return 1; }
    std::printf("00499aa0_TEST PASS\n");
    return 0;
}