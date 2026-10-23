// CSystemManager::InitSystemColours() -- fill the 25 GUI colours from the Windows system palette
// Retail 0x009A5490, 0x318 bytes. Each GetSysColor (import pointer 0x01440374, held in edi) COLORREF
// 0x00BBGGRR is split into the CRGBColour bytes {B,G,R,A}: R = low byte -> +2, G = ah -> +1,
// B = (c >> 16) -> +0, alpha 0xff (bl) -> +3.
// Layout: retail CGuiColoursInfo SystemColours sits at +0x100 (Ego_r.pdb says +0xf8): the
// retail CSystemManager is 8 bytes larger ahead of it, so the model pads to 0x100.
extern "C" __declspec(dllimport) unsigned long __stdcall GetSysColor(int index);   // [0x01440374]

struct CRGBColour
{
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) {}
};

static inline void SetFromSystem(CRGBColour& colour, unsigned long c)
{
    colour.R = (unsigned char)c;                              // GetRValue
    colour.G = (unsigned char)((unsigned short)c >> 8);       // GetGValue
    colour.B = (unsigned char)(c >> 16);                      // GetBValue
    colour.A = 0xff;
}

struct CGuiColoursInfo
{
    CRGBColour BackgroundShadow3D;
    CRGBColour ButtonFace;
    CRGBColour ButtonHighlight;
    CRGBColour ButtonLight;
    CRGBColour ButtonShadow;
    CRGBColour ButtonText;
    CRGBColour ActiveBorder;
    CRGBColour ActiveWindowLeftSideCaption;
    CRGBColour ActiveWindowRightSideCaption;
    CRGBColour InactiveWindowLeftSideCaption;
    CRGBColour InactiveWindowRightSideCaption;
    CRGBColour DesktopColour;
    CRGBColour CaptionText;
    CRGBColour CaptionInactiveText;
    CRGBColour GrayText;
    CRGBColour HighlightItem;
    CRGBColour HighlightText;
    CRGBColour TooltipBackground;
    CRGBColour TooltipText;
    CRGBColour MenuBackground;
    CRGBColour MenuText;
    CRGBColour Scrollbar;
    CRGBColour WindowBackground;
    CRGBColour WindowFrame;
    CRGBColour WindowText;
};

class CSystemManager
{
public:
    unsigned char _pad[0x100];
    CGuiColoursInfo SystemColours;
    void InitSystemColours();
};

void CSystemManager::InitSystemColours()
{
    SetFromSystem(SystemColours.BackgroundShadow3D, GetSysColor(21));            // COLOR_3DDKSHADOW
    SetFromSystem(SystemColours.ButtonFace, GetSysColor(15));                    // COLOR_BTNFACE
    SetFromSystem(SystemColours.ButtonHighlight, GetSysColor(20));               // COLOR_BTNHIGHLIGHT
    SetFromSystem(SystemColours.ButtonLight, GetSysColor(22));                   // COLOR_3DLIGHT
    SetFromSystem(SystemColours.ButtonShadow, GetSysColor(16));                  // COLOR_BTNSHADOW
    SetFromSystem(SystemColours.ButtonText, GetSysColor(18));                    // COLOR_BTNTEXT
    SetFromSystem(SystemColours.ActiveBorder, GetSysColor(10));                  // COLOR_ACTIVEBORDER
    SetFromSystem(SystemColours.ActiveWindowRightSideCaption, GetSysColor(27));  // COLOR_GRADIENTACTIVECAPTION
    SetFromSystem(SystemColours.InactiveWindowRightSideCaption, GetSysColor(28));// COLOR_GRADIENTINACTIVECAPTION
    SetFromSystem(SystemColours.ActiveWindowLeftSideCaption, GetSysColor(2));    // COLOR_ACTIVECAPTION
    SetFromSystem(SystemColours.InactiveWindowLeftSideCaption, GetSysColor(3));  // COLOR_INACTIVECAPTION
    SetFromSystem(SystemColours.DesktopColour, GetSysColor(1));                  // COLOR_DESKTOP
    SetFromSystem(SystemColours.CaptionText, GetSysColor(9));                    // COLOR_CAPTIONTEXT
    SetFromSystem(SystemColours.CaptionInactiveText, GetSysColor(19));           // COLOR_INACTIVECAPTIONTEXT
    SetFromSystem(SystemColours.GrayText, GetSysColor(17));                      // COLOR_GRAYTEXT
    SetFromSystem(SystemColours.HighlightItem, GetSysColor(13));                 // COLOR_HIGHLIGHT
    SetFromSystem(SystemColours.HighlightText, GetSysColor(14));                 // COLOR_HIGHLIGHTTEXT
    SetFromSystem(SystemColours.TooltipBackground, GetSysColor(24));             // COLOR_INFOBK
    SetFromSystem(SystemColours.TooltipText, GetSysColor(23));                   // COLOR_INFOTEXT
    SetFromSystem(SystemColours.MenuBackground, GetSysColor(4));                 // COLOR_MENU
    SetFromSystem(SystemColours.MenuText, GetSysColor(7));                       // COLOR_MENUTEXT
    SetFromSystem(SystemColours.Scrollbar, GetSysColor(0));                      // COLOR_SCROLLBAR
    SetFromSystem(SystemColours.WindowBackground, GetSysColor(5));               // COLOR_WINDOW
    SetFromSystem(SystemColours.WindowFrame, GetSysColor(6));                    // COLOR_WINDOWFRAME
    SetFromSystem(SystemColours.WindowText, GetSysColor(8));                     // COLOR_WINDOWTEXT
}
