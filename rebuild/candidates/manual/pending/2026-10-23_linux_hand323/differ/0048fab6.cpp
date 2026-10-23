// CSubtitleRenderer::DrawButtonText(C2DVector* pos, bool withPanel) -- lay the three button labels out right-to-left from the given position (gapped by the def's button spacing, lower in a panel), then draw: all three (button mode), the skip prompt (cutscene / paused: the localised ESC abbreviation prefixed onto the skip text, pushed right in widescreen), button 1 only, or each non-empty button; finally the extra label (+0x70), clamped to the screen edge in widescreen; returns whether anything was drawn
// Grade: DIFFER(1569v1599) -- first draft: frame 0x38 vs 0x3c (font-handle temp reuses the dead pos param slot, ebx not spilled to [ebp-8]), local slot order and branch/tail-merge layout differ
// Leaf: DrawButtonText
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048FAB6, 1599 bytes, `ret 8`; size-optimised TU with an ebp frame. Local layout (offsets do not
//   follow the Ego_r.pdb CSubtitleRenderer layout).
// Callees: ToScreen 0x009E21D0 (__fastcall), GetSubtitleDef 0x00438570, GetFont 0x009E2C80 / handle dtor 0x004057A0, ScaleH 0x009E1DD0,
//   ScaleW 0x009E1DA0, MeasureText 0x0048E062, GetResolution 0x009BEDC0, DrawText 0x0048F1F4, IsSkipAvailable 0x0048DE44,
//   IsButton1Active 0x0048E072, CWideString copy 0x0099B720 / operator!=(const unsigned short*) 0x0099B150 / operator= 0x0099B7D0 /
//   dtor 0x0099B510, operator+ 0x0099BF30 / 0x0099BE70 (__fastcall, hidden result), CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0,
//   GetLocalisedText 0x009C95E0, CFont::GetTextSize 0x00437210, GetDisplayMode 0x00644D54, GetWidescreenOffset 0x00643CA1 (__fastcall, hidden result),
//   GetPlayerManager 0x00449970.
// Literals: L"" 0x0122D70C, "TEXT_GUI_CONTROL_ESC_ABBREVIATED" 0x01238208, L" " 0x0122E070; 640.0f 0x01238168, 16.0f 0x01238164,
//   55.0f 0x01238204, 2.0f 0x0122DCDC, 4.0f 0x0123088C.
#pragma optimize("s", on)
#pragma optimize("y", off)

struct C2DVector { float X, Y; C2DVector() { } C2DVector(float x, float y) : X(x), Y(y) { } };
void __fastcall ToScreen(const C2DVector& in, C2DVector& out);           // 0x009E21D0
C2DVector __fastcall GetWidescreenOffset(const void* setting);          // 0x00643CA1
struct CIntVec2 { long X, Y; CIntVec2() { } };
struct CDisplay { CIntVec2 GetResolution() const; };                    // 0x009BEDC0
extern CDisplay* g_Display;                                              // 0x013B8390
float ScaleW(float v);                                                   // 0x009E1DA0
float ScaleH(float v);                                                   // 0x009E1DD0
struct CRGBColour { unsigned char B, G, R, A; };
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);                               // 0x0099B720
    ~CWideString();                                                      // 0x0099B510
    CWideString& operator=(const CWideString& other);                    // 0x0099B7D0
    bool operator!=(const unsigned short* text) const;                   // 0x0099B150
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);       // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);    // 0x0099BF30
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);                     // 0x0099EBF0
    ~CCharString();                                                      // 0x0099EAE0
};
struct CFont { C2DVector GetTextSize(const CWideString& text); };        // 0x00437210
struct CFontHandle { CFont* P; ~CFontHandle(); };                        // dtor 0x004057A0
struct CFontManager { CFontHandle GetFont(const CCharString& name); };   // 0x009E2C80
extern CFontManager* g_FontManager;                                      // 0x013B838C
struct CTextManager { CWideString GetText(const CCharString& key); };    // 0x009C95E0
struct CPlayer { unsigned char _pad[0x3c]; long Mode; };
struct CPlayerManager { CPlayer* GetMainPlayer(); };                     // 0x00449970
struct CEngineRoot { unsigned char _pad[0x14]; CTextManager* Text; unsigned char _pad18[4]; CPlayerManager* Players; };
extern CEngineRoot* g_EngineRoot;                                        // 0x013B86A0
struct CDisplaySettings { long GetDisplayMode(); };                     // 0x00644D54
struct CGameOptions { unsigned char _pad[0x1c8]; CDisplaySettings* Display; };
extern CGameOptions* g_GameOptions;                                      // 0x013B8790
struct CSubtitleDef {
    unsigned char _pad00[0x5ac];
    float TextBaseX;                                                     // +0x5ac
    unsigned char _pad5b0[0x5b4 - 0x5b0];
    CRGBColour ButtonColour;                                             // +0x5b4
    unsigned char _pad5b8[0x5c8 - 0x5b8];
    float ButtonGap;                                                     // +0x5c8
    float ButtonPanelGap;                                                // +0x5cc
    float ButtonSpacing;                                                 // +0x5d0
    CCharString ButtonFontName;                                          // +0x5d4
    unsigned char _pad5d8[0x60c - 0x5d8];
    unsigned char WidescreenOffset[8];                                   // +0x60c
};
CSubtitleDef* GetSubtitleDef();                                          // 0x00438570

class CSubtitleRenderer {
public:
    void* __vftable;
    unsigned char _pad04[0x64 - 0x04];
    CWideString Button1Text;                                             // +0x64
    CWideString Button2Text;                                             // +0x68
    CWideString Button3Text;                                             // +0x6c
    CWideString ExtraText;                                               // +0x70
    CWideString SkipText;                                                // +0x74
    unsigned char Button1Alpha;                                          // +0x78
    unsigned char Button2Alpha;                                          // +0x79
    unsigned char _pad7a[2];
    unsigned long DisplayFlags;                                          // +0x7c
    unsigned char _pad80[0xde - 0x80];
    bool InCutscene;                                                     // +0xde
    unsigned char _paddf[0xe2 - 0xdf];
    bool FixedPosition;                                                  // +0xe2
    bool ShowExtra;                                                      // +0xe3
    bool PausedGame;                                                     // +0xe4

    bool IsSkipAvailable();                                              // 0x0048DE44
    bool IsButton1Active();                                              // 0x0048E072
    float MeasureText(const CWideString& text, CFont* font);             // 0x0048E062
    void DrawText(const C2DVector& pos, const CWideString& text, CFont* font, const CRGBColour& colour, bool shadow,
                  float scaleX, float scaleY, bool centred);             // 0x0048F1F4
    bool DrawButtonText(C2DVector* pos, bool withPanel);
};

bool CSubtitleRenderer::DrawButtonText(C2DVector* pos, bool withPanel)
{
    C2DVector screen;
    ToScreen(*pos, screen);
    CSubtitleDef* def = GetSubtitleDef();
    CFont* font = g_FontManager->GetFont(def->ButtonFontName).P;
    C2DVector pos1(0.0f, screen.Y);
    C2DVector pos2(0.0f, screen.Y);
    C2DVector pos3(0.0f, screen.Y);
    if (withPanel) {
        pos1.Y += ScaleH(def->ButtonPanelGap);
        pos2.Y += ScaleH(def->ButtonPanelGap);
        pos3.Y += ScaleH(def->ButtonPanelGap);
    } else {
        pos1.Y += ScaleH(def->ButtonGap);
        pos2.Y += ScaleH(def->ButtonGap);
        pos3.Y += ScaleH(def->ButtonGap);
    }
    float width1 = MeasureText(Button1Text, font);
    float width2 = MeasureText(Button2Text, font);
    float width3 = MeasureText(Button3Text, font);
    if (FixedPosition)
        pos1.X = ScaleW((float)g_Display->GetResolution().X + def->TextBaseX - 640.0f) + ScaleW(16.0f);
    else
        pos1.X = ScaleW(16.0f) + screen.X - width1;
    pos2.X = pos1.X - width2 - ScaleW(def->ButtonSpacing);
    pos3.X = pos2.X - width3 - ScaleW(def->ButtonSpacing);
    CRGBColour colour = def->ButtonColour;
    bool drawn = false;
    bool drewButton1 = false;
    if (FixedPosition) {
        drawn = true;
    } else if (DisplayFlags & 4) {
        colour.A = Button1Alpha;
        DrawText(pos1, Button1Text, font, colour, true, 1.0f, 1.0f, false);
        colour.A = Button2Alpha;
        DrawText(pos2, Button2Text, font, colour, true, 1.0f, 1.0f, false);
        colour.A = 0xff;
        DrawText(pos3, Button3Text, font, colour, true, 1.0f, 1.0f, false);
        return true;
    } else if ((InCutscene || PausedGame) && !(DisplayFlags & 0x48)) {
        if (IsSkipAvailable() || PausedGame) {
            CWideString text(SkipText);
            if (text != L"" && !FixedPosition) {
                CCharString key("TEXT_GUI_CONTROL_ESC_ABBREVIATED");
                text = g_EngineRoot->Text->GetText(key) + L" " + text;
            }
            pos1.X = screen.X - font->GetTextSize(text).X;
            colour.A = Button2Alpha;
            float scale = 1.0f;
            if (InCutscene) {
                colour.A = 0xd8;
                pos1.X += ScaleW(55.0f);
                pos1.Y = pos1.Y - ScaleW(2.0f);
                scale = 0.75f;
            }
            if (g_GameOptions->Display->GetDisplayMode() == 1)
                pos1.X += ScaleW(55.0f);
            DrawText(pos1, text, font, colour, true, 1.0f, scale, false);
            drawn = true;
        }
    } else if (DisplayFlags & 2) {
        if ((!InCutscene && IsButton1Active()) || (DisplayFlags & 0x48)) {
            colour.A = Button1Alpha;
            DrawText(pos1, Button1Text, font, colour, true, 1.0f, 1.0f, false);
            return true;
        }
    } else if (!InCutscene && g_EngineRoot->Players->GetMainPlayer()->Mode == 0) {
        if (Button1Text != L"") {
            colour.A = Button1Alpha;
            DrawText(pos1, Button1Text, font, colour, true, 1.0f, 1.0f, false);
            drawn = true;
            drewButton1 = true;
        }
        if (Button2Text != L"") {
            colour.A = Button2Alpha;
            DrawText(pos2, Button2Text, font, colour, true, 1.0f, 1.0f, false);
            drawn = true;
        }
        if (Button3Text != L"") {
            colour.A = 0xff;
            DrawText(pos3, Button3Text, font, colour, true, 1.0f, 1.0f, false);
            drawn = true;
        }
        if (drewButton1)
            return drawn;
    }
    if (ShowExtra) {
        pos2.X = pos1.X - MeasureText(ExtraText, font) - ScaleW(def->ButtonSpacing);
        if (g_GameOptions->Display->GetDisplayMode() == 1) {
            C2DVector offset = GetWidescreenOffset(def->WidescreenOffset);
            pos2.X = ScaleW(offset.X);
            pos2.Y = ScaleW(offset.Y);
            float width = MeasureText(ExtraText, font);
            if (ScaleW((float)g_Display->GetResolution().X) < width + pos2.X)
                pos2.X = pos2.X - (width + pos2.X - ScaleW((float)g_Display->GetResolution().X) + ScaleW(4.0f));
        }
        colour.A = Button1Alpha;
        DrawText(pos2, ExtraText, font, colour, true, 1.0f, 1.0f, false);
        drawn = true;
    }
    return drawn;
}
