// Behaviour test for 0x00498490 CProgressDisplay::DrawRetailDisplay: the same algorithm over a recording renderer (every
// Draw2D* call is logged with its box / position, colour and texture or graphic), with the screen conversions modelled as
// GFAXToVX(x) = 2x, GFAYToVY(y) = y / 2 and pixel rounding as floor(v + 0.5). Five frames are checked against values worked out
// by hand: a loading frame at the half-way stage (picture wraps, full bar, tip overlay, guild seal texture, tip text and title),
// an auto-save frame (six backdrop tiles, scaled bar graphics, black screen, save text), the file installer finishing (restart,
// empty bar), a windowed region-change frame (seal box and graphic) and the "invalid save" text.
#include <cstdio>
#include <cstring>
#include <cmath>

struct C2DVector {
    float X, Y;
    C2DVector() : X(0.0f), Y(0.0f) { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator+(const C2DVector& v) const { return C2DVector(X + v.X, Y + v.Y); }
};
struct C2DExtentsF { float X, Y; C2DExtentsF(float x, float y) : X(x), Y(y) { } };
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF() : TLX(0.0f), TLY(0.0f), BRX(0.0f), BRY(0.0f) { }
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
    C2DBoxF(const C2DVector& tl, const C2DExtentsF& e) : TLX(tl.X), TLY(tl.Y), BRX(tl.X + e.X), BRY(tl.Y + e.Y) { }
    C2DBoxF(const C2DVector& tl, const C2DVector& br) : TLX(tl.X), TLY(tl.Y), BRX(br.X), BRY(br.Y) { }
};
struct Text {                                   // CCharString / CWideString stand-in (behaviour only)
    char s[96];
    Text() { s[0] = 0; }
    Text(const char* t) { std::strcpy(s, t); }
    long GetLength() const { return (long)std::strlen(s); }
    bool operator==(const char* t) const { return std::strcmp(s, t) == 0; }
    bool operator!=(const char* t) const { return std::strcmp(s, t) != 0; }
    Text operator+(const char* t) const { Text r(s); std::strcat(r.s, t); return r; }
    Text GetSubString(long start, long count) const { Text r; std::strncpy(r.s, s + start, count); r.s[count] = 0; return r; }
    bool EndsWith(const Text& suffix) const     // FableWin ?EndsWith@CCharString@@QBE_NABV1@@Z, as the row expands it
    {
        long offset = GetLength() - suffix.GetLength();
        if (offset >= 0)
            return std::strcmp(s + offset, suffix.s) == 0;
        return false;
    }
};

// ---- the engine around the row --------------------------------------------------------------------------------------------
static float GFAXToVX(float x) { return x * 2.0f; }
static float GFAYToVY(float y) { return y * 0.5f; }
static float GFRoundVXToNearestPixel(double x) { return (float)std::floor(x + 0.5); }
static float GFRoundVYToNearestPixel(double y) { return (float)std::floor(y + 0.5); }
static double g_Time = 12.5; static int g_TimeCalls = 0;
static double GFGetTime() { ++g_TimeCalls; return g_Time; }
static const double VIRTUAL_SCREEN_WIDTH = 640.0, VIRTUAL_SCREEN_HEIGHT = 480.0;
static long g_RTDX = 800, g_RTDY = 600;                        // CDisplayManager::GetRenderTargetDimensions
static bool g_bUseCompiledWorldFiles = true, g_bUseCompiledGlobalThings = true, g_WADBuildEnabled = false;
static bool g_bAutoSaveProgressShown = false, g_bSaveLoadFailed = false, g_bProgressDisplaySuppressed = false;
static double g_AutoSaveProgressEndTime = 0.0;
static const char* GResetProgressTipText = "reset tip";
static const char* GResetProgressTipTitleText = "reset title";
static bool g_PlayerGuiSetup = true;                           // CPlayerGui::IsSingletonSetup
static long g_RegionSealGraphics[2] = { 300, 301 }; static unsigned long g_nbRegionSealGraphics = 2;
static long g_Backdrop[6] = { 100, 101, 102, 103, 104, 105 };
enum { LEFT_GRAPHIC = 200, MID_GRAPHIC = 201, RIGHT_GRAPHIC = 202, INNER_GRAPHIC = 203 };
static bool g_HaveTextBank = true;
static Text Bank(const Text& symbol) { Text r("<"); std::strcat(r.s, symbol.s); std::strcat(r.s, ">"); return r; }
static void ConstructString(Text& text, const char* arg) { std::strcat(text.s, "{"); std::strcat(text.s, arg); std::strcat(text.s, "}"); }
static const char* AutoSaveDisplayName = "Slot 1";

struct Call { char kind; C2DBoxF box; long tex; long a, b; unsigned long colour; char text[96]; };
static Call g_Calls[32]; static int g_nbCalls = 0;
static unsigned long RGBA(unsigned long r, unsigned long g, unsigned long b, unsigned long a) { return (a << 24) | (r << 16) | (g << 8) | b; }
static void Draw2DTexture(const C2DBoxF& box, unsigned long colour, long texture, long flag)
{ Call& c = g_Calls[g_nbCalls++]; c.kind = 'T'; c.box = box; c.colour = colour; c.tex = texture; c.a = flag; c.b = 0; c.text[0] = 0; }
static void Draw2DScaledGraphic(const C2DBoxF& box, long graphic, long a, long b)
{ Call& c = g_Calls[g_nbCalls++]; c.kind = 'S'; c.box = box; c.colour = 0; c.tex = graphic; c.a = a; c.b = b; c.text[0] = 0; }
static void Draw2DBox(const C2DBoxF& box, unsigned long colour, long flag)
{ Call& c = g_Calls[g_nbCalls++]; c.kind = 'B'; c.box = box; c.colour = colour; c.tex = 0; c.a = flag; c.b = 0; c.text[0] = 0; }
static void Draw2DGraphic(const C2DVector& p, long graphic, long a, long b, unsigned long colour)
{ Call& c = g_Calls[g_nbCalls++]; c.kind = 'G'; c.box = C2DBoxF(p.X, p.Y, 0.0f, 0.0f); c.colour = colour; c.tex = graphic; c.a = a; c.b = b; c.text[0] = 0; }
static void DisplayCenteredText(const Text& text, const C2DBoxF& vbox)
{ Call& c = g_Calls[g_nbCalls++]; c.kind = 'X'; c.box = vbox; c.colour = 0; c.tex = 0; c.a = c.b = 0; std::strcpy(c.text, text.s); }

struct CProgressDisplayModel {
    bool FullScreen, BlackScreen, ForFrontEnd, FileInstallerFinished, ForFileInstaller, DisplayText, DisplayResetProgressTipText;
    long StageCount, LoadingScreenIndex, GuildSealFrameIndex;
    long LoadingScreenTextures[4]; unsigned long nbLoadingScreenTextures;
    long ProgressTextures[6]; unsigned long nbProgressTextures;
    long GuildSealTextures[3]; unsigned long nbGuildSealTextures;
    Text LastTextDisplayed, TextTag;
    double LastProgressUpdate;
    void DrawRetailDisplay(float progress, bool use_progress);
};

static long Limit(long v, long lo, long hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }   // GFLimitReturn<long>
static void Round(C2DBoxF& box)
{
    box.TLX = GFRoundVXToNearestPixel(box.TLX); box.TLY = GFRoundVYToNearestPixel(box.TLY);
    box.BRX = GFRoundVXToNearestPixel(box.BRX); box.BRY = GFRoundVYToNearestPixel(box.BRY);
}

void CProgressDisplayModel::DrawRetailDisplay(float progress, bool use_progress)
{
    const unsigned long white = RGBA(255, 255, 255, 255), black = RGBA(0, 0, 0, 255);
    if (FullScreen) {
        long total = 130;
        if (!g_bUseCompiledWorldFiles) total = 465;
        if (!g_bUseCompiledGlobalThings) total += 347;
        if (g_WADBuildEnabled) total += 1788;
        if (g_bAutoSaveProgressShown) total = 300;
        ++StageCount;
        if (g_bAutoSaveProgressShown)
            StageCount = (long)(300.0 - (g_AutoSaveProgressEndTime - GFGetTime()) * 100.0);
        bool restarted = false;
        float fraction = (float)Limit(StageCount, 0L, total) / total;
        if (use_progress) {
            fraction = progress;
            if (progress >= 1.0f) {
                fraction = 1.0f;
                if (!FileInstallerFinished) {
                    FileInstallerFinished = true; ForFileInstaller = false; StageCount = 0; fraction = 0.0f; restarted = true;
                }
            }
        }
        if (!g_bAutoSaveProgressShown && (StageCount == total / 2 || restarted)) {
            ++LoadingScreenIndex;
            if (LoadingScreenIndex == (long)nbLoadingScreenTextures) LoadingScreenIndex = 0;
        }
        if (g_bAutoSaveProgressShown) {
            if (g_PlayerGuiSetup) {
                float scaleX = g_RTDX * (1.0f / 640.0f), scaleY = g_RTDY * (1.0f / 480.0f);
                C2DBoxF box(GFAXToVX(0.0f), GFAYToVY(0.0f), GFAXToVX(256.0f) * scaleX, GFAYToVY(256.0f) * scaleY);
                Draw2DScaledGraphic(box, g_Backdrop[0], 0, 0);
                box = C2DBoxF(box.BRX, box.TLY, box.BRX + GFAXToVX(256.0f) * scaleX, GFAYToVY(256.0f) * scaleY);
                Draw2DScaledGraphic(box, g_Backdrop[1], 0, 0);
                box = C2DBoxF(box.BRX, box.TLY, box.BRX + GFAXToVX(128.0f) * scaleX, GFAYToVY(256.0f) * scaleY);
                Draw2DScaledGraphic(box, g_Backdrop[2], 0, 0);
                box = C2DBoxF(GFAXToVX(0.0f), box.BRY, GFAXToVX(256.0f) * scaleX, box.BRY + GFAYToVY(256.0f) * scaleY);
                Draw2DScaledGraphic(box, g_Backdrop[3], 0, 0);
                box = C2DBoxF(box.BRX, box.TLY, box.BRX + GFAXToVX(256.0f) * scaleX, box.TLY + GFAYToVY(256.0f) * scaleY);
                Draw2DScaledGraphic(box, g_Backdrop[4], 0, 0);
                box = C2DBoxF(box.BRX, box.TLY, box.BRX + GFAXToVX(128.0f) * scaleX, box.TLY + GFAYToVY(256.0f) * scaleY);
                Draw2DScaledGraphic(box, g_Backdrop[5], 0, 0);
            }
        } else
            Draw2DTexture(C2DBoxF(0.0f, 0.0f, (float)VIRTUAL_SCREEN_WIDTH, (float)VIRTUAL_SCREEN_HEIGHT), white,
                          LoadingScreenTextures[LoadingScreenIndex], 0);
        if ((nbProgressTextures != 0 || g_bAutoSaveProgressShown) && !g_bSaveLoadFailed) {
            float renderHeight = (float)g_RTDY;
            float barX = GFAXToVX((float)g_RTDX) * 0.5f - GFAXToVX(96.0f);
            C2DVector barPosition(barX, GFAYToVY(renderHeight + 380.0f - 480.0f));
            C2DBoxF box(barPosition, C2DExtentsF(GFAXToVX(64.0f), GFAYToVY(32.0f)));
            Round(box);
            if (g_bAutoSaveProgressShown) Draw2DScaledGraphic(box, LEFT_GRAPHIC, 0, 1); else Draw2DTexture(box, white, ProgressTextures[1], 1);
            C2DVector position(barPosition);
            position.X += GFAXToVX(64.0f);
            box = C2DBoxF(position, C2DExtentsF(GFAXToVX(64.0f), GFAYToVY(32.0f)));
            Round(box);
            if (g_bAutoSaveProgressShown) Draw2DScaledGraphic(box, MID_GRAPHIC, 0, 1); else Draw2DTexture(box, white, ProgressTextures[2], 1);
            position.X += GFAXToVX(64.0f);
            box = C2DBoxF(position, C2DExtentsF(GFAXToVX(64.0f), GFAYToVY(32.0f)));
            Round(box);
            if (g_bAutoSaveProgressShown) Draw2DScaledGraphic(box, RIGHT_GRAPHIC, 0, 1); else Draw2DTexture(box, white, ProgressTextures[3], 1);
            float fillWidth = GFAXToVX(146.0f) * fraction;
            if (fillWidth > 0.0f) {
                C2DVector fillPosition = barPosition + C2DVector(GFAXToVX(23.0f), GFAYToVY(5.0f));
                box = C2DBoxF(fillPosition, C2DExtentsF(fillWidth, GFAYToVY(16.0f)));
                Round(box);
                if (g_bAutoSaveProgressShown) Draw2DScaledGraphic(box, INNER_GRAPHIC, 0, 0); else Draw2DTexture(box, white, ProgressTextures[4], 1);
            }
            if (!g_bAutoSaveProgressShown) {
                position = C2DVector(GFAXToVX(g_RTDX * 0.5f - 129.0f), GFAYToVY(renderHeight + 315.0f - 480.0f));
                box = C2DBoxF(position, C2DExtentsF(GFAXToVX(256.0f), GFAYToVY(128.0f)));
                Round(box);
                Draw2DTexture(box, white, ProgressTextures[5], 1);
            }
        }
    }
    if (BlackScreen)
        Draw2DBox(C2DBoxF(0.0f, 0.0f, (float)VIRTUAL_SCREEN_WIDTH, (float)VIRTUAL_SCREEN_HEIGHT), black, 0);
    if (!g_bAutoSaveProgressShown && !g_bSaveLoadFailed && !ForFrontEnd) {
        float sealX = (GFAXToVX((float)g_RTDX) * 0.5f) - GFAXToVX(32.0f) - 4.0f;
        C2DVector sealPosition(sealX, GFAYToVY(g_RTDY + 310.0f - 480.0f));
        if (FullScreen) {
            if ((unsigned long)GuildSealFrameIndex < nbGuildSealTextures)
                Draw2DTexture(C2DBoxF(sealPosition, C2DExtentsF(GFAXToVX(64.0f), GFAYToVY(64.0f))), white, GuildSealTextures[GuildSealFrameIndex], 1);
            if (nbGuildSealTextures != 0) { ++GuildSealFrameIndex; GuildSealFrameIndex %= (long)nbGuildSealTextures; }
        } else if (g_PlayerGuiSetup) {
            if ((unsigned long)GuildSealFrameIndex < g_nbRegionSealGraphics) {
                float right = g_RTDX - 112.0f, bottom = g_RTDY + 380.0f - 480.0f;
                sealPosition = C2DVector(GFAXToVX(right), GFAYToVY(bottom));
                Draw2DBox(C2DBoxF(sealPosition, C2DExtentsF(GFAXToVX(64.0f), GFAYToVY(64.0f))), black, 0);
                sealPosition = C2DVector(GFAXToVX(right), GFAYToVY(bottom));
                Draw2DGraphic(sealPosition, g_RegionSealGraphics[GuildSealFrameIndex], 0, 1, 0xFFFFFFFFul);
            }
            if (g_nbRegionSealGraphics != 0) { ++GuildSealFrameIndex; GuildSealFrameIndex %= (long)g_nbRegionSealGraphics; }
        }
        if (LastTextDisplayed != "")
            GFGetTime();
        LastProgressUpdate = GFGetTime();
    }
    if (FullScreen) {
        Text text(GResetProgressTipText), title(GResetProgressTipTitleText);
        if (g_bAutoSaveProgressShown) { text = ""; title = ""; }
        if (g_HaveTextBank) {
            if (DisplayText) {
                if (!DisplayResetProgressTipText) {
                    text = Bank(TextTag);
                    Text tag(TextTag);
                    if (tag.EndsWith("0")) tag = tag.GetSubString(0, tag.GetLength() - 3);
                    if (tag.EndsWith("_PC")) tag = tag.GetSubString(0, tag.GetLength() - 3);
                    title = Bank(tag + "_TITLE");
                }
                if (g_bAutoSaveProgressShown) {
                    text = Bank("TEXT_GUI_SAVE_SAVING_EXTENDED_10"); ConstructString(text, AutoSaveDisplayName); title = "";
                }
            }
            if (g_bProgressDisplaySuppressed) {
                text = Text("A - ") + Bank("TEXT_GUI_INVENTORY_BUTTON_TEXT_EXIT").s;
                title = Bank("TEXT_GUI_MENU_INVALID_SAVE"); ConstructString(title, AutoSaveDisplayName);
            }
        }
        if (text != "") {
            float left = g_RTDX * 0.125f, scale = g_RTDY * (1.0f / 480.0f);
            C2DBoxF window(C2DVector(left, scale * 100.0f), C2DVector(g_RTDX - left, scale * 300.0f));
            C2DBoxF vbox = window;                     // GFABoxToVBox (identity here)
            Text formatted = text;                     // FormatTextForVWindow (no wrapping here)
            vbox.TLY -= GFAYToVY(10.0f); vbox.BRY -= GFAYToVY(10.0f);
            DisplayCenteredText(formatted, vbox);
            if (title != "") {
                float extentsY = 20.0f;                // PFont->GetVStringExtents(formatted).Y
                vbox.TLY -= extentsY; vbox.BRY -= extentsY;
                DisplayCenteredText(title, vbox);
            }
        }
    }
}

static int g_Fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); ++g_Fail; } }
// 1/640 and 1/480 are not exact in binary, so the scaled tiles and the text window land within float rounding of the hand values
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
static bool Box(const C2DBoxF& b, float tlx, float tly, float brx, float bry) { return Near(b.TLX, tlx) && Near(b.TLY, tly) && Near(b.BRX, brx) && Near(b.BRY, bry); }
static bool IsCall(int i, char kind, long tex, float tlx, float tly, float brx, float bry)
{ return i < g_nbCalls && g_Calls[i].kind == kind && g_Calls[i].tex == tex && Box(g_Calls[i].box, tlx, tly, brx, bry); }

static void Reset(CProgressDisplayModel& d)
{
    // every field set by hand: VC7.1 does not value-initialise `T()` for a class with constructed members
    d.FullScreen = true; d.BlackScreen = false; d.ForFrontEnd = false; d.FileInstallerFinished = false; d.ForFileInstaller = true;
    d.DisplayText = true; d.DisplayResetProgressTipText = false; d.StageCount = 0; d.LoadingScreenIndex = 0; d.GuildSealFrameIndex = 0;
    d.LastTextDisplayed = ""; d.TextTag = ""; d.LastProgressUpdate = 0.0;
    for (int i = 0; i < 4; ++i) d.LoadingScreenTextures[i] = 10 + i;
    for (int i = 0; i < 6; ++i) d.ProgressTextures[i] = 20 + i;
    for (int i = 0; i < 3; ++i) d.GuildSealTextures[i] = 30 + i;
    d.nbLoadingScreenTextures = 4; d.nbProgressTextures = 6; d.nbGuildSealTextures = 3;
    g_nbCalls = 0; g_TimeCalls = 0; g_bAutoSaveProgressShown = false; g_bProgressDisplaySuppressed = false; g_HaveTextBank = true;
}

int main()
{
    CProgressDisplayModel d;
    // 1. loading frame, half way (65 of 130): the picture index wraps 3 -> 0, the bar is half full, the seal frame wraps 2 -> 0
    Reset(d); d.StageCount = 64; d.LoadingScreenIndex = 3; d.GuildSealFrameIndex = 2; d.TextTag = "TEXT_LOADING_TIP_PC_10";
    d.DrawRetailDisplay(0.0f, false);
    Check(d.StageCount == 65 && d.LoadingScreenIndex == 0 && d.GuildSealFrameIndex == 0, "1 counters");
    Check(g_nbCalls == 9, "1 call count");
    Check(IsCall(0, 'T', 10, 0, 0, 640, 480), "1 loading picture");
    Check(IsCall(1, 'T', 21, 608, 250, 736, 266) && g_Calls[1].a == 1, "1 bar left");
    Check(IsCall(2, 'T', 22, 736, 250, 864, 266), "1 bar middle");
    Check(IsCall(3, 'T', 23, 864, 250, 992, 266), "1 bar right");
    Check(IsCall(4, 'T', 24, 654, 253, 800, 261), "1 bar fill (146 of 292)");
    Check(IsCall(5, 'T', 25, 542, 218, 1054, 282), "1 tip overlay");
    Check(IsCall(6, 'T', 32, 732, 215, 860, 247) && g_Calls[6].colour == 0xFFFFFFFFul, "1 guild seal (800 - 64 - 4, 215)");
    Check(g_Calls[7].kind == 'X' && std::strcmp(g_Calls[7].text, "<TEXT_LOADING_TIP_PC_10>") == 0 && Box(g_Calls[7].box, 100, 120, 700, 370), "1 tip text");
    Check(g_Calls[8].kind == 'X' && std::strcmp(g_Calls[8].text, "<TEXT_LOADING_TIP_TITLE>") == 0 && Box(g_Calls[8].box, 100, 100, 700, 350), "1 tip title");
    Check(d.LastProgressUpdate == 12.5 && g_TimeCalls == 1, "1 progress time");

    // 2. auto-save frame: stage from the end time (300 - 1.5 * 100 = 150 of 300), six tiles at 1.25 scale, black screen, save text
    Reset(d); g_bAutoSaveProgressShown = true; g_AutoSaveProgressEndTime = 14.0; d.BlackScreen = true; d.DisplayResetProgressTipText = true;
    d.LoadingScreenIndex = 1; d.LastProgressUpdate = 3.0;
    d.DrawRetailDisplay(0.0f, false);
    Check(d.StageCount == 150 && d.LoadingScreenIndex == 1, "2 counters");
    Check(g_nbCalls == 12, "2 call count");
    Check(IsCall(0, 'S', 100, 0, 0, 640, 160) && IsCall(1, 'S', 101, 640, 0, 1280, 160) && IsCall(2, 'S', 102, 1280, 0, 1600, 160), "2 top tiles");
    Check(IsCall(3, 'S', 103, 0, 160, 640, 320) && IsCall(4, 'S', 104, 640, 160, 1280, 320) && IsCall(5, 'S', 105, 1280, 160, 1600, 320), "2 bottom tiles");
    Check(IsCall(6, 'S', LEFT_GRAPHIC, 608, 250, 736, 266) && g_Calls[6].b == 1 && IsCall(8, 'S', RIGHT_GRAPHIC, 864, 250, 992, 266), "2 bar graphics");
    Check(IsCall(9, 'S', INNER_GRAPHIC, 654, 253, 800, 261) && g_Calls[9].b == 0, "2 bar fill graphic");
    Check(IsCall(10, 'B', 0, 0, 0, 640, 480) && g_Calls[10].colour == 0xFF000000ul, "2 black screen");
    Check(g_Calls[11].kind == 'X' && std::strcmp(g_Calls[11].text, "<TEXT_GUI_SAVE_SAVING_EXTENDED_10>{Slot 1}") == 0, "2 save text, no title");
    Check(d.LastProgressUpdate == 3.0, "2 no seal while auto-saving");

    // 3. the file installer finishes: restart (stage 0, empty bar, next picture), no seal for the front end, no text bank
    Reset(d); d.StageCount = 40; d.LoadingScreenIndex = 1; d.ForFrontEnd = true; g_HaveTextBank = false;
    d.DrawRetailDisplay(1.5f, true);
    Check(d.FileInstallerFinished && !d.ForFileInstaller && d.StageCount == 0 && d.LoadingScreenIndex == 2, "3 restart");
    Check(g_nbCalls == 7 && IsCall(0, 'T', 12, 0, 0, 640, 480) && IsCall(4, 'T', 25, 542, 218, 1054, 282), "3 no fill drawn");
    Check(std::strcmp(g_Calls[5].text, "reset tip") == 0 && std::strcmp(g_Calls[6].text, "reset title") == 0, "3 reset tip text");
    Check(g_TimeCalls == 0, "3 no seal for the front end");

    // 4. windowed frame: black screen, region-change seal box and graphic at (2 x (800 - 112), (600 - 100) / 2), frame 1 -> 0
    Reset(d); d.FullScreen = false; d.BlackScreen = true; d.GuildSealFrameIndex = 1; d.StageCount = 7; d.LastTextDisplayed = "x";
    d.DrawRetailDisplay(0.0f, false);
    Check(d.StageCount == 7 && d.GuildSealFrameIndex == 0 && g_nbCalls == 3, "4 counters");
    Check(IsCall(1, 'B', 0, 1376, 250, 1504, 282) && g_Calls[1].colour == 0xFF000000ul, "4 seal backing box");
    Check(g_Calls[2].kind == 'G' && g_Calls[2].tex == 301 && Near(g_Calls[2].box.TLX, 1376) && Near(g_Calls[2].box.TLY, 250) && g_Calls[2].b == 1, "4 seal graphic");
    Check(g_TimeCalls == 2 && d.LastProgressUpdate == 12.5, "4 times");

    // 5. "invalid save" text replaces the tip
    Reset(d); g_bProgressDisplaySuppressed = true; d.StageCount = 1; d.TextTag = "TEXT_TIP_3";
    d.DrawRetailDisplay(0.0f, false);
    Check(std::strcmp(g_Calls[g_nbCalls - 2].text, "A - <TEXT_GUI_INVENTORY_BUTTON_TEXT_EXIT>") == 0, "5 invalid save text");
    Check(std::strcmp(g_Calls[g_nbCalls - 1].text, "<TEXT_GUI_MENU_INVALID_SAVE>{Slot 1}") == 0, "5 invalid save title");

    if (g_Fail == 0) std::printf("00498490_TEST PASS\n");
    return g_Fail;
}