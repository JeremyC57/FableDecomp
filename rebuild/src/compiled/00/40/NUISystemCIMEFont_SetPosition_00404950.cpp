// NUISystem::CIMEFont::SetPosition(int, int) -- store the IME text position as floats
// Retail 0x00404950, 17 bytes (vtable 0x0122DCBC slot 2, `ret 8`). Name: Ego_r.pdb ?SetPosition@CIMEFont@NUISystem@@UAEXHH@Z.
// Layout after the landed CIMEFont rows 0x00404970 / 0x00405020: CIMEFont derives CImeUiFont_Base (vtable 0x0122DCBC):
// Colour +4, Position +8, Height +0x10; composition alpha 0x013750B0; wchar_t is `unsigned short` (no /Zc:wchar_t).
class CCharString {
public:
 void* PStringData;
 CCharString();                                      // 0x0099E4B0
 ~CCharString();                                     // 0x0099EAE0
};
struct CCPPointerInfo {
 long References;
 void (__fastcall* Destroy)(void* object);
 void* Object;
};
template <class T> struct CCountedPointer {
 T* Data;
 CCPPointerInfo* Info;
 void Release()
 {
  if (Info) {
   --Info->References;
   if (Info->References == 0) {
    Info->Destroy(Info->Object);
    operator delete(Info);                         // 0x00BFE9BC
   }
  }
 }
 ~CCountedPointer() { Release(); Data = 0; Info = 0; }
};
struct C2DVector {
 float X, Y;
 C2DVector(float x, float y) : X(x), Y(y) {}
};
struct CFontBank {
 virtual void Slot0();
 virtual C2DVector GetTextExtent(const unsigned short* text, long flags);   // slot 1 (+4)
};
struct CFontBankSource {
 CCountedPointer<CFontBank> GetFont(const CCharString& name);   // 0x009E2C80
};
struct CSystemManager {
 unsigned char _pad00[0x84];
 CFontBankSource* FontBank;                          // +0x84
 CFontBankSource* GetFontBank() const { return FontBank; }
};
CSystemManager* __fastcall GFGetSystemManager();     // 0x009A4EC0
struct CRGBColour {
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
void CIMEFont::SetPosition(int x, int y)
{
 Position[0] = (float)x;
 Position[1] = (float)y;
}
}