typedef unsigned char u8;
typedef unsigned int u32;

class CRGBColour
{ public:
    u8 b0; u8 b1; u8 b2; u8 b3;
    CRGBColour(u32 argb) { b2 = (u8)(argb >> 16); b1 = (u8)(argb >> 8); b0 = (u8)argb; b3 = (u8)(argb >> 24); }
    CRGBColour(const CRGBColour& o) { b0 = o.b0; b1 = o.b1; b2 = o.b2; b3 = o.b3; }
};

class CTCCreditsUI;
class CGameScriptInterface
{
public:
    void __fastcall FadeScreenOutUntilNextCallToFadeScreenIn(float fadeTime, float holdTime);
};

struct CTCCreditsUIOverlay
{
    u8 pad00[0xD8];
    u8 fadeScreenOutFlag_d8;
};

struct CGameScriptInterfaceOverlay
{
    u8 pad00[0x0C];
    CTCCreditsUI* creditsUI_0c;
};

typedef char(__fastcall* FadeScreenOutRawThunk)(CGameScriptInterface*, float, float, CRGBColour);

void __fastcall CGameScriptInterface::FadeScreenOutUntilNextCallToFadeScreenIn(float fadeTime, float holdTime)
{
    CGameScriptInterfaceOverlay* overlay = reinterpret_cast<CGameScriptInterfaceOverlay*>(this);

    if ((*reinterpret_cast<FadeScreenOutRawThunk**>(this))[0x5D4 / sizeof(void*)](this, fadeTime, holdTime, CRGBColour(0xFF000000u)) != 0) {
        CTCCreditsUIOverlay* creditsUI = reinterpret_cast<CTCCreditsUIOverlay*>(overlay->creditsUI_0c);
        creditsUI->fadeScreenOutFlag_d8 = 1;
    }
}