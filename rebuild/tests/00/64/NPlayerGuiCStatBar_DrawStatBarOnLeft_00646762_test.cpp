// Behaviour test for 0x00646762 NPlayerGui::CStatBar::DrawStatBarOnLeft (links the REAL row object: its expanded GFGetInterpolatedValue /
// GFFloatToLongFloor / C2DVector operators / CFontBank::GetVStringWidth, its COMDAT copies of GFIntToWideString and
// CFontBank::GetVStringExtents, the STLport list walk and its same-TU GFAnchorToBottomLeft body run as compiled).
// Fixtures (raw native layout, every offset read by an instruction of the row): CStatBar vptr +0 (DrawStatBox = slot 2, 0x00646940),
// GuiDef +0x04, OldValue +0x10, GuiMode +0x28, FadeInTimer +0x2C, GraphicIndex +0x50, BorderGraphicIndex +0x54, EffectIndex +0x58,
// FontBank +0x60, ChangeTexts +0x64 (STLport list: sentinel node pointer; node = next, prev, CChangeText at +8: OldOffset +0, Offset
// +8, Text +0x10, Value +0x14, Colour +0x18); CPlayerGuiDef StatBoxGoldGainEffect +0x2EC, StatBoxGoldLossEffect +0x2F0, MoneyWidth
// +0x4BC, StatUpdateMoralityGoodGraphic +0x530, ...EvilGraphic +0x534, ...GoodEffect +0x538, ...EvilEffect +0x53C, StatUpdateGraphics
// +0x54C and StatUpdateEffects +0x558 (vector<long> begin words; RENOWN = 4, MORALITY = 5); CEnginePrimitivePrepareInfo WFInterpolate
// +0x04; CRGBColour bytes B, G, R, A; the CPlayerGui singleton pointer; CFontBank vptr (GetStringExtents = slot 1).
// The out-of-line engine callees are provided here and log every call (receiver, arguments); CWideString is a counted heap string.
// Reference model (independent): see Model() below.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <wchar.h>

typedef unsigned short WCH;
static int g_liveStrings;
class CWideString { public:
    WCH* P;
    CWideString(); CWideString(const CWideString& other); ~CWideString();
    const CWideString& operator=(const CWideString& other);
    const WCH* GetAsCharArray() const;
    void __cdecl Sprintf(const WCH* format, ...);
};
static WCH* Dup(const WCH* s) { size_t n = wcslen((const wchar_t*)s) + 1; WCH* p = (WCH*)malloc(n * sizeof(WCH)); memcpy(p, s, n * sizeof(WCH)); ++g_liveStrings; return p; }
static void Drop(WCH* p) { if (p) { free(p); --g_liveStrings; } }
static const WCH kEmpty[1] = { 0 };
CWideString::CWideString() : P(0) { }
CWideString::CWideString(const CWideString& other) : P(other.P ? Dup(other.P) : 0) { }
CWideString::~CWideString() { Drop(P); P = 0; }
const CWideString& CWideString::operator=(const CWideString& other) { if (this != &other) { WCH* n = other.P ? Dup(other.P) : 0; Drop(P); P = n; } return *this; }
const WCH* CWideString::GetAsCharArray() const { return P ? P : kEmpty; }
void __cdecl CWideString::Sprintf(const WCH* format, ...)
{
    wchar_t buf[64]; va_list va; va_start(va, format); _vsnwprintf(buf, 63, (const wchar_t*)format, va); va_end(va); buf[63] = 0;
    Drop(P); P = Dup((const WCH*)buf);
}
CWideString __fastcall operator+(const WCH* left, const CWideString& right)
{
    wchar_t buf[128]; buf[0] = 0; wcscat(buf, (const wchar_t*)left); wcscat(buf, (const wchar_t*)right.GetAsCharArray());
    CWideString out; out.P = Dup((const WCH*)buf); return out;
}

class C2DVector { public: float X, Y; };
class C2DExtentsI { public: long X, Y; C2DExtentsI() { } C2DExtentsI(long x, long y) : X(x), Y(y) { } C2DExtentsI(const C2DExtentsI& p) : X(p.X), Y(p.Y) { } };
class C2DExtentsF { public: float X, Y; C2DExtentsF() { } C2DExtentsF(float x, float y) : X(x), Y(y) { } C2DExtentsF(const C2DExtentsF& p) : X(p.X), Y(p.Y) { } };
class CRGBColour { public: unsigned char B, G, R, A; };
class CEnginePrimitivePrepareInfo;
class CGraphicDataBank;
class CFontBank;
enum EEngine2DLayer { ENGINE_2D_LAYER_PROBE = 0x7FFFFFFF };
extern const float DELTA_NOTIONAL_ZERO = 1e-4f;

// ---- event log
struct Ev { char t; const void* p0; const void* p1; const void* p2; long i[6]; float f[4]; WCH s[40]; };
static Ev g_log[64], g_exp[64]; static int g_n, g_en; static bool g_model;
static Ev& Log(char t)
{
    static Ev junk; Ev* a = g_model ? g_exp : g_log; int& n = g_model ? g_en : g_n;
    if (n >= 64) { memset(&junk, 0, sizeof junk); return junk; }
    Ev& e = a[n++]; memset(&e, 0, sizeof e); e.t = t; return e;
}
static void SetS(Ev& e, const WCH* s) { size_t n = wcslen((const wchar_t*)s); if (n > 39) n = 39; memcpy(e.s, s, n * sizeof(WCH)); e.s[n] = 0; }

// ---- provided callees (the same value functions serve the row and the model)
static long g_dimX, g_dimY;
static void VCoord(float ax, float ay, float& vx, float& vy) { vx = ax * 0.5f + 3.0f; vy = ay * 0.25f - 1.0f; }
static float AXToVX(float x) { return x * 0.5f; }
static float AYToVY(float y) { return y * 0.25f; }
static void Extents(const WCH* s, float& x, float& y) { x = (float)(wcslen((const wchar_t*)s) * 8 + 2); y = 16.0f; }

class CDisplayManager { public: const C2DExtentsI GetRenderTargetDimensions(); };
static CDisplayManager g_display;
CDisplayManager* GPDisplayManager = &g_display;
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions() { Ev& e = Log('R'); e.p0 = this; return C2DExtentsI(g_dimX, g_dimY); }
void __fastcall GFACoordToVCoord(const C2DVector& acoord, C2DVector& vcoord)
{
    Ev& e = Log('C'); e.f[0] = acoord.X; e.f[1] = acoord.Y; VCoord(acoord.X, acoord.Y, vcoord.X, vcoord.Y);
}
float __fastcall GFAXToVX(float x) { Ev& e = Log('X'); e.f[0] = x; return AXToVX(x); }
float __fastcall GFAYToVY(float y) { Ev& e = Log('Y'); e.f[0] = y; return AYToVY(y); }

struct FakeFont {
    virtual void Slot0() { printf("FAIL font slot 0\n"); exit(3); }
    virtual C2DExtentsF GetStringExtents(const WCH* text, bool flag) const
    {
        Ev& e = Log('E'); e.p0 = this; e.i[0] = flag; SetS(e, text); float x, y; Extents(text, x, y); return C2DExtentsF(x, y);
    }
};

class CPlayerGui { public:
    void Draw2DText(const C2DVector& pos, const CWideString& text, const CFontBank& font, const CRGBColour& colour, float scale, EEngine2DLayer layer, bool flag);
};
void CPlayerGui::Draw2DText(const C2DVector& pos, const CWideString& text, const CFontBank& font, const CRGBColour& colour, float scale, EEngine2DLayer layer, bool flag)
{
    Ev& e = Log('T'); e.p0 = this; e.p1 = &font; e.f[0] = pos.X; e.f[1] = pos.Y; e.f[2] = scale; e.i[0] = colour.B; e.i[1] = colour.G; e.i[2] = colour.R;
    e.i[3] = colour.A; e.i[4] = (long)layer; e.i[5] = flag; SetS(e, text.GetAsCharArray());
}
template<class T> class CCountedPointer { public: T* Data; void* Info; };
template<class T> class CTBaseSingletonNoDefaultAlloc {
    static CCountedPointer<T> PClass;
public:
    static void TestSet(T* p) { PClass.Data = p; PClass.Info = 0; }
};
template<> CCountedPointer<CPlayerGui> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass = CCountedPointer<CPlayerGui>();
static CPlayerGui g_gui;

namespace NPlayerGui { class CStatBar { public:
    virtual void DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& graphic_bank, const C2DVector& bar_pos,
                                   const C2DVector& text_offset, const CRGBColour& colour, unsigned char alpha, bool mirror);
protected:
    void DrawChangeText(const C2DVector& pos, const CWideString& text, const CRGBColour& colour);
}; }
void NPlayerGui::CStatBar::DrawChangeText(const C2DVector& pos, const CWideString& text, const CRGBColour& colour)
{
    Ev& e = Log('D'); e.p0 = this; e.p1 = &text; e.p2 = &colour; e.f[0] = pos.X; e.f[1] = pos.Y;
}

// ---- raw objects
struct Node { Node* next; Node* prev; C2DVector OldOffset, Offset; CWideString Text; long Value; CRGBColour Colour; unsigned char StartToEnd; unsigned char pad[3]; float PercentageThrough; };
struct Def { unsigned char b[0x570]; };
struct Info { unsigned long w0; float WFInterpolate; };
struct FakeBar {
    virtual void Slot0() { printf("FAIL bar slot 0\n"); exit(3); }
    virtual void Slot1() { printf("FAIL bar slot 1\n"); exit(3); }
    virtual void DrawStatBox(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, long graphic, long border, long effect,
                             const CWideString& text, unsigned char alpha, bool mirror, bool flag)
    {
        Ev& e = Log('B'); e.p0 = this; e.p1 = &info; e.p2 = &bank; e.f[0] = pos.X; e.f[1] = pos.Y; e.i[0] = graphic; e.i[1] = border; e.i[2] = effect;
        e.i[3] = alpha; e.i[4] = mirror; e.i[5] = flag; SetS(e, text.GetAsCharArray());
    }
    Def* GuiDef;                 // +0x04
    unsigned char p08[8];
    float OldValue;              // +0x10
    unsigned char p14[0x14];
    long GuiMode;                // +0x28
    float FadeInTimer;           // +0x2C
    unsigned char p30[0x20];
    long GraphicIndex;           // +0x50
    long BorderGraphicIndex;     // +0x54
    long EffectIndex;            // +0x58
    unsigned char p5c[4];
    FakeFont* FontBank;          // +0x60
    Node* ChangeTexts;           // +0x64
};
template<class T> static T& At(void* base, int off) { return *(T*)((unsigned char*)base + off); }

enum { GAIN = 7, LOSS = 9, G_RENOWN = 21, G_MORALITY = 22, G_GOOD = 31, G_EVIL = 32, E_RENOWN = 41, E_GOOD = 51, E_EVIL = 52 };
static long g_graphics[6] = { 11, 12, 13, 14, G_RENOWN, G_MORALITY };
static long g_effects[6] = { 61, 62, 63, 64, E_RENOWN, 66 };
static Def g_def; static FakeFont g_font; static FakeBar g_bar; static Info g_info; static unsigned char g_bank[4];
static Node g_sentinel; static Node g_nodes[3];

static void IntText(long v, WCH* out) { wchar_t b[32]; _snwprintf(b, 31, L"%d", v); b[31] = 0; wcscpy((wchar_t*)out, b); }
static long Floor(float x) { return (long)floor(x); }

// Reference model: produces the expected event log and the expected EffectIndex.
static long Model(const C2DVector& bar_pos, const C2DVector& text_offset, const CRGBColour& colour, unsigned char alpha, bool mirror, int count)
{
    g_model = true; g_en = 0;
    float money = At<float>(&g_def, 0x4BC);
    float start = -money;
    float lerped = (bar_pos.X - start) * g_bar.FadeInTimer + start;
    float tlx = (float)Floor(lerped), tly;
    { Ev& e = Log('R'); e.p0 = &g_display; tly = (float)g_dimY + bar_pos.Y - 480.0f; }
    float textx = tlx + text_offset.X, texty = tly + text_offset.Y;
    WCH text[64]; IntText((long)g_bar.OldValue, text);
    long member = g_bar.EffectIndex, effect = member;
    if (effect == GAIN)
    {
        effect = 0;
        if (count > 0)
        {
            if (g_nodes[0].Value > 0) effect = GAIN;
            else if (g_nodes[0].Value < 0) effect = LOSS;
        }
    }
    else
    {
        if (g_bar.FadeInTimer >= 1.0f)
        {
            if (member == 0)
            {
                if (g_bar.GraphicIndex == G_RENOWN) member = E_RENOWN;
                else if (g_bar.GraphicIndex == G_GOOD) member = E_GOOD;
                else if (g_bar.GraphicIndex == G_EVIL) member = E_EVIL;
                effect = member;
            }
        }
        else if (g_bar.FadeInTimer < 1e-4f)
        {
            member = 0; effect = 0;
        }
        if (g_bar.GraphicIndex == G_MORALITY)
        {
            WCH t2[64]; t2[0] = '+'; IntText((long)fabs(g_bar.OldValue), t2 + 1); memcpy(text, t2, sizeof t2);
        }
        if (g_bar.OldValue > 0.0f)
        {
            WCH t2[64]; t2[0] = '+'; wcscpy((wchar_t*)(t2 + 1), (const wchar_t*)text); memcpy(text, t2, sizeof t2);
        }
    }
    { Ev& e = Log('B'); e.p0 = &g_bar; e.p1 = &g_info; e.p2 = g_bank; e.f[0] = tlx; e.f[1] = tly; e.i[0] = g_bar.GraphicIndex; e.i[1] = g_bar.BorderGraphicIndex;
      e.i[2] = effect; e.i[3] = alpha; e.i[4] = mirror; e.i[5] = 0; SetS(e, text); }
    float vx, vy;
    { Ev& e = Log('C'); e.f[0] = textx; e.f[1] = texty; VCoord(textx, texty, vx, vy); }
    if (mirror)
    {
        float ex, ey; Extents(text, ex, ey);
        { Ev& e = Log('E'); e.p0 = &g_font; e.i[0] = 0; SetS(e, text); }
        { Ev& e = Log('X'); e.f[0] = ex; }
        { Ev& e = Log('Y'); e.f[0] = ey; }
        float width = AXToVX(ex);
        { Ev& e = Log('R'); e.p0 = &g_display; }
        { Ev& e = Log('X'); e.f[0] = (float)g_dimX; }
        vx = AXToVX((float)g_dimX) - vx - width;
    }
    { Ev& e = Log('T'); e.p0 = &g_gui; e.p1 = &g_font; e.f[0] = vx; e.f[1] = vy; e.f[2] = 1.0f; e.i[0] = colour.B; e.i[1] = colour.G; e.i[2] = colour.R; e.i[3] = alpha;
      e.i[4] = 0x43; e.i[5] = 0; SetS(e, text); }
    if (g_bar.GuiMode == 0 || g_bar.GuiMode == 3)
    {
        float t = g_info.WFInterpolate;
        for (int k = 0; k < count; ++k)
        {
            const Node& n = g_nodes[k];
            float ox = (n.Offset.X - n.OldOffset.X) * t + n.OldOffset.X;
            float oy = (n.Offset.Y - n.OldOffset.Y) * t + n.OldOffset.Y;
            float cx, cy;
            { Ev& e = Log('C'); e.f[0] = ox + tlx; e.f[1] = oy + tly; VCoord(ox + tlx, oy + tly, cx, cy); }
            { Ev& e = Log('D'); e.p0 = &g_bar; e.p1 = &n.Text; e.p2 = &n.Colour; e.f[0] = cx; e.f[1] = cy; }
        }
    }
    g_model = false;
    return member;
}

static int g_cases, g_fail;
static void Fail(const char* what, int k)
{
    if (g_fail++ < 10) printf("FAIL case %d: %s (event %d: got '%c' want '%c')\n", g_cases, what, k, k < g_n ? g_log[k].t : '-', k < g_en ? g_exp[k].t : '-');
}
static void Run(const C2DVector& bar_pos, const C2DVector& text_offset, const CRGBColour& colour, unsigned char alpha, bool mirror, int count)
{
    ++g_cases;
    g_sentinel.next = count ? &g_nodes[0] : &g_sentinel;
    for (int k = 0; k < count; ++k) g_nodes[k].next = (k + 1 < count) ? &g_nodes[k + 1] : &g_sentinel;
    long want = Model(bar_pos, text_offset, colour, alpha, mirror, count);
    int live = g_liveStrings; g_n = 0;
    C2DVector bp = bar_pos, to = text_offset; CRGBColour c = colour;
    ((NPlayerGui::CStatBar*)&g_bar)->NPlayerGui::CStatBar::DrawStatBarOnLeft(*(const CEnginePrimitivePrepareInfo*)&g_info, *(const CGraphicDataBank*)g_bank, bp, to, c, alpha, mirror);
    if (g_liveStrings != live) Fail("string leak", 0);
    if (memcmp(&bp, &bar_pos, sizeof bp) || memcmp(&to, &text_offset, sizeof to) || memcmp(&c, &colour, sizeof c)) Fail("input written", 0);
    if (g_bar.EffectIndex != want) Fail("EffectIndex", 0);
    if (g_n != g_en) { Fail("event count", g_n < g_en ? g_n : g_en); return; }
    for (int k = 0; k < g_n; ++k) if (memcmp(&g_log[k], &g_exp[k], sizeof(Ev))) { Fail("event differs", k); return; }
}

int main()
{
    if ((char*)&g_bar.GuiDef - (char*)&g_bar != 4 || (char*)&g_bar.OldValue - (char*)&g_bar != 0x10 || (char*)&g_bar.GuiMode - (char*)&g_bar != 0x28 ||
        (char*)&g_bar.FadeInTimer - (char*)&g_bar != 0x2C || (char*)&g_bar.GraphicIndex - (char*)&g_bar != 0x50 || (char*)&g_bar.EffectIndex - (char*)&g_bar != 0x58 ||
        (char*)&g_bar.FontBank - (char*)&g_bar != 0x60 || (char*)&g_bar.ChangeTexts - (char*)&g_bar != 0x64 || sizeof(Node) != 8 + 36 ||
        (char*)&g_nodes[0].Text - (char*)&g_nodes[0] != 0x18 || (char*)&g_nodes[0].Value - (char*)&g_nodes[0] != 0x1C || (char*)&g_nodes[0].Colour - (char*)&g_nodes[0] != 0x20)
    { printf("FAIL fixture layout\n"); return 2; }
    memset(&g_def, 0xEE, sizeof g_def);
    At<long>(&g_def, 0x2EC) = GAIN; At<long>(&g_def, 0x2F0) = LOSS; At<float>(&g_def, 0x4BC) = 64.0f;
    At<long>(&g_def, 0x530) = G_GOOD; At<long>(&g_def, 0x534) = G_EVIL; At<long>(&g_def, 0x538) = E_GOOD; At<long>(&g_def, 0x53C) = E_EVIL;
    At<long*>(&g_def, 0x54C) = g_graphics; At<long*>(&g_def, 0x558) = g_effects;
    g_bar.GuiDef = &g_def; g_bar.FontBank = &g_font; g_bar.ChangeTexts = &g_sentinel; g_bar.BorderGraphicIndex = 77;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::TestSet(&g_gui);
    static const WCH kT0[] = { 'a', 0 }, kT1[] = { 'b', 'c', 0 };
    g_nodes[0].Text.P = Dup(kT0); g_nodes[1].Text.P = Dup(kT1);
    g_nodes[0].OldOffset.X = 4.0f; g_nodes[0].OldOffset.Y = -8.0f; g_nodes[0].Offset.X = 20.0f; g_nodes[0].Offset.Y = 40.0f;
    g_nodes[1].OldOffset.X = -16.5f; g_nodes[1].OldOffset.Y = 2.25f; g_nodes[1].Offset.X = 3.5f; g_nodes[1].Offset.Y = -30.0f;
    g_nodes[2].OldOffset.X = 100.0f; g_nodes[2].OldOffset.Y = 50.0f; g_nodes[2].Offset.X = 60.0f; g_nodes[2].Offset.Y = 90.0f;
    g_nodes[1].Value = 5; g_nodes[2].Value = -5;
    g_nodes[0].Colour.B = 1; g_nodes[1].Colour.G = 2; g_nodes[2].Colour.R = 3;

    static const float kFade[] = { 0.0f, 9.5367431640625e-7f, 0.00390625f, 0.25f, 0.5f, 1.0f, 1.5f };
    static const long kEffect[] = { 0, GAIN, LOSS, 5 };
    static const long kGraphic[] = { G_RENOWN, G_MORALITY, G_GOOD, G_EVIL, 1 };
    static const float kValue[] = { -37.75f, 0.0f, 0.5f, 12.5f, 250.0f };
    static const long kFirst[] = { 1, -1, 0 };   // audit: the sign boundaries (kills `> 1` / `< -1` tests)
    static const float kInterp[] = { 0.0f, 0.25f, 1.0f };
    int seed = 0;
    for (int fi = 0; fi < 7; ++fi) for (int ei = 0; ei < 4; ++ei) for (int gi = 0; gi < 5; ++gi) for (int vi = 0; vi < 5; ++vi)
    for (int count = 0; count < 4; ++count) for (int mode = 0; mode < 4; ++mode)
    {
        ++seed;
        g_bar.FadeInTimer = kFade[fi]; g_bar.EffectIndex = kEffect[ei]; g_bar.GraphicIndex = kGraphic[gi]; g_bar.OldValue = kValue[vi]; g_bar.GuiMode = mode;
        g_nodes[0].Value = kFirst[seed % 3]; g_info.WFInterpolate = kInterp[(seed / 3) % 3];
        g_dimX = (seed & 1) ? 1024 : 800; g_dimY = (seed & 2) ? 768 : 600;
        C2DVector bar_pos; bar_pos.X = (seed % 5) * 10.25f - 6.0f; bar_pos.Y = (seed % 7) * 12.5f + 300.0f;
        C2DVector text_offset; text_offset.X = (seed % 4) * 2.5f + 1.0f; text_offset.Y = (seed % 3) * -3.25f + 9.0f;
        CRGBColour colour; colour.B = (unsigned char)(seed * 7); colour.G = (unsigned char)(seed * 13 + 1); colour.R = (unsigned char)(seed * 29 + 2); colour.A = 0x99;
        Run(bar_pos, text_offset, colour, (unsigned char)(seed * 3 + 5), (seed % 2) != 0, count);
        Run(bar_pos, text_offset, colour, (unsigned char)(seed * 3 + 5), (seed % 2) == 0, count);
    }
    if (g_fail) { printf("00646762_TEST FAIL (%d of %d)\n", g_fail, g_cases); return 1; }
    printf("00646762_TEST PASS (%d cases)\n", g_cases);
    return 0;
}