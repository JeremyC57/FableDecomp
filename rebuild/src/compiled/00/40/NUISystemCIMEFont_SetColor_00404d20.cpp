// NUISystem::CIMEFont::SetColor(unsigned long) -- take the colour's RGB bytes and the IME composition alpha
// Grade: RELOCATION_MATCH -- the colour is built by the four-argument CRGBColour constructor (R,G,B,A stores) from `high = colour >> 8` (R = high >> 8, G = high, B = colour, A = the composition alpha) and assigned to Colour as one dword; the parked member-by-member stores reused eax (43v47); the shared engine/CCountedPointer.h replaces the local stand-in of the TU context
// Leaf: ?SetColor@CIMEFont@NUISystem@@UAEXK@Z
// Retail 0x00404D20, 47 bytes (vtable 0x0122DCBC slot 1, `ret 4`). The CRGBColour temporary is built in the dead argument
// slot, then copied as one dword to Colour (+4). Alpha byte 0x013750B0. Name: Ego_r.pdb ?SetColor@CIMEFont@NUISystem@@UAEXK@Z.
// Layout after the landed CIMEFont rows 0x00404970 / 0x00405020: CIMEFont derives CImeUiFont_Base (vtable 0x0122DCBC):
// Colour +4, Position +8, Height +0x10; composition alpha 0x013750B0; wchar_t is `unsigned short` (no /Zc:wchar_t).
#include "engine/CCountedPointer.h"
class CCharString {
public:
 void* PStringData;
 CCharString();                                      // 0x0099E4B0
 ~CCharString();                                     // 0x0099EAE0
};


class C2DVector { public:
 float X, Y;
 C2DVector(float x, float y) : X(x), Y(y) {}
};
class CFontBank { public:
 virtual void Slot0();
 virtual C2DVector GetTextExtent(const unsigned short* text, long flags);   // slot 1 (+4)
};
struct CFontBankSource {
 CCountedPointer<CFontBank> GetFont(const CCharString& name);   // 0x009E2C80
};
class CSystemManager { public:
 unsigned char _pad00[0x84];
 CFontBankSource* FontBank;                          // +0x84
 CFontBankSource* GetFontBank() const { return FontBank; }
};
CSystemManager* __fastcall GFGetSystemManager();     // 0x009A4EC0
class CRGBColour { public:
 unsigned char B, G, R, A;
 CRGBColour() {}
 CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
extern unsigned char g_imeCompositionAlpha_013750b0;   // 0x013750B0
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
void CIMEFont::SetColor(unsigned long colour)
{
 unsigned long high = colour >> 8;
 Colour = CRGBColour((unsigned char)(high >> 8), (unsigned char)high, (unsigned char)colour, g_imeCompositionAlpha_013750b0);
}
}