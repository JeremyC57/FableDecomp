// TransferIMEColour(CPersistContext& context, const CCharString& name, unsigned long colour) -- persist one IME colour as four <name>R/G/B/A byte settings (defaulting to the given colour) and return it as an opaque X8R8G8B8 colour
// Grade: DIFFER(290v290) -- return packing only: retail reads R as a byte and G/B from one dword load (`mov ecx,[esp+14h] ; movzx edx,ch ; movzx ecx,cl`), ours byte-loads all three (the out-of-line RGB2IMEColour 0x00404920 byte-loads too); copies of rgb before the call fold away
// Leaf: ?TransferIMEColour@@YIKAAVCPersistContext@@ABVCCharString@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00405220, 290 bytes (304 with padding), `ret 4`; Ego_r.pdb / FableWin.pdb
//   ?TransferIMEColour@@YIKAAVCPersistContext@@ABVCCharString@@K@Z (__fastcall: context in ecx, name in edx, the colour on the stack).
//   Callers: the colour settings of NUISystem::CIME::LoadIMESettings 0x00405350.
// Inlined: ?IMEColour2RGB@@YI?AVCRGBColour@@K@Z (B/G/R from the low three bytes, A = the IME composition alpha 0x013750B0) and
//   ?RGB2IMEColour@@YIKAAVCRGBColour@@@Z (0xFF000000 | R<<16 | G<<8 | B; its unreferenced out-of-line copy is 0x00404920).
// Callees: operator+(const CCharString&, const char*) 0x0099F600 (__fastcall, hidden result), CCharString::operator const char* 0x0099E4C0,
//   CPersistContext::Transfer<unsigned char>(const char*, unsigned char&, const unsigned char&) 0x00405A60, ~CCharString 0x0099EAE0.
// Shape: the colour is built in the dead argument slot and copied whole (CRGBColour's dword copy constructor,
//   FABLE_RETAIL_CRGBCOLOUR_DWORD_COPY) as the defaults; each Transfer takes &default, &value and the name temporary.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CRGBCOLOUR_DWORD_COPY
#include "fable_string.h"
#include "engine/CRGBColour.h"
#include "engine/CPersistContext.h"

extern unsigned char g_imeCompositionAlpha_013750b0;   // 0x013750B0 (landed name: the alpha the IME UI draws the composition with)

// ?IMEColour2RGB@@YI?AVCRGBColour@@K@Z: an X8R8G8B8 IME colour to a CRGBColour with the composition alpha.
CRGBColour FABLE_FASTCALL IMEColour2RGB(unsigned long colour)
{
    unsigned char blue = (unsigned char)colour;
    colour >>= 8;
    unsigned char green = (unsigned char)colour;
    colour >>= 8;
    unsigned char red = (unsigned char)colour;
    return CRGBColour(red, green, blue, g_imeCompositionAlpha_013750b0);
}

// ?RGB2IMEColour@@YIKAAVCRGBColour@@@Z: a CRGBColour back to an opaque X8R8G8B8 colour (0x00404920).
unsigned long FABLE_FASTCALL RGB2IMEColour(CRGBColour& colour)
{
    return 0xff000000 | ((unsigned long)colour.R << 16) | ((unsigned long)colour.G << 8) | colour.B;
}

unsigned long FABLE_FASTCALL TransferIMEColour(CPersistContext& context, const CCharString& name, unsigned long colour)
{
    CRGBColour rgb = IMEColour2RGB(colour);
    CRGBColour defaultRgb = rgb;
    context.Transfer(name + "R", rgb.R, defaultRgb.R);
    context.Transfer(name + "G", rgb.G, defaultRgb.G);
    context.Transfer(name + "B", rgb.B, defaultRgb.B);
    context.Transfer(name + "A", rgb.A, defaultRgb.A);
    return RGB2IMEColour(rgb);
}
