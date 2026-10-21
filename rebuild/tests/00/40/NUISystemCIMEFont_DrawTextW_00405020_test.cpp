// Behaviour test for 00405020: src/00405020.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. NUISystem::CIMEFont::DrawTextW(const unsigned short*)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NUISystem::CIMEFont::DrawTextW(const unsigned short* text) -- draw IME text with the Arial font bank picked by height: the colour takes the composition alpha, a black drop shadow two units down-left is drawn first unless the colour is dark (all channels <= 0x60), then the text at the converted position
// Shape: the address-taken `scale` local lands in the dead `text` parameter slot and is read once into ebp only when the
//   GetFontNameAndScale body is visible in the TU (same-TU noinline copy of the landed 0x00404970); each draw fetches the 2D
//   render manager into a local before the call (retail evaluates it before the CWideString argument copy), and the main draw's
//   C3DVector sits in its own block so it shares the shadow draw's slot (0x2c frame).
// Leaf: DrawTextW
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00405020, length 410 bytes (`ret 4`).
// Callees: CWideString(const unsigned short*) 0x0099B6B0 / CWideString(const CWideString&) 0x0099B720 / ~CWideString 0x0099B510,
//   CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0, CIMEFont::GetFontNameAndScale 0x00404970 (landed), GFGetSystemManager
//   0x009A4EC0 (font bank +0x84, display manager +0x60 -> 2D render manager +8), the bank's GetFont 0x009E2C80 (counted
//   pointer by hidden pointer, as the landed NUISystem::CText::GetDataFromDef 0x0054ED90), GFACoordToVCoord 0x009E21D0
//   (__fastcall in, out), CRenderManager2D::Draw3DScaledText 0x009DBFF0 (landed), operator delete 0x00BFE9BC.
// Layout: CIMEFont as the landed GetFontNameAndScale 0x00404970 (vptr, Colour +4, Position +8, Height +0x10); the composition
//   alpha 0x013750B0 as the landed CIME::Render 0x00404C00; shadow offset 0x0122DCDC = 2.0f; wchar_t is `unsigned short` (no /Zc:wchar_t).
class CCharString {
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
    const CCharString& operator+=(const char* text);    // 0x0099F100
};
class CWideString {
public:
    void* PStringData;
    CWideString(const unsigned short* text);                   // 0x0099B6B0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
};
struct CCPPointerInfo {
    long References;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    void Release()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
    }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
};
struct CFontBank;
struct CFontBankSource {
    CCountedPointer<CFontBank> GetFont(const CCharString& name);   // 0x009E2C80
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CRenderManager2D {
    bool Draw3DScaledText(const C3DVector& position, float scale, CWideString text, CFontBank* font,
                          const CRGBColour& colour, unsigned long flags);                 // 0x009DBFF0
};
struct CDisplayManager {
    unsigned char     _pad00[8];
    CRenderManager2D* RenderManager2D;                  // +8
};
struct CSystemManager {
    unsigned char    _pad00[0x60];
    CDisplayManager* DisplayManager;                    // +0x60
    unsigned char    _pad64[0x84 - 0x64];
    CFontBankSource* FontBank;                          // +0x84
    CFontBankSource* GetFontBank() const { return FontBank; }
};
CSystemManager* __fastcall GFGetSystemManager();        // 0x009A4EC0
void __fastcall GFACoordToVCoord(const C2DVector* in, C2DVector* out);   // 0x009E21D0
extern unsigned char g_imeCompositionAlpha_013750b0;    // 0x013750B0
class CImeUiFont_Base {
public:
    virtual void SetHeight(unsigned int) = 0;
    virtual void SetColor(unsigned long) = 0;
    virtual void SetPosition(int, int) = 0;
    virtual void GetTextExtent(const unsigned short*, unsigned long*, unsigned long*) = 0;
    virtual void DrawTextW(const unsigned short*) = 0;
};
namespace NUISystem {
class CIMEFont : public CImeUiFont_Base {
public:
    virtual void SetHeight(unsigned int);
    virtual void SetColor(unsigned long);
    virtual void SetPosition(int, int);
    virtual void GetTextExtent(const unsigned short*, unsigned long*, unsigned long*);
    virtual void DrawTextW(const unsigned short* text);
    CRGBColour    Colour;                               // +0x04
    float         Position[2];                          // +0x08
    unsigned long Height;                               // +0x10
    void GetFontNameAndScale(CCharString& name, float& scale) const;   // 0x00404970
};

#pragma auto_inline(off)
// Same-TU copy of the landed CIMEFont::GetFontNameAndScale 0x00404970 (scale written by value only): with the body visible VC7.1
// knows the out-parameter's address does not escape. Only DrawTextW is graded.
__declspec(noinline) void CIMEFont::GetFontNameAndScale(CCharString& name, float& scale) const
{
    float height = (float)Height;
    name = "ENG_ARIAL_";
    if (Height < 16) {
        name += "12";
        scale = height * (1.0f / 12.0f);
    } else if (Height < 18) {
        name += "16";
        scale = height * (1.0f / 16.0f);
    } else if (Height < 24) {
        name += "18";
        scale = height * (1.0f / 18.0f);
    } else if (Height < 28) {
        name += "24";
        scale = height * (1.0f / 24.0f);
    } else {
        name += "28";
        scale = height * (1.0f / 28.0f);
    }
}
#pragma auto_inline(on)
void CIMEFont::DrawTextW(const unsigned short* text)
{
    CWideString wideText(text);
    CCharString fontName;
    float scale;
    GetFontNameAndScale(fontName, scale);
    CCountedPointer<CFontBank> font = GFGetSystemManager()->GetFontBank()->GetFont(fontName);
    C2DVector position(Position[0], Position[1]);
    GFACoordToVCoord(&position, &position);
    Colour.A = g_imeCompositionAlpha_013750b0;
    if (Colour.R > 0x60 || Colour.G > 0x60 || Colour.B > 0x60) {
        C2DVector shadowPosition(Position[0] - 2.0f, Position[1] + 2.0f);
        GFACoordToVCoord(&shadowPosition, &shadowPosition);
        CRGBColour shadow(0, 0, 0, Colour.A);
        C3DVector shadowPosition3(shadowPosition.X, shadowPosition.Y, 0.0f);
        CRenderManager2D* renderManager = GFGetSystemManager()->DisplayManager->RenderManager2D;
        renderManager->Draw3DScaledText(shadowPosition3, scale, wideText, font.Data, shadow, 0);
    }
    {
        C3DVector position3(position.X, position.Y, 0.0f);
        CRenderManager2D* renderManager = GFGetSystemManager()->DisplayManager->RenderManager2D;
        renderManager->Draw3DScaledText(position3, scale, wideText, font.Data, Colour, 0);
    }
}
}
// ---- stubs ----
CCharString::CCharString() { char* b = new char[64]; b[0] = 0; PStringData = b; }
CCharString::~CCharString() { delete[] (char*)PStringData; }
CCharString& CCharString::operator=(const char* text) { strcpy((char*)PStringData, text); return *this; }
const CCharString& CCharString::operator+=(const char* text) { strcat((char*)PStringData, text); return *this; }
static int g_wctor, g_wcopy, g_wdtor;
CWideString::CWideString(const unsigned short* text) { PStringData = (void*)text; ++g_wctor; }
CWideString::CWideString(const CWideString& other) { PStringData = other.PStringData; ++g_wcopy; }
CWideString::~CWideString() { ++g_wdtor; }
static char g_fontName[64];
static CFontBank* const g_font = (CFontBank*)0x1234;
static CCPPointerInfo* g_info;
static int g_destroyed; static void* g_destroyedObj;
static void __fastcall DestroyFont(void* object) { ++g_destroyed; g_destroyedObj = object; }
CCountedPointer<CFontBank> CFontBankSource::GetFont(const CCharString& name)
{
    strcpy(g_fontName, (const char*)name.PStringData);
    static CCountedPointer<CFontBank> held;   // returned by bitwise copy; the static itself is never released here
    held.Data = g_font; held.Info = g_info;
    return held;
}
static CDisplayManager g_dm; static CRenderManager2D g_rm; static CFontBankSource g_fbs; static CSystemManager g_sm;
CSystemManager* __fastcall GFGetSystemManager() { return &g_sm; }
void __fastcall GFACoordToVCoord(const C2DVector* in, C2DVector* out) { out->X = in->X * 10.0f; out->Y = in->Y * 10.0f; }
unsigned char g_imeCompositionAlpha_013750b0;
struct DrawCall { CRenderManager2D* self; float x, y, z, scale; void* text; CFontBank* font; const CRGBColour* colourRef;
                  CRGBColour colour; unsigned long flags; };
static DrawCall g_draws[4]; static int g_nDraws;
bool CRenderManager2D::Draw3DScaledText(const C3DVector& position, float scale, CWideString text, CFontBank* font,
                                        const CRGBColour& colour, unsigned long flags)
{
    DrawCall& d = g_draws[g_nDraws++];
    d.self = this; d.x = position.X; d.y = position.Y; d.z = position.Z; d.scale = scale; d.text = text.PStringData;
    d.font = font; d.colourRef = &colour; d.colour = colour; d.flags = flags;
    return true;
}
namespace NUISystem {
void CIMEFont::SetHeight(unsigned int) {}
void CIMEFont::SetColor(unsigned long) {}
void CIMEFont::SetPosition(int, int) {}
void CIMEFont::GetTextExtent(const unsigned short*, unsigned long*, unsigned long*) {}
}
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
static void Reset() { g_wctor = g_wcopy = g_wdtor = g_nDraws = g_destroyed = 0; g_destroyedObj = 0; g_fontName[0] = 0; memset(g_draws, 0, sizeof g_draws); }
int main()
{
    g_sm.DisplayManager = &g_dm; g_dm.RenderManager2D = &g_rm; g_sm.FontBank = &g_fbs;
    static const unsigned short text[] = { 'h', 'i', 0 };
    NUISystem::CIMEFont font;
    CCPPointerInfo info = { 2, DestroyFont, (void*)0x5678 };
    // bright colour, height 20: shadow then text, Arial 18, scale 20/18
    Reset(); g_info = &info; g_imeCompositionAlpha_013750b0 = 0x80;
    font.Colour.R = 0x90; font.Colour.G = 0x10; font.Colour.B = 0x10; font.Colour.A = 0xff;
    font.Position[0] = 5.0f; font.Position[1] = 7.0f; font.Height = 20;
    font.DrawTextW(text);
    CHECK(strcmp(g_fontName, "ENG_ARIAL_18") == 0);
    CHECK(font.Colour.A == 0x80);
    CHECK(g_nDraws == 2);
    CHECK(g_draws[0].self == &g_rm && Near(g_draws[0].x, 30.0f) && Near(g_draws[0].y, 90.0f) && g_draws[0].z == 0.0f);
    CHECK(Near(g_draws[0].scale, 20.0f / 18.0f) && g_draws[0].text == (void*)text && g_draws[0].font == g_font);
    CHECK(g_draws[0].colour.R == 0 && g_draws[0].colour.G == 0 && g_draws[0].colour.B == 0 && g_draws[0].colour.A == 0x80);
    CHECK(g_draws[0].flags == 0);
    CHECK(g_draws[1].self == &g_rm && Near(g_draws[1].x, 50.0f) && Near(g_draws[1].y, 70.0f) && g_draws[1].z == 0.0f);
    CHECK(Near(g_draws[1].scale, 20.0f / 18.0f) && g_draws[1].text == (void*)text && g_draws[1].font == g_font);
    CHECK(g_draws[1].colourRef == &font.Colour && g_draws[1].flags == 0);
    CHECK(g_wctor == 1 && g_wcopy == 2 && g_wdtor == 3);
    CHECK(info.References == 1 && g_destroyed == 0);
    // dark colour (all channels <= 0x60), height 30: no shadow, Arial 28; last reference destroys the font
    Reset();
    CCPPointerInfo* heapInfo = new CCPPointerInfo; heapInfo->References = 1; heapInfo->Destroy = DestroyFont; heapInfo->Object = (void*)0x9abc;
    g_info = heapInfo; g_imeCompositionAlpha_013750b0 = 0x40;
    font.Colour.R = 0x60; font.Colour.G = 0x60; font.Colour.B = 0x60; font.Height = 30;
    font.DrawTextW(text);
    CHECK(strcmp(g_fontName, "ENG_ARIAL_28") == 0);
    CHECK(g_nDraws == 1 && Near(g_draws[0].x, 50.0f) && Near(g_draws[0].y, 70.0f));
    CHECK(Near(g_draws[0].scale, 30.0f / 28.0f) && g_draws[0].colour.A == 0x40 && g_draws[0].colourRef == &font.Colour);
    CHECK(g_wctor == 1 && g_wcopy == 1 && g_wdtor == 2);
    CHECK(g_destroyed == 1 && g_destroyedObj == (void*)0x9abc);
    // only blue bright, height 12: shadow drawn, Arial 12
    Reset(); info.References = 3; g_info = &info;
    font.Colour.R = 0; font.Colour.G = 0; font.Colour.B = 0x61; font.Height = 12;
    font.DrawTextW(text);
    CHECK(strcmp(g_fontName, "ENG_ARIAL_12") == 0 && g_nDraws == 2 && Near(g_draws[1].scale, 1.0f));
    CHECK(info.References == 2);
    if (fails == 0) printf("00405020_TEST PASS\n"); else printf("00405020_TEST FAIL\n");
    return fails;
}