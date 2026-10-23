// Behaviour test for 0048f230: src/0048f230.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CSubtitleRenderer::DrawTextEmbeddedGraphics(const CEnginePrimitivePrepareInfo&, const C2DBoxF&)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CSubtitleRenderer::DrawTextEmbeddedGraphics(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box) -- for each embedded controller graphic already reached by the text reveal, add its sprite (layer 0x45) at the box corner plus the graphic's and the GUI def's offsets, and draw the graphic's caption (if any) beside it in white at 0.75 scale on layer 0x43
// Leaf: DrawTextEmbeddedGraphics
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048F230, 474 bytes, `ret 8` (retail mangling
//   ?DrawTextEmbeddedGraphics@CSubtitleRenderer@@AAEXABVCEnginePrimitivePrepareInfo@@ABVC2DBoxF@@@Z).
// Callees: PeekGuiDef 0x00438570 (__fastcall), GFVYToAY 0x009E1E40 / GFVXToAX 0x009E1E10 (__stdcall), CEnginePrimitive2DSprite
//   ctor 0x00493AC2 (13 arguments), the renderer's AddPrimitive (slot 23, +0x5C), CSubtitleRenderer::GetTextFromEmbeddedTagGraphic
//   0x0048DF77 and GetTextOffsetFromEmbeddedTagGraphic 0x0048DC3C (both landed), CWideString::operator!=(const wchar_t*)
//   0x0099B150 (L"" 0x0122D70C) / ~CWideString 0x0099B510, GFACoordToVCoord 0x009E21D0 (__fastcall), CGameUI::Draw2DText
//   0x00438010 (g_pGameUI 0x013B8790); 0.75f 0x01238174.
// Layout: the retail CSubtitleRenderer view of the landed neighbours (TextCharactersShowing +0x24, PFont +0x80,
//   EmbeddedPrimitiveHandles +0xAC, EmbeddedGraphics +0xB8 of 16-byte CEmbeddedGraphic, AllCharactersDisplayed +0xDD);
//   CPlayerGuiDef ControllerGraphicOffset +0x694 (PDB +0x6D0, retail 0x3C lower).
// Shape: the size TU of its neighbours (`y` off + `s` on: frame pointer, leave); the sprite position is
//   `Offset + def offset + C2DVector(GFVXToAX, GFVYToAY)` (the right operand's calls run first, Y before X); the caption
//   position is a named sum passed by address.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                          // 0x0099B510
    bool operator!=(const unsigned short* text) const;             // 0x0099B150
};

struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CFontBank;

float __stdcall GFVXToAX_009e1e10(float x);                           // 0x009E1E10
float __stdcall GFVYToAY(float y);                           // 0x009E1E40
void __fastcall GFACoordToVCoord(const C2DVector* in, C2DVector* out);   // 0x009E21D0

struct CEnginePrimitiveHandle { unsigned long Data[2]; };
class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& pos, long layer, unsigned long graphic, unsigned long b, long blend,
                             long interpolation, float scaleX, float scaleY, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long e, const C2DVector& uv0, const C2DVector& uv1);   // 0x00493AC2
};
class CEnginePrimitiveRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 23 (+0x5C)
};
class CEnginePrimitivePrepareInfo {
public:
    unsigned char _pad00[0x34];
    CEnginePrimitiveRenderer* Renderer;                      // +0x34
};
struct CPlayerGuiDef {
    unsigned char _pad000[0x694];
    C2DVector     ControllerGraphicOffset;                   // +0x694 (PDB +0x6D0)
};
const CPlayerGuiDef* __fastcall PeekGuiDef();                // 0x00438570

enum EEngine2DLayer { ENGINE_2D_LAYER_SUBTITLE = 0x43 };
struct CGameUI {
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag);   // 0x00438010
};
extern CGameUI* g_pGameUI;                                   // 0x013B8790

struct CEmbeddedGraphic {                                    // CSubtitleRenderer::CEmbeddedGraphic
    long      Graphic;                                       // +0x00
    long      CharIndex;                                     // +0x04
    C2DVector Offset;                                        // +0x08
};
template <class T> struct CRetailVector {                    // VC6 vector: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    bool empty() const { return m_pFirst == m_pLast; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long i) { return m_pFirst[i]; }
};

class CSubtitleRenderer {
public:
    void*              __vftable;                            // +0x00
    C2DBoxF            VWindow;                              // +0x04
    C2DBoxF            VTelevisionWindow;                    // +0x14
    long               TextCharactersShowing;                // +0x24
    unsigned char      _pad28[0x80 - 0x28];
    CFontBank*         PFont;                                // +0x80
    unsigned char      _pad84[0xac - 0x84];
    CRetailVector<CEnginePrimitiveHandle> EmbeddedPrimitiveHandles;   // +0xAC
    CRetailVector<CEmbeddedGraphic>       EmbeddedGraphics;           // +0xB8
    unsigned char      _padc4[0xdd - 0xc4];
    bool               AllCharactersDisplayed;               // +0xDD
    CWideString GetTextFromEmbeddedTagGraphic(long graphic) const;            // 0x0048DF77
    const C2DVector& GetTextOffsetFromEmbeddedTagGraphic(long graphic) const;  // 0x0048DC3C
    void DrawTextEmbeddedGraphics(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box);
};

void CSubtitleRenderer::DrawTextEmbeddedGraphics(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box)
{
    if (EmbeddedGraphics.empty())
        return;
    const CPlayerGuiDef* def = PeekGuiDef();
    for (unsigned long i = 0; i < EmbeddedGraphics.size(); ++i) {
        if (AllCharactersDisplayed || TextCharactersShowing > EmbeddedGraphics[i].CharIndex) {
            C2DVector pos = EmbeddedGraphics[i].Offset + def->ControllerGraphicOffset + C2DVector(GFVXToAX_009e1e10(box.TLX), GFVYToAY(box.TLY));
            CEnginePrimitive2DSprite sprite(0, pos, 0x45, EmbeddedGraphics[i].Graphic, 0, 2, 0, 0.0f, 0.0f, 0xff, 0xff, 0xff, 0xff, 0,
                                            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Renderer->AddPrimitive(&EmbeddedPrimitiveHandles[i], sprite, 0xc0);
            CWideString text = GetTextFromEmbeddedTagGraphic(EmbeddedGraphics[i].Graphic);
            if (text != L"") {
                C2DVector textPos;
                C2DVector at = pos + GetTextOffsetFromEmbeddedTagGraphic(EmbeddedGraphics[i].Graphic);
                GFACoordToVCoord(&at, &textPos);
                g_pGameUI->Draw2DText(textPos, text, PFont, CRGBColour(0xff, 0xff, 0xff, 0xff), 0.75f, ENGINE_2D_LAYER_SUBTITLE, true);
            }
        }
    }
}

#pragma optimize("", on)
// ---- stubs ----
static char g_log[1024];
static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
static int g_wdtors = 0;
CWideString::~CWideString() { ++g_wdtors; }
bool CWideString::operator!=(const unsigned short* text) const
{
    const unsigned short* a = (const unsigned short*)PStringData;
    while (*a && *a == *text) { ++a; ++text; }
    return *a != *text;
}
float __stdcall GFVXToAX_009e1e10(float x) { Log("VX"); return x * 2.0f; }
float __stdcall GFVYToAY(float y) { Log("VY"); return y * 3.0f; }
void __fastcall GFACoordToVCoord(const C2DVector* in, C2DVector* out) { Log("VCoord"); out->X = in->X * 10.0f; out->Y = in->Y * 10.0f; }
struct SSprite { float X, Y; long layer; unsigned long graphic, b; long blend, interp; float sx, sy; unsigned char a, r, g, bl; unsigned long e; float u0, v0, u1, v1; };
static SSprite g_sprites[4]; static int g_spriteCount = 0;
CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& pos, long layer, unsigned long graphic, unsigned long b,
    long blend, long interpolation, float scaleX, float scaleY, unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue,
    unsigned long e, const C2DVector& uv0, const C2DVector& uv1)
{
    Log("Sprite");
    CHECK(id == 0);
    SSprite s = { pos.X, pos.Y, layer, graphic, b, blend, interpolation, scaleX, scaleY, alpha, red, green, blue, e, uv0.X, uv0.Y, uv1.X, uv1.Y };
    memcpy(_body, &g_spriteCount, sizeof g_spriteCount);
    if (g_spriteCount < 4) g_sprites[g_spriteCount] = s;
    ++g_spriteCount;
}
void CEnginePrimitiveRenderer::v00() {} void CEnginePrimitiveRenderer::v01() {} void CEnginePrimitiveRenderer::v02() {}
void CEnginePrimitiveRenderer::v03() {} void CEnginePrimitiveRenderer::v04() {} void CEnginePrimitiveRenderer::v05() {}
void CEnginePrimitiveRenderer::v06() {} void CEnginePrimitiveRenderer::v07() {} void CEnginePrimitiveRenderer::v08() {}
void CEnginePrimitiveRenderer::v09() {} void CEnginePrimitiveRenderer::v10() {} void CEnginePrimitiveRenderer::v11() {}
void CEnginePrimitiveRenderer::v12() {} void CEnginePrimitiveRenderer::v13() {} void CEnginePrimitiveRenderer::v14() {}
void CEnginePrimitiveRenderer::v15() {} void CEnginePrimitiveRenderer::v16() {} void CEnginePrimitiveRenderer::v17() {}
void CEnginePrimitiveRenderer::v18() {} void CEnginePrimitiveRenderer::v19() {} void CEnginePrimitiveRenderer::v20() {}
void CEnginePrimitiveRenderer::v21() {} void CEnginePrimitiveRenderer::v22() {}
void CEnginePrimitiveRenderer::AddPrimitive(CEnginePrimitiveHandle*, const CEnginePrimitive2DSprite&, long) {}
static CEnginePrimitiveHandle* g_handles[4]; static int g_added[4]; static long g_addFlags = 0; static int g_addCount = 0;
struct CTestRenderer : CEnginePrimitiveRenderer {
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags)
    {
        Log("Add");
        int n; memcpy(&n, primitive._body, sizeof n);
        if (g_addCount < 4) { g_handles[g_addCount] = handle; g_added[g_addCount] = n; }
        ++g_addCount; g_addFlags = flags;
    }
};
static CPlayerGuiDef g_def;
const CPlayerGuiDef* __fastcall PeekGuiDef() { Log("Def"); return &g_def; }
static unsigned short g_empty[] = { 0 };
static unsigned short g_caption[] = { 'L', 'e', 'f', 't', 0 };
CWideString CSubtitleRenderer::GetTextFromEmbeddedTagGraphic(long graphic) const
{
    Log("Text");
    CWideString r; r.PStringData = graphic == 7 ? g_caption : g_empty;
    return r;
}
static C2DVector g_textOffset(0.5f, 0.25f);
const C2DVector& CSubtitleRenderer::GetTextOffsetFromEmbeddedTagGraphic(long graphic) const { Log("TextOffset"); CHECK(graphic == 7); return g_textOffset; }
struct SDraw { float X, Y; const void* text; CFontBank* font; unsigned char b, g, r, a; float scale; long layer; bool flag; };
static SDraw g_draw; static int g_draws = 0;
void CGameUI::Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour, float scale,
                         EEngine2DLayer layer, bool flag)
{
    Log("Draw");
    SDraw d = { pos.X, pos.Y, text.PStringData, font, colour.B, colour.G, colour.R, colour.A, scale, layer, flag };
    g_draw = d; ++g_draws;
}
static CGameUI g_ui;
CGameUI* g_pGameUI = &g_ui;

static void Reset() { g_log[0] = 0; g_step = 0; g_spriteCount = 0; g_addCount = 0; g_draws = 0; g_wdtors = 0; g_addFlags = 0; }

int main()
{
    CTestRenderer renderer;
    CEnginePrimitivePrepareInfo info; memset(&info, 0, sizeof info); info.Renderer = &renderer;
    g_def.ControllerGraphicOffset = C2DVector(100.0f, 200.0f);
    C2DBoxF box = { 1.0f, 2.0f, 50.0f, 60.0f };
    CSubtitleRenderer r; memset(&r, 0, sizeof r);
    CHECK((char*)&r.EmbeddedGraphics - (char*)&r == 0xb8 && (char*)&r.AllCharactersDisplayed - (char*)&r == 0xdd && (char*)&r.PFont - (char*)&r == 0x80);
    r.PFont = (CFontBank*)0x1234;
    CEnginePrimitiveHandle handles[3];
    CEmbeddedGraphic graphics[3] = { { 7, 2, C2DVector(10.0f, 20.0f) }, { 8, 5, C2DVector(30.0f, 40.0f) }, { 9, 1, C2DVector(0.0f, 0.0f) } };
    r.EmbeddedPrimitiveHandles.m_pFirst = handles; r.EmbeddedPrimitiveHandles.m_pLast = handles + 3;
    // 1: no embedded graphics: nothing at all (not even the GUI def)
    {
        Reset();
        r.DrawTextEmbeddedGraphics(info, box);
        CHECK(g_log[0] == 0);
    }
    r.EmbeddedGraphics.m_pFirst = graphics; r.EmbeddedGraphics.m_pLast = graphics + 3;
    // 2: five characters showing: graphics 0 (char 2) and 2 (char 1) drawn, graphic 1 (char 5) not yet; only graphic 7 has a caption
    {
        Reset(); r.TextCharactersShowing = 5; r.AllCharactersDisplayed = false;
        r.DrawTextEmbeddedGraphics(info, box);
        CHECK(strcmp(g_log, "1:Def;2:VY;3:VX;4:Sprite;5:Add;6:Text;7:TextOffset;8:VCoord;9:Draw;10:VY;11:VX;12:Sprite;13:Add;14:Text;") == 0);
        const SSprite& s = g_sprites[0];
        CHECK(s.X == 10.0f + 100.0f + 2.0f && s.Y == 20.0f + 200.0f + 6.0f && s.layer == 0x45 && s.graphic == 7 && s.b == 0 && s.blend == 2);
        CHECK(s.interp == 0 && s.sx == 0.0f && s.sy == 0.0f && s.a == 0xff && s.r == 0xff && s.g == 0xff && s.bl == 0xff && s.e == 0);
        CHECK(s.u0 == 0.0f && s.v0 == 0.0f && s.u1 == 0.0f && s.v1 == 0.0f);
        CHECK(g_sprites[1].graphic == 9 && g_sprites[1].X == 102.0f && g_sprites[1].Y == 206.0f);
        CHECK(g_addCount == 2 && g_handles[0] == &handles[0] && g_handles[1] == &handles[2] && g_added[0] == 0 && g_added[1] == 1 && g_addFlags == 0xc0);
        CHECK(g_draws == 1 && g_draw.X == (112.0f + 0.5f) * 10.0f && g_draw.Y == (226.0f + 0.25f) * 10.0f && g_draw.text == g_caption);
        CHECK(g_draw.font == (CFontBank*)0x1234 && g_draw.b == 0xff && g_draw.g == 0xff && g_draw.r == 0xff && g_draw.a == 0xff);
        CHECK(g_draw.scale == 0.75f && g_draw.layer == 0x43 && g_draw.flag == true);
        CHECK(g_wdtors == 4);
    }
    // 3: all characters displayed: every graphic is drawn whatever the reveal count
    {
        Reset(); r.TextCharactersShowing = 0; r.AllCharactersDisplayed = true;
        r.DrawTextEmbeddedGraphics(info, box);
        CHECK(g_spriteCount == 3 && g_addCount == 3 && g_handles[1] == &handles[1] && g_sprites[1].graphic == 8 && g_draws == 1);
    }
    // 4: nothing revealed yet: the def is peeked but no graphic is drawn
    {
        Reset(); r.AllCharactersDisplayed = false;
        r.DrawTextEmbeddedGraphics(info, box);
        CHECK(strcmp(g_log, "1:Def;") == 0);
    }
    if (fails == 0) printf("0048f230_TEST PASS\n");
    return fails;
}