// NUISystem::CIMEFont::DrawTextW(const unsigned short* text) -- draw IME text with the Arial font bank picked by height: the colour takes the composition alpha, a black drop shadow two units down-left is drawn first unless the colour is dark (all channels <= 0x60), then the text at the converted position
// Grade: DIFFER(406v410) -- frame packing only: retail puts the address-taken `scale` local in the dead `text` parameter slot ([esp+0x40], 0x2c frame) and reads it once into ebp for both draws (saving ebp/edi); ours gives it its own slot (0x30 frame) and re-reads it per call. A copy of the scale after the position conversion moves it into the param slot but loads into edi (408v410)
// Leaf: DrawTextW
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00405020, length 410 bytes (`ret 4`).
// Callees: CWideString(const unsigned short*) 0x0099B6B0 / CWideString(const CWideString&) 0x0099B720 / ~CWideString 0x0099B510,
//   CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0, CIMEFont::GetFontNameAndScale 0x00404970 (landed), GFGetSystemManager
//   0x009A4EC0 (font bank +0x84, display manager +0x60 -> 2D render manager +8), the bank's GetFont 0x009E2C80 (counted
//   pointer by hidden pointer, as the landed NUISystem::CText::GetDataFromDef 0x0054ED90), GFACoordToVCoord 0x009E21D0
//   (__fastcall in, out), CRenderManager2D::Draw3DScaledText 0x009DBFF0 (landed), operator delete 0x00BFE9BC.
// Layout: CIMEFont as the landed GetFontNameAndScale 0x00404970 (vptr, Colour +4, Position +8, Height +0x10); the composition
//   alpha 0x013750B0 as the landed CIME::Render 0x00404C00; shadow offset 0x0122DCDC = 2.0f; wchar_t is `unsigned short` (no /Zc:wchar_t).
namespace NUISystem {
class CCharString {
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
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
        GFGetSystemManager()->DisplayManager->RenderManager2D->Draw3DScaledText(
            C3DVector(shadowPosition.X, shadowPosition.Y, 0.0f), scale, wideText, font.Data, shadow, 0);
    }
    GFGetSystemManager()->DisplayManager->RenderManager2D->Draw3DScaledText(
        C3DVector(position.X, position.Y, 0.0f), scale, wideText, font.Data, Colour, 0);
}
}
